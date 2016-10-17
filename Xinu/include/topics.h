#define NTOPICS		256
#define	NSUBSCRIBERS	8

typedef struct subscriberEntry{
	pid32 processId;
	void (*handler)(topic16,uint32);
}subscriberEntry;

typedef struct topicEntry{
	subscriberEntry subscriberTab[NSUBSCRIBERS];
	int16 nSubscribers;
	uint32 data;
	bool6 dataPublished;
}topicEntry;

extern topicEntry topicTab[NTOPICS];
