#include <xinu.h>

syscall subscribe(topic16 topic, void (*callback)(topic16, uint32)){

	intmask mask;
	topicEntry *topicPtr;
	int16 nSubscribers,i;
	struct	procent *prPtr;
	uint8* groupNTopicId;

	mask = disable();

	groupNTopicId = hexToDec(topic);

	if (isbadtopic(*(groupNTopicId+1))) {
		kprintf("\nBAD TOPICID IN subscribe");
		restore(mask);
		return SYSERR;
	}

	if (isbadgroup(*groupNTopicId)) {
		kprintf("\nBAD GROUPID IN subscribe");
		restore(mask);
		return SYSERR;
	}

	prPtr = &proctab[currpid];
	topicPtr = &topicTab[*(groupNTopicId+1)];
	nSubscribers = topicPtr->nSubscribers;

	for(i = 0; i < prPtr->nTopics; i++){
		if(prPtr->topicsSubscribed[i] == *(groupNTopicId+1)){
			kprintf("\nSUBSCRIBE FAILURE: Process: %d already subscribed to topic: 0x%X",currpid,*(groupNTopicId+1));
			restore(mask);
			return SYSERR;
		}
	}

	wait(topicPtr->topicSem);

	if(nSubscribers == 8){
		kprintf("\nMAX subscribers reached for topic: 0x%X",*(groupNTopicId+1));
		restore(mask);
		signal(topicPtr->topicSem);
		return SYSERR;
	}

	topicPtr->subscribersTab[nSubscribers].processId = currpid;
	topicPtr->subscribersTab[nSubscribers].callback = callback;
	topicPtr->subscribersTab[nSubscribers].groupId = *groupNTopicId;
	topicPtr->nSubscribers++;

	prPtr->topicsSubscribed[prPtr->nTopics++] = topic;
	signal(topicPtr->topicSem);

	kprintf("\nProcess: %d, subscribed to topic: 0x%04X",currpid,topic);

	restore(mask);
	return OK;

} 