#include "static_stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

int static_stack_init(struct data *d)
{

	struct static_stack *st = malloc(sizeof(struct static_stack));
	st->top = -1;
	static char *arr[MAX_SIZE] = {""};
	st->arr = arr;
	d->data_type_pnt = st;
	return TRUE;

}
int static_stack_push(struct data *d, void *data)
{
	
	struct static_stack *st = (struct static_stack*)(d->data_type_pnt);
	if (static_stack_is_full(d) == FALSE) {
		st->arr[++st->top] = data;

		return TRUE;
	}
	return FALSE;
}
int static_stack_is_empty(struct data *d)
{
	
	struct static_stack *st = (struct static_stack*)(d->data_type_pnt);
	if (st->top == -1) {

		return TRUE;
	} else {

		return FALSE;
	}
	
}
int static_stack_pop(struct data *d)
{

	struct static_stack *st = (struct static_stack*)(d->data_type_pnt);
	if (static_stack_is_empty(d) == FALSE) {
		st->arr[st->top--] = NULL;
	}
	
	return TRUE;
	
}
int static_stack_is_full(struct data *d)
{
	
	struct static_stack *st = (struct static_stack*)(d->data_type_pnt);
		if (st->top == MAX_SIZE) {
		return TRUE;
	}
	return FALSE;
	
}
int static_stack_print(struct data *d)
{

	struct static_stack *st = (struct static_stack*)(d->data_type_pnt);
	FILE *stream;
	if (d->filename) {
		FILE *f = fopen(d->filename, "w");
		if (!f)
			return FALSE;
		stream = f;
	}
	else {
		stream = stdout;
	}
	for (int i = 0; i <= st->top; i++) {
		fprintf(stream, "\n%s\n", st->arr[i]);
	}
	if (stream)
		fclose(stream);
	return TRUE;
}
int static_stack_upload(struct data *d)
{
	return TRUE;
}
int static_stack_download(struct data *d)
{
	return TRUE;
}
