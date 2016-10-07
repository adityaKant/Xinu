/* sendMsg.c - sendMsg */

#include <xinu.h>

#define BUFFER_MAX 10

//forward declaration
umsg32 produce(msgQueueStruct *prMsgStream, umsg32 value);

/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	sendMsg(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	msgQueueStruct *prMsgStream;	//Ptr to process's msg queue structure

	mask = disable();
	if (isbadpid(pid)) {
		kprintf("BAD PID IN sendMsg");
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	prMsgStream = &(prptr->prMsgStream);
	
	wait(prMsgStream->mutex);

	if (prMsgStream->nMsgs >=10) {
		kprintf("RECIEVER'S QUEUE IS FULL");
		signal(prMsgStream->mutex);
		restore(mask);
		return SYSERR;
	}
	
	
	msg = produce(prMsgStream,msg);
	(prMsgStream->nMsgs)++;
	kprintf("\n sendMsg SENT: %u to recipientId: %d",msg,pid);
	signal(prMsgStream->mutex);

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} 
	else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}

umsg32 produce(msgQueueStruct *prMsgStream, umsg32 value){
	if((prMsgStream->front == 0 && prMsgStream->rear == BUFFER_MAX-1) || prMsgStream->front == ((prMsgStream->rear) + 1)){
		kprintf("\nOVERFLOW");
		exit();
	}
	else{
		if((prMsgStream->rear) == -1){
			(prMsgStream->rear) = (prMsgStream->front) = 0;
			(prMsgStream->buffer)[prMsgStream->rear] = value;
		}
		else if((prMsgStream->rear) == BUFFER_MAX-1){
			(prMsgStream->rear) = 0;
			(prMsgStream->buffer)[prMsgStream->rear] = value;
		}
		else
			(prMsgStream->buffer)[++(prMsgStream->rear)] = value;
		return value;
	}
}
