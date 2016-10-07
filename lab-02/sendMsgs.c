#include <xinu.h>

#define BUFFER_MAX 10

uint32 sendMsgs(
	pid32 pid,
	umsg32* msgs,
	uint32	msg_count
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	msgQueueStruct *prMsgStream;	//Ptr to process's msg queue structure
	int32 successMsgCount,i, msgsDroppedCount;
	successMsgCount = msgsDroppedCount = 0;

	mask = disable();
	if (isbadpid(pid)) {
		kprintf("BAD PID IN sendMsgs");
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	prMsgStream = &(prptr->prMsgStream);

	wait(prMsgStream->mutex);

	if (prMsgStream->nMsgs >= BUFFER_MAX) {
		kprintf("RECIEVER's QUEUE IS FULL");
		signal(prMsgStream->mutex);
		restore(mask);
		return SYSERR;
	}

	if(msg_count > BUFFER_MAX){
		msgsDroppedCount = msg_count - BUFFER_MAX;
		msg_count = BUFFER_MAX;
	}

	successMsgCount = msg_count - prMsgStream->nMsgs;	//Number of msgs that can be sent.

	for (i = 0; i < successMsgCount; i++){
		produce(prMsgStream,*(msgs + i));
		(prMsgStream->nMsgs)++;
		kprintf("\n sendMsgs SENT: %u to recipientId: %d",*(msgs + i),pid);
	}
	
	signal(prMsgStream->mutex);

	 // If recipient waiting or in timed-wait make it ready 

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} 
	else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return successMsgCount;
}