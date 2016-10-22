#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

//Declaring processes
pid32 A;
pid32 B;
pid32 C;
pid32 D;


void foo(topic16 t, uint32 data){
	
	kprintf("\n----FOO-Displaying Subscribed Data----");	
	kprintf("\nTopic:%d , Data: %d",t,data);
}

void bar(topic16 t, uint32 data){
	
	kprintf("\n----BAR-Displaying Subscribed Data----");	
	kprintf("\nTopic:%d , Data: %d",t,data);
}


/* Code process1 */
process a(void) {

	topic16 t=10;
	kprintf("\nProcess A Subscribing to %d",t);
	subscribe(t, &foo);

	return OK;
}

/* Code process2 */
process b(void) {
	/* */
	uint32 data1 = 77;
	uint32 data2 = 42;
	topic16 t=10;
	// kprintf("\nProcess B publishing %d to topic %d",data1 , t);
	publish(t, data1);
	
	resume(C);
	
	// kprintf("\nProcess B publishing %d to topic %d",data2 , t);
	publish(t, data2);

	return OK;
}

/* Code process3 */
process c(void) {
	/* */
	topic16 t=10;
	// kprintf("\nProcess C Subscribing to %d",t);
	subscribe(t, &bar);

	return OK;
}

/* Code process4 */
process d(void) {
	/* */
	uint32 data1 = 17;
	topic16 t=7;
	// kprintf("\nProcess D publishing %d to topic %d",data1 , t);
	publish(t, data1);
	
	return OK;
}


//Main function
process main(void) {

	//recvclr();
	// kprintf("\nStarted Program Execution\n");

	//creating sample processes with varying priorities
	A = create(a, 4096, 50, "Process-A", 0);
	B = create(b, 4096, 50, "Process-B", 0);
	C = create(c, 4096, 50, "Process-C", 0);
	D = create(d, 4096, 50, "Process-D", 0);

	//processes are created in suspend mode, so resuming processes
	resume(A);
	resume(B);
	//resume c is in process b
	resume(D);


	// kprintf("\nEnd of Program Execution\n");

	return OK;
}