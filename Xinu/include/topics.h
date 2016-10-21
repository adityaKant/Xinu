#define NTOPICS		256
#define	NSUBSCRIBERS	8

#define	isbadtopic(x)	( ((topic16)(x) < 0) || \
			  ((topic16)(x) >= NTOPICS))

typedef struct subscriberEntry{
	pid32 processId;
	void (*callback)(topic16,uint32);
}subscriberEntry;

typedef struct topicEntry{
	subscriberEntry subscribersTab[NSUBSCRIBERS];
	int16 nSubscribers;
	// uint32 data;
	// bool8 dataPublished;
	sid32 topicSem;
}topicEntry;

extern topicEntry topicTab[NTOPICS];
