#include <xinu.h>

void removeTopicFromPr(topic16);
void removeProcessId(subscriberEntry,int16,pid32);

syscall  unsubscribe(topic16  topic){

	intmask mask;
	topicEntry *topicPtr;
	int16 nSubscribers;

	mask = disable();

	if (isbadtopic(topic)) {
		kprintf("BAD TOPIC IN unsubscribe");
		restore(mask);
		return SYSERR;
	}

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

	removeTopicFromPr(topic);

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
				*(subscribersTab + j) = *(subscribersTab + j + 1);
			}
			break;
		}
	}

}

void removeTopicFromPr(topic16 topic){
	
	int16 i,j;
	struct procent *prPtr;

	prPtr = &proctab[currpid];

	for(i = 0; i < prPtr->nTopics; i++){
		if(topic == prPtr->topicsSubscribed[i]){
			for(j = i; j < prPtr->nTopics-1; j++){
				prPtr->topicsSubscribed[j] = prPtr->topicsSubscribed[j+1];
			}
			break;
		}
	}
	prPtr->nTopics--;
}