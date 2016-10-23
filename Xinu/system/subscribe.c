#include <xinu.h>

syscall subscribe(topic16 topic, void (*callback)(topic16, uint32)){

	intmask mask;
	topicEntry *topicPtr;
	int16 nSubscribers;
	struct	procent *prPtr;

	mask = disable();

	if (isbadtopic(topic)) {
		kprintf("BAD TOPIC IN subscribe");
		restore(mask);
		return SYSERR;
	}

	prPtr = &proctab[currpid];
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

	prPtr->topicsSubscribed[prPtr->nTopics++] = topic;
	signal(topicPtr->topicSem);

	kprintf("\nProcess: %d, subcribed to topic: %d",currpid,topic);

	restore(mask);
	return OK;

} 