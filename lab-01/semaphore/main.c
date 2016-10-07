/*  main.c  - main */

#include <xinu.h>

pid32 producer_id;
pid32 consumer_id;
pid32 timer_id;

int32 consumed_count = 0;
const int32 CONSUMED_MAX = 100;
const int32 BUFFER_MAX = 10;

/* Define your circular buffer structure and semaphore variables here */

sid32 mutex, emptySlots, fullSlots;

int32 bufferArray[10], front = -1 , rear = -1;

void produce(int value){
	if((front == 0 && rear == BUFFER_MAX-1) || front == (rear + 1))
		kprintf("\nOVERFLOW");
	else{
		if(rear == -1){
			rear = front = 0;
			bufferArray[rear] = value;
		}
		else if(rear == BUFFER_MAX-1){
			rear = 0;
			bufferArray[rear] = value;
		}
		else
			bufferArray[++rear] = value;
		kprintf("\nPRODUCED: %d, front: %d, rear: %d", value,front,rear);
	}
}

void consume(){
	int value;
	if(rear == -1)
		kprintf("\nUNDERFLOW");
	else{
		value = bufferArray[front];
		if(front == rear){
			front = -1;
			rear = -1;
		}
		else if(front == BUFFER_MAX-1)
			front = 0;
		else
			front++;
		kprintf("\nCONSUMED: %d, front: %d, rear: %d", value,front,rear);
	}
}

/* */

/* Place your code for entering a critical section here */
void mutex_acquire(sid32 mutex)
{
	/* */
	wait(mutex);
}

/* Place your code for leaving a critical section here */
void mutex_release(sid32 mutex)
{
	/* */
	signal(mutex);
}

/* Place the code for the buffer producer here */
process producer(void)
{
	/* */

	int32 i = 0;

	while(1){
		
			wait(emptySlots);
			mutex_acquire(mutex);
			produce(i);
			mutex_release(mutex);
			signal(fullSlots);
			i++;
		
	}
	
	

	return OK;
}

/* Place the code for the buffer consumer here */
process consumer(void)
{
	/* Every time your consumer consumes another buffer element,
	 * make sure to include the statement:
	 *   consumed_count += 1;
	 * this will allow the timing function to record performance */
	/* */

	while(1){
		
			wait(fullSlots);
			mutex_acquire(mutex);
			consume();
			consumed_count += 1;
			mutex_release(mutex);
			signal(emptySlots);
		
	}

	return OK;
}


/* Timing utility function - please ignore */
process time_and_end(void)
{
	int32 times[5];
	int32 i;

	for (i = 0; i < 5; ++i)
	{
		times[i] = clktime_ms;
		yield();

		consumed_count = 0;
		while (consumed_count < CONSUMED_MAX * (i+1))
		{
			yield();
		}

		times[i] = clktime_ms - times[i];
	}

	kill(producer_id);
	kill(consumer_id);

	for (i = 0; i < 5; ++i)
	{
		kprintf("\nTIME ELAPSED (%d): %d\n", (i+1) * CONSUMED_MAX, times[i]);
	}
}

process	main(void)
{
	mutex = semcreate(1);
	emptySlots = semcreate(10);
	fullSlots = semcreate(0);

	recvclr();

	/* Create the shared circular buffer and semaphores here */
	/* */

	producer_id = create(producer, 4096, 50, "producer", 0);
	consumer_id = create(consumer, 4096, 50, "consumer", 0);
	timer_id = create(time_and_end, 4096, 50, "timer", 0);

	resched_cntl(DEFER_START);
	resume(producer_id);
	resume(consumer_id);
	/* Uncomment the following line for part 3 to see timing results */
	resume(timer_id); 
	resched_cntl(DEFER_STOP);

	return OK;
}
