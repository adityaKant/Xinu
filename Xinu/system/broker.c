#include <xinu.h>

qEntry consume();
int16 fmin(int16,int16);

process broker(){
	while(TRUE){
		qEntry topicAndData;
		topicEntry *temp;
		int i;
		uint8* groupNTopicId;
		
		if(pendingPublishQueue.rear != -1){
			
			wait(pendingPublishQueue.fullSlots);
			wait(pendingPublishQueue.mutex);
			
			topicAndData = consume();
			
			signal(pendingPublishQueue.mutex);
			signal(pendingPublishQueue.emptySlots);

			groupNTopicId = hexToDec(topicAndData.topic);
			temp = &topicTab[*(groupNTopicId + 1)];
			
			wait(temp->topicSem);

			for(i = 0; i < fmin(temp->nSubscribers,topicAndData.nSubscribers); i++){
				if(temp->subscribersTab[i].groupId == *groupNTopicId || *groupNTopicId == 0 || temp->subscribersTab[i].groupId == 0){
					// kprintf("\ncalling handler %d",i);
					temp->subscribersTab[i].callback(topicAndData.topic,topicAndData.data);
				}
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
		topicAndData.nSubscribers = pendingPublishQueue.queue[pendingPublishQueue.front].nSubscribers;

		if(pendingPublishQueue.front == pendingPublishQueue.rear){
			pendingPublishQueue.front = -1;
			pendingPublishQueue.rear = -1;
		}
		else if(pendingPublishQueue.front == BUFFER_MAX-1)
			pendingPublishQueue.front = 0;
		else
			pendingPublishQueue.front++;
		// kprintf("\nIN BROKER topic: 0x%04X, data: %d",topicAndData.topic,topicAndData.data);
		return topicAndData; 
	}
}

int16 fmin(int16 a, int16 b){
	if(a<b)
		return a;
	else
		return b;
}