#include <xinu.h>

qEntry consume();

process broker(){
	while(TRUE){
		qEntry topicAndData;
		topicEntry *temp;
		int i;
		
		if(pendingPublishQueue.rear != -1){
			
			wait(pendingPublishQueue.fullSlots);
			wait(pendingPublishQueue.mutex);
			
			topicAndData = consume();
			
			signal(pendingPublishQueue.mutex);
			signal(pendingPublishQueue.emptySlots);

			temp = &topicTab[topicAndData.topic];
			
			wait(temp->topicSem);
			for(i = 0; i < temp->nSubscribers; i++){
				kprintf("\ncalling handler %d",i);
				temp->subscribersTab[i].callback(topicAndData.topic,topicAndData.data);
			}
			signal(temp->topicSem);
		}
	}	
}

qEntry consume(){

	qEntry topicAndData;

	if(pendingPublishQueue.rear == -1){
		kprintf("\nUNDERFLOW");
	}
	else{

		topicAndData.topic = pendingPublishQueue.queue[pendingPublishQueue.front].topic;
		topicAndData.data = pendingPublishQueue.queue[pendingPublishQueue.front].data;

		if(pendingPublishQueue.front == pendingPublishQueue.rear){
			pendingPublishQueue.front = -1;
			pendingPublishQueue.rear = -1;
		}
		else if(pendingPublishQueue.front == BUFFER_MAX-1)
			pendingPublishQueue.front = 0;
		else
			pendingPublishQueue.front++;
		kprintf("\nIN BROKER topic:%d, data: %d, front: %d, rear: %d",topicAndData.topic,topicAndData.data,pendingPublishQueue.front,pendingPublishQueue.rear);
		return topicAndData; 
	}
}