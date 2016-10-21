#include <xinu.h>

syscall  unsubscribe(topic16  topic){

	intmask mask;
	topicEntry *topicPtr;
	int16 nSubscribers;

	mask = disable();

	topicPtr = &topicTab[topic];
	nSubscribers = topicPtr->nSubscribers;

	wait(topicPtr->topicSem);

	if(nSubscribers == 0){
		kprintf("\nProcess not subscribed for the topic: %d",topic);
		restore(mask);
		signal(topicPtr->topicSem);
		return SYSERR;
	}

	removeProcessId(topicPtr->subscribersTab,nSubscribers,currpid);
	topicPtr->nSubscribers--;

	signal(topicPtr->topicSem);

	kprintf("\nProcess: %d, unsubcribed to topic: %d",currpid,topic);

	restore(mask);
	return OK;

}

void removeProcessId(subscriberEntry *subscribersTab,int16 nSubscribers,pid32 processId){

	int16 i,j;
	
	for(i = 0; i < nSubscribers; i++){
		if((subscribersTab + i)->processId == processId){
			for(j = i; j < nSubscribers - 1; j++){
				*(subscribersTab + i) = *(subscribersTab + i + 1);
			}
			break;
		}
	}

}