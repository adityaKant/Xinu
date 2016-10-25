#include <xinu.h>

void produce(topic16, uint32);

syscall  publish(topic16  topic,  uint32  data){

	intmask mask;
	uint8* groupNTopicId;

	mask = disable();

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
	
	produce(topic,data);

	signal(pendingPublishQueue.mutex);
	signal(pendingPublishQueue.fullSlots);
	
	restore(mask);
	return OK;

}

void produce(topic16 topic, uint32 data){

	topicEntry *topicPtr;
	uint8* groupNTopicId;
	groupNTopicId = hexToDec(topic);
	topicPtr = &topicTab[*(groupNTopicId+1)];

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

		kprintf("\nPUBLISHED to topic: 0x%04X, data: %d", topic,data);
	}
}