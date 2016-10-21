#include <xinu.h>

process topicsInit(void){
	int32 i = 0;

	for(i = 0; i < NTOPICS; i++){
		topicEntry *temp = &topicTab[i];
		temp->nSubscribers = 0;
		temp->topicSem = semcreate(1);
	}
}