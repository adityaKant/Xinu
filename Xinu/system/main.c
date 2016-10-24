#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

//Declaring processes
pid32 A;
pid32 B;
pid32 C;
pid32 D;
pid32 E;
pid32 F;
pid32 G;
pid32 H;


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
	sleep(1);

	return OK;
}
process e(void) {

	topic16 t=10;
	kprintf("\nProcess E Subscribing to %d",t);
	subscribe(t, &foo);
	sleep(1);

	return OK;
}
process f(void) {

	topic16 t=10;
	kprintf("\nProcess F Subscribing to %d",t);
	subscribe(t, &foo);
	sleep(1);

	return OK;
}
process g(void) {

	topic16 t=10;
	kprintf("\nProcess G Subscribing to %d",t);
	subscribe(t, &foo);
	sleep(1);

	return OK;
}
process h(void) {

	topic16 t=10;
	kprintf("\nProcess H Subscribing to %d",t);
	subscribe(t, &foo);
	sleep(1);

	return OK;
}

/* Code process2 */
process b(void) {
	/* */
	topic16 t=10;
	kprintf("\nProcess B Subscribing to %d",t);
	subscribe(t, &foo);
	sleep(1);

	return OK;
}

/* Code process3 */
process c(void) {
	topic16 t=10;
	kprintf("\nProcess C Subscribing to %d",t);
	subscribe(t, &foo);
	sleep(1);

	return OK;
}

/* Code process4 */
process d(void) {
	topic16 t=10;
	kprintf("\nProcess D Subscribing to %d",t);
	subscribe(t, &foo);
	sleep(1);

	return OK;
}


//Main function
process main(void) {

	//recvclr();
	// kprintf("\nStarted Program Execution\n");
	int32 i = 0;
	uint8 *temp;

	//creating sample processes with varying priorities
	A = create(a, 4096, 50, "Process-A", 0);
	B = create(b, 4096, 50, "Process-B", 0);
	C = create(c, 4096, 50, "Process-C", 0);
	D = create(d, 4096, 50, "Process-D", 0);
	E = create(e, 4096, 50, "Process-E", 0);
	F = create(f, 4096, 50, "Process-F", 0);
	G = create(g, 4096, 50, "Process-G", 0);
	H = create(h, 4096, 50, "Process-H", 0);

	//processes are created in suspend mode, so resuming processes
	// resume(A);
	// resume(B);
	// resume(C);
	// resume(D);
	// resume(E);
	// resume(F);
	// resume(G);
	// resume(H);
	// subscribe(10,&bar);
	// publish(10,99);
	
	uint16 topic = 0x013f;
	temp = hexToDec(topic);
	kprintf("\n%u\n%u",*temp,*(temp+1));

	// for(i = 0;i<30;i++){
	// 	publish(22,i);
	// }
	// sleep(2);


	// kprintf("\nEnd of Program Execution\n");
}
