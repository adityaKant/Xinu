#define BUFFER_MAX  10

typedef struct qEntry{
	topic16 topic;
	void *data;
	uint32 size;
	int16 nSubscribers;
}qEntry;

typedef struct brokerQ{
	qEntry queue[BUFFER_MAX];
	int16 front, rear;
	sid32 mutex, emptySlots, fullSlots;
}brokerQ;

extern brokerQ pendingPublishQueue;