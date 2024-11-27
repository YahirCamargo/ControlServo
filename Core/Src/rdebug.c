#include "rdebug.h"
#include "stdarg.h"
#include "string.h"
#include "stdlib.h"


UART_HandleTypeDef *huartx;

void Initrdebug(UART_HandleTypeDef *_huartx){
	huartx = _huartx;
}

void transmit(const char* buffer){
	HAL_UART_Transmit(huartx, (uint8_t*)buffer, strlen(buffer), 100);
}

void rprintf(const char* format,...){
	va_list args;
	va_start(args,format);
	char buffer[256];
	vsnprintf(buffer,sizeof(buffer),format,args);
	buffer[strlen((char*)buffer)] = '\0';
	va_end(args);
	transmit(buffer);
}

void rmsg(const char* msg){
	transmit(msg);
}

void rdmsg(int n){
	char buffer[50];
	itoa(n,buffer,10);
	transmit(buffer);
}

