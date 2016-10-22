#include <xinu.h>

process topicsInit(void){
	int32 i = 0;

	/* initializing topicTab entries  */
	for(i = 0; i < NTOPICS; i++){
		topicEntry *temp = &topicTab[i];
		temp->nSubscribers = 0;
		temp->topicSem = semcreate(1);
	}

	/* initializing pendingPublishCircularQueue */
	pendingPublishQueue.front = -1;
	pendingPublishQueue.rear = -1;
	pendingPublishQueue.mutex = semcreate(1);
	pendingPublishQueue.emptySlots = semcreate(BUFFER_MAX);
	pendingPublishQueue.fullSlots = semcreate(0);

	broker();
}