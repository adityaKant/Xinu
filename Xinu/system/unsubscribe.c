#include <xinu.h>

void removeTopicFromPr(topic16);
void removeProcessId(subscriberEntry*,int16,pid32);

syscall  unsubscribe(topic16  topic){

	intmask mask;
	topicEntry *topicPtr;
	int16 nSubscribers, i;
	bool8 presentFlag;
	presentFlag = FALSE;
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

	topicPtr = &topicTab[*(groupNTopicId+1)];
	nSubscribers = topicPtr->nSubscribers;

	for(i = 0; i <topicPtr->nSubscribers;i++){
		if(topicPtr->subscribersTab[i].processId == currpid){
			presentFlag = TRUE;
			break;
		}
	}
	if(presentFlag == FALSE){
		kprintf("\nUNSUBSCRIBE FAILURE: Process: %s is not subscribed to topic: 0x%04X", proctab[currpid].prname,topic);
		restore(mask);
		return SYSERR;
	}

	wait(topicPtr->topicSem);

	removeProcessId(topicPtr->subscribersTab,nSubscribers,currpid);
	topicPtr->nSubscribers--;

	removeTopicFromPr(topic);

	signal(topicPtr->topicSem);

	kprintf("\nProcess: %s, unsubcribed to topic: 0x%04X",proctab[currpid].prname,topic);

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