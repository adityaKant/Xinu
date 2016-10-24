#include <xinu.h>

uint8 result[2];
uint8* hexToDec(uint16 value){
	result[0] = (value >> 8)  & 0xff;    
    result[1] = value & 0xff;

    return result;
}