#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

//Declaring processes
pid32 A;
pid32 B;

void processA(topic16 t,void* data, uint32 size){
	int i;
	kprintf("\n----processA-Displaying Subscribed Data----");
	
	for(i = 0; i < size; i++){
		kprintf("\nTopic: 0x%04X , Data: %d",t,*((int*)data+i));
	}
	
	freemem(data,size);
}

void processB(topic16 t,void* data, uint32 size){
	
	int i;
	kprintf("\n----processB-Displaying Subscribed Data----");
	for(i = 0; i < size; i++)
		kprintf("\nTopic: 0x%04X , Data: %d",t,*((int*)data+i));
	freemem(data,size);
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

//Main function
process main(void) {

	int i;
	topic16 t=0x003F;

    int value[2];
    value[0] = 1;
    value[1] = 2;
    void *ptr = value;

	A = create(a, 4096, 50, "Process-A", 0);
	B = create(b, 4096, 50, "Process-B", 0);

	resume(A);
	resume(B);

	publish(t,ptr,2);
	value[0] = 2;

	kprintf("\n----values changes in main after publishing value----");
	for(i = 0; i < 2; i++)
		kprintf("\nvalue[%d]: %d",i,*(value+i));
}
