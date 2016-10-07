#include <xinu.h>

#define BUFFER_MAX 10

uint32 sendnMsg(uint32 pid_count,pid32* pids,umsg32 msg)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	msgQueueStruct *prMsgStream;	//Ptr to process's msg queue structure
	int32 i, successfulCount = pid_count;

	mask = disable();

	for(i = 0; i<pid_count;i++){
		if (isbadpid(*(pids+i))) {
			restore(mask);
			return SYSERR;
		}
	}

	for(i = 0; i<pid_count;i++){
		prptr = &proctab[*(pids+i)];
		prMsgStream = &(prptr->prMsgStream);
	
		wait(prMsgStream->mutex);

		if (prMsgStream->nMsgs >=10) {
			kprintf("\nRECIEVER's QUEUE IS FULL");
			signal(prMsgStream->mutex);
			restore(mask);
			successfulCount--;
			continue;
		}

		msg = produce(prMsgStream,msg);
		(prMsgStream->nMsgs)++;
		kprintf("\nBroadcast Msg %u, recipientId: %d",msg,*(pids+i));

		if (prptr->prstate == PR_RECV) {
		ready(*(pids+i));
		} 
		else if (prptr->prstate == PR_RECTIM) {
			unsleep(*(pids+i));
			ready(*(pids+i));
		}
		signal(prMsgStream->mutex);
	}

	/* If recipient waiting or in timed-wait make it ready */
	restore(mask);		/* Restore interrupts */
	return successfulCount;
}