#ifndef KERNEL_H
#define KERNEL_H


int interrupt(int int_number, int AX, int BX, int CX, int DX);
void putInMemory(int segment, int address, char character);

void printString(char* str); 
void readString(char* buf);  
void clearScreen(void);      
#endif
