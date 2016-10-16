/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */

 #define BUFFER_MAX 10

umsg32 consume(msgQueueStruct *prMsgStream);

umsg32	receiveMsg(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/
	msgQueueStruct *prMsgStream;

	mask = disable();
	prptr = &proctab[currpid];
	prMsgStream =  &(prptr->prMsgStream);

	if (prMsgStream->nMsgs == 0) {
		// kprintf("\nin recieveMsg(RecieverId: %d), WAITING for messeages to be RECEIVED",currpid);
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}

	wait(prMsgStream->mutex);

	msg = consume(prMsgStream);
	(prMsgStream->nMsgs)--;
	kprintf("\n receiveMsg(RecieverId: %d) RECEIVED: %u",currpid,msg);
	signal(prMsgStream->mutex);

	restore(mask);
	return msg;
}

umsg32 consume(msgQueueStruct *prMsgStream){
	umsg32 value;
	// kprintf("\nIN RECEIVER's CONSUME");
	if(prMsgStream->rear == -1){
		kprintf("\nUNDERFLOW");
		exit();
	}
	else{
		value = prMsgStream->buffer[prMsgStream->front];
		if(prMsgStream->front == prMsgStream->rear){
			prMsgStream->front = -1;
			prMsgStream->rear = -1;
		}
		else if(prMsgStream->front == BUFFER_MAX-1)
			prMsgStream->front = 0;
		else
			(prMsgStream->front)++;
		return value;
	}
}
