#include <reg51.h>
#define uint unsigned int
#define uchar unsigned char
sbit P11=P1^1;
void main(){
	while(1){
		P11=0;
	}
}