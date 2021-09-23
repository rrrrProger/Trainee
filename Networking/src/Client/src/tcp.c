#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <errno.h>
#include "tcp.h"
#include "executor.h"

int tcp_client_connect(struct client *cl, int port)
{
	short sockfd;
	int iRetval = -1;
	struct sockaddr_in remote = {0};
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		return ERR_CREATE_SOCKET;
	}


	remote.sin_addr.s_addr = inet_addr("127.0.0.1");
	remote.sin_family = AF_INET;
	remote.sin_port = htons(port);

	printf("\nConnecting on port : %d\n", port);

	iRetval = connect(sockfd, (struct sockaddr *)&remote, sizeof(struct sockaddr_in));
	cl->sockfd = sockfd;
	if (iRetval == -1) {
		return ERR_CONNECT;
	}

	return SUCCESS;
}

/**
 * Accepts connections, handshakes with clients
 */
int tcp_client_send(struct client *cl, char *buff)
{
	union u_frame *frame;
	uint32_t cmd_id;
	uint32_t packet_len;
	uint32_t cmd_len;
	uint32_t cmd_size;
	uint32_t structures_size;

	frame = malloc(sizeof(union u_frame));
	if (!frame) {
		printf("Memory problem\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	cmd_size = strlen(buff);
	structures_size = sizeof(struct packet_frame);

	cmd_id = TCP;
	packet_len = cmd_size + structures_size;
// + reconnect for client + change to u_data
	
	printf("CMD SIZE : %d\n", cmd_size);
	cmd_id = htonl(cmd_id);
	packet_len = htonl(packet_len);
	cmd_size = htonl(cmd_size);


	memcpy(frame->pkt.fields.cmd_data, buff, strlen(buff) * sizeof(char));
	memcpy(&(frame->pkt.fields.cmd_id), &cmd_id, sizeof(cmd_id));
	memcpy(&(frame->pkt.fields.cmd_len), &cmd_size, sizeof(cmd_size));
	memcpy(&(frame->pkt.header.packet_len), &packet_len, sizeof(uint32_t));

	if (write(cl->sockfd, (void*)(frame->u_data), structures_size + cmd_size) < 0) {
		printf("Error write");
		return ERR_WRITE;
	}

	printf("\n\n\nSent answer");

	return SUCCESS;
}

int tcp_client_receive(struct client *cl)
{
	void *recv_input;
	uint32_t packet_id;
	uint32_t comd_id;
	int size;
	uint32_t cmd_size;
	char *cmd_data;
	char *result;
	union u_frame *frame;
	uint32_t tcp_size = 1024;
	uint32_t packet_len;
	int i = 0;

	recv_input = malloc(sizeof(struct packet_frame));
	if (!recv_input) {
		printf("Memory problem\n");
		return MEMORY_ALLOCATION_ERROR;
	}
	frame = malloc(sizeof(union u_frame));
	if (!frame) {
		printf("Memory problem\n");
	}

	printf("Entered here socket : %d\n", cl->sockfd);
	printf("RECEIVED STRUCTURE:\n");

	//cast to struct 
	//payload after struct header -> (alligned) 
	//read in massive in union in u_data and s union in struct and net to host
	read(cl->sockfd, recv_input, 4);
	packet_len = ntohl(*((uint32_t*)recv_input));
	printf("PACKET_LEN : %d\n", packet_len);

	void *recv_from = malloc(packet_len);
	if (!recv_from) {
		printf("Memory problem\n");
		return MEMORY_ALLOCATION_ERROR;
	}

	if( (size = recv(cl->sockfd, frame->u_data, packet_len, 0)) >= 0) {
	} else {
		printf("Receive SIZE error\n");
	}

	packet_id = (*((uint32_t*)(frame->u_data)));
	cmd_size = (*((uint32_t*)(frame->u_data + 2 * sizeof(uint32_t))));

	cmd_data = malloc(cmd_size);
	if (!cmd_data) {
		return MEMORY_ALLOCATION_ERROR;
	}

	memcpy(cmd_data, (frame->u_data + 3 * sizeof(uint32_t)), cmd_size);

	result = client_executor(cmd_data);

	tcp_client_send(cl, result);

	free(recv_input);
	free(frame);
	free(recv_from);
//	free(cmd_data);
	free(result);

	return SUCCESS;
}