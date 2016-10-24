#define NTOPICS		256
#define	NSUBSCRIBERS	8

#define	isbadtopic(x)	( ((uint8)(x) < 0) || \
			  ((uint8)(x) >= NTOPICS))
#define isbadgroup(x)	( ((uint8)(x) < 0) || \
			  ((uint8)(x) >= NTOPICS))

typedef struct subscriberEntry{
	pid32 processId;
	void (*callback)(topic16,uint32);
	uint8 groupId;
}subscriberEntry;

typedef struct topicEntry{
	subscriberEntry subscribersTab[NSUBSCRIBERS];
	int16 nSubscribers;
	sid32 topicSem;
}topicEntry;

extern topicEntry topicTab[NTOPICS];
