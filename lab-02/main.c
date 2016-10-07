/*  main.c  - main */

#include <xinu.h>

pid32 senders_id;
pid32 receiver_id;
pid32 sender_id;
pid32 singleSend_id;

int32 consumed_count = 0;
const int32 CONSUMED_MAX = 100;
const int32 BUFFER_MAX = 10;


process senderNreciever(void)
{	
	int msgArr[] = {11,22,33,44,55,66,77,88,99,110,220};
	umsg32 msgs[6];

	sendMsgs(receiver_id,msgArr,11);
	receiveMsgs(msgs,2);
	return OK;
}

/* Place the code for the buffer consumer here */
process receiver(void)
{	
	int32 msg_count = 10;
	umsg32 msgs[10];

	receiveMsgs(msgs,msg_count);
	return OK;
}

process sender(void)
{	

	sendMsg(senders_id,9999);

	return OK;
}

process broadcast(void){
	int32 successfulBroadcast;
	pid32 pids[] = {senders_id,receiver_id};
	successfulBroadcast = sendnMsg(2,pids,80085);
	if(successfulBroadcast == -1)
		kprintf("\nSYSERR in sendnMsg DUE TO BAD PID");
	else
		kprintf("\nsuccessfull Broadcast: %d",successfulBroadcast);
}

process	main(void)
{

	senders_id = create(senderNreciever, 4096, 50, "senderNreciever", 0);
	receiver_id = create(receiver, 4096, 50, "receiver", 0);
	sender_id = create(broadcast, 4096, 50, "broadcast", 0);
	singleSend_id = create(sender, 4096, 50, "sender", 0);
	
	resume(senders_id);
	resume(sender_id);
	resume(receiver_id);	
	resume(singleSend_id);

	return OK;
}
