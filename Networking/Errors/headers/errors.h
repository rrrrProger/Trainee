#ifndef ERRORS_H
#define ERRORS_H

enum {SUCCESS, MEMORY_ALLOCATION_ERROR, FILE_NOT_EXISTS_ERROR, PARSING_INPUT_ERROR, 
	  ERR_LISTEN, ERR_ACCEPT, ERR_UNKNOWN, ERR_CHOICE, ERR_BIND,
	  ERR_CREATE_SOCKET, ERR_CONNECT, ERR_TIMEOUT, ERR_SEND};

int error(int err); 

#endif /* ERRORS_H */