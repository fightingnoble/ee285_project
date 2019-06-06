#ifndef __HCSR04_H
#define __HCSR04_H

#include "sys.h"
void Trig_Pulse(void);
void HC_SR04_Init(void);
void HC_SR04_Count(void);
void Echo_Count(void);

//#define Echo PBin(12)
#define Trig PBout(13)

#endif

