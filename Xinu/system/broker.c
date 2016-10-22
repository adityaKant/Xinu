#include <xinu.h>

void broker(){
	while(TRUE){
		qEntry topicAndData;
		topicEntry *temp;
		int i;


		wait(pendingPublishQueue.fullSlots);
		wait(pendingPublishQueue.mutex);
		topicAndData = consume();
		signal(pendingPublishQueue.mutex);
		signal(pendingPublishQueue.emptySlots);

		temp = &topicTab[topicAndData.topic];
		for(i = 0; i < temp->nSubscribers; i++)
			temp->subscribersTab[i].callback(topicAndData.topic,topicAndData.data);
		signal(temp->topicSem);
		
	}	
}

qEntry consume(){

	qEntry topicAndData;

	if(rear == -1){
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
		return topicAndData; 
	}
}