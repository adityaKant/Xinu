#include <xinu.h>

syscall subscribe(topic16 topic, void (*callback)(topic16, uint32)){

	intmask mask;
	topicEntry *topicPtr;
	int16 nSubscribers;

	mask = disable();

	topicPtr = &topicTab[topic];
	nSubscribers = topicPtr->nSubscribers;

	wait(topicPtr->topicSem);

	if(nSubscribers == 8){
		kprintf("\nMAX subscribers reached for topic: %d",topic);
		restore(mask);
		signal(topicPtr->topicSem);
		return SYSERR;
	}

	topicPtr->subscribersTab[nSubscribers].processId = currpid;
	topicPtr->subscribersTab[nSubscribers].callback = callback;
	topicPtr->nSubscribers++;

	signal(topicPtr->topicSem);

	kprintf("\nProcess: %d, subcribed to topic: %d",currpid,topic);

	restore(mask);
	return OK;

} 