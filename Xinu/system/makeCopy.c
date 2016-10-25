#include <xinu.h>

void* makeCopy(void *data, uint32 size){
	// int i;

	void *newptr = (void *)getmem( size * (sizeof(data)) );
	memcpy(newptr, data, size * (sizeof(data)));
	return newptr;
}