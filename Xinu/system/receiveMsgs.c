/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */

#define BUFFER_MAX 10

syscall	receiveMsgs(
	umsg32* msgs,
	uint32	msg_count
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	msgQueueStruct *prMsgStream;
	int32 i;

	mask = disable();
	prptr = &proctab[currpid];
	prMsgStream =  &(prptr->prMsgStream);

	while(1){
		if (prMsgStream->nMsgs == 0 || msg_count > prMsgStream->nMsgs) {
			// kprintf("\nIn recieveMsgs(RecieverId: %d), WAITING for messeages to be RECEIVED",currpid);
			prptr->prstate = PR_RECV;
			resched();		/* Block until message arrives	*/
		}
		else{
			break;
		}
	}

	wait(prMsgStream->mutex);

	for(i = 0; i < msg_count; i++){
		*(msgs+i) = consume(prMsgStream);
		(prMsgStream->nMsgs)--;
		kprintf("\n receiveMsgs(RecieverId: %d) RECEIVED: %u",currpid,*(msgs+i));
	}

	// msg = consume(prMsgStream);
	
	signal(prMsgStream->mutex);

	restore(mask);
	return OK;
}