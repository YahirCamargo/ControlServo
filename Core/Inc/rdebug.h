#ifndef __RDEBUG_H__
#define __RDEBUG_H__

#define BUFF_SIZE 256
#include "stdio.h"
#include "main.h"

void Initrdebug(UART_HandleTypeDef *huartx);
void rprintf(const char* format,...);
void rmsg(const char* msg);
void rdmsg(int n);
void rfmsg(float f);


#endif
