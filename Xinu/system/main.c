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


void processA(topic16 t, uint32 data){
	
	kprintf("\n----processA-Displaying Subscribed Data----");	
	kprintf("\nTopic: 0x%04X , Data: %d",t,data);
}

void processB(topic16 t, uint32 data){
	
	kprintf("\n----processB-Displaying Subscribed Data----");	
	kprintf("\nTopic: 0x%04X , Data: %d",t,data);
}

void processC(topic16 t, uint32 data){
	
	kprintf("\n----processC-Displaying Subscribed Data----");	
	kprintf("\nTopic: 0x%04X , Data: %d",t,data);
}

void processD(topic16 t, uint32 data){
	
	kprintf("\n----processD-Displaying Subscribed Data----");	
	kprintf("\nTopic: 0x%04X , Data: %d",t,data);
}

void processE(topic16 t, uint32 data){
	
	kprintf("\n----processE-Displaying Subscribed Data----");	
	kprintf("\nTopic: 0x%04X , Data: %d",t,data);
}

void bar(topic16 t, uint32 data){
	
	kprintf("\n----bar-Displaying Subscribed Data----");	
	kprintf("\nTopic: 0x%04X , Data: %d",t,data);
}


/* Code process1 */
process a(void) {

	topic16 t=0x003f;
	kprintf("\nProcess A Subscribing to 0x%04X",t);
	subscribe(t, &processA);
	sleep(1);

	return OK;
}

/* Code process2 */
process b(void) {
	/* */
	topic16 t= 0x013f;
	kprintf("\nProcess B Subscribing to 0x%04X",t);
	subscribe(t, &processB);
	sleep(1);

	return OK;
}

/* Code process3 */
process c(void) {
	topic16 t=0x023f;
	kprintf("\nProcess C Subscribing to 0x%04X",t);
	subscribe(t, &processC);
	sleep(1);

	return OK;
}

/* Code process4 */
process d(void) {
	topic16 t=0xff00;
	kprintf("\nProcess D Subscribing to 0x%04X",t);
	subscribe(t, &processD);
	sleep(1);

	return OK;
}

process e(void) {
	topic16 t=0x3f2ff;
	kprintf("\nProcess E Subscribing to 0x%04X",t);
	subscribe(t, &processE);
	sleep(1);

	return OK;
}


//Main function
process main(void) {

	// kprintf("\nStarted Program Execution\n");
	topic16 t=0x0000;


	void* ptr;
    int value[2];
    value[0] = 1;
    value[1] = 2;
    // ptr  = &value;
    ptr = memCopy(value,2);
    kprintf("*ptr:%d *ptr+1: %d", *ptr, *(ptr+1))


	//creating sample processes with same priorities
	A = create(a, 4096, 50, "Process-A", 0);
	B = create(b, 4096, 50, "Process-B", 0);
	C = create(c, 4096, 50, "Process-C", 0);
	D = create(d, 4096, 50, "Process-D", 0);
	E = create(e, 4096, 50, "Process-E", 0);
	// F = create(f, 4096, 50, "Process-F", 0);
	// G = create(g, 4096, 50, "Process-G", 0);
	// H = create(h, 4096, 50, "Process-H", 0);

	//processes are created in suspend mode, so resuming processes
	resume(A);
	resume(B);
	resume(C);
	resume(D);
	resume(E);
	// resume(F);
	// resume(G);
	// resume(H);
	subscribe(t,&bar);

	publish(t,101);
	t = 0x003f;
	publish(t,102);
	t = 0x023f;
	publish(t,103);
	t = 0xff00;
	publish(t,104);


	// kprintf("\nEnd of Program Execution\n");
}

void* memCopy(void *data, uint32 size){
	return (void *)getmem( size * (sizeof(data)) );
}
