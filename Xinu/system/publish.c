#include <xinu.h>

syscall  publish(topic16  topic,  uint32  data){
	
	wait(pendingPublishQueue.emptySlots);
	mutex_acquire(pendingPublishQueue.mutex);
	produce(topic,data);
	mutex_release(pendingPublishQueue.mutex);
	signal(pendingPublishQueue.fullSlots);

}

void produce(topic16  topic,  uint32  data){
	if((pendingPublishQueue.front == 0 && pendingPublishQueue.rear == BUFFER_MAX-1) || pendingPublishQueue.front == (pendingPublishQueue.rear + 1))
		kprintf("\nOVERFLOW");
	else{
		if(pendingPublishQueue.rear == -1){
			pendingPublishQueue.rear = pendingPublishQueue.front = 0;
			pendingPublishQueue.queue[pendingPublishQueue.rear].topic = topic;
			pendingPublishQueue.queue[pendingPublishQueue.rear].data = data;
		}
		else if(pendingPublishQueue.rear == BUFFER_MAX-1){
			pendingPublishQueue.rear = 0;
			pendingPublishQueue.queue[pendingPublishQueue.rear].topic = topic;
			pendingPublishQueue.queue[pendingPublishQueue.rear].data = data;
		}
		else{
			pendingPublishQueue.queue[++pendingPublishQueue.rear].topic = topic;
			pendingPublishQueue.queue[++pendingPublishQueue.rear].data = data;
		}
		kprintf("\nPUBLISHED to topic: %d, data: %d", topic,data);
	}
}