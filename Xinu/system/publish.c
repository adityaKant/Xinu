#include <xinu.h>

void produce(topic16, void *, uint32);

syscall  publish(topic16 topic, void *data, uint32 size){

	intmask mask;
	uint8* groupNTopicId;

	mask = disable();

	void* dataCopy = makeCopy(data,size);
	groupNTopicId = hexToDec(topic);

	if (isbadtopic(*(groupNTopicId+1))) {
		kprintf("\nBAD TOPICID IN publish");
		restore(mask);
		return SYSERR;
	}

	if (isbadgroup(*groupNTopicId)) {
		kprintf("\nBAD GROUPID IN publish");
		restore(mask);
		return SYSERR;
	}
	
	wait(pendingPublishQueue.emptySlots);
	wait(pendingPublishQueue.mutex);
	
	produce(topic,dataCopy,size);

	signal(pendingPublishQueue.mutex);
	signal(pendingPublishQueue.fullSlots);
	
	restore(mask);
	return OK;

}

void produce(topic16 topic, void *data, uint32 size){

	topicEntry *topicPtr;
	uint8* groupNTopicId;
	groupNTopicId = hexToDec(topic);
	topicPtr = &topicTab[*(groupNTopicId+1)];
	int i;

	if((pendingPublishQueue.front == 0 && pendingPublishQueue.rear == BUFFER_MAX-1) || pendingPublishQueue.front == (pendingPublishQueue.rear + 1))
		kprintf("\nOVERFLOW");
	else{
		if(pendingPublishQueue.rear == -1){
			pendingPublishQueue.rear = pendingPublishQueue.front = 0;
		}
		else if(pendingPublishQueue.rear == BUFFER_MAX-1){
			pendingPublishQueue.rear = 0;
		}
		else{
			++pendingPublishQueue.rear;
		}

		pendingPublishQueue.queue[pendingPublishQueue.rear].topic = topic;
		pendingPublishQueue.queue[pendingPublishQueue.rear].data = data;
		pendingPublishQueue.queue[pendingPublishQueue.rear].nSubscribers = topicPtr->nSubscribers;
		pendingPublishQueue.queue[pendingPublishQueue.rear].size = size;

		kprintf("\nPUBLISHED to topic: 0x%04X, data ", topic);
		for(i = 0;i < size;i++)
			kprintf("->%d",(*(int*)data+i));
	}
}