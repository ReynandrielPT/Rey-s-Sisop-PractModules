#include "kernel.h"
#include "std_lib.h"

int main() {
  char buf[128];
  clearScreen();
  printString("Welcome to MengOS\n");

  while (true) {
    printString("$ ");
    readString(buf);
    printString(buf);
    printString("\n");
  }
}

void printString(char* str) {
  interrupt(0x10, 0xE00 + '\r', 0, 0, 0);
  while (*str) {
    interrupt(0x10, 0xE00 + *str++, 0, 0, 0);
  }
}

void readString(char *buf) {
  int i = 0;
  int input;
  char ah, al;
  input = interrupt(0x16, 0, 0, 0, 0);
  ah = input >> 8;
  al = input & 0x00FF;
  while (al != 0xD && ah != 0x48 && ah != 0x50) {
    if (al != 0x8) {
      buf[i++] = al;
      interrupt(0x10, 0xE00 + al, 0, 0, 0);
    } else if (i > 0) {
      interrupt(0x10, 0xE00 + '\b', 0xF, 0, 0);
      interrupt(0x10, 0xE00 + ' ', 0xF, 0, 0);
      interrupt(0x10, 0xE00 + '\b', 0xF, 0, 0);
      buf[--i] = 0;
    }
    input = interrupt(0x16, 0, 0, 0, 0);
    ah = input >> 8;
    al = input & 0x00FF;
  }
  interrupt(0x10, 0xE00 + '\n', 0xF, 0, 0);
  interrupt(0x10, 0xE00 + '\r', 0xF, 0, 0);
  buf[i] = 0;
}

void clearScreen() {
  int i;
  interrupt(0x10, 0x02 << 8, 0, 0, 0);
  for (i = 0; i < 80 * 25; i++) {
    putInMemory(0xB000, 0x8000 + i * 2, ' ');
    putInMemory(0xB000, 0x8001 + i * 2, 0x0F);
  }
}
