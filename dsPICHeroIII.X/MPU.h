#ifndef MPU_H
#define	MPU_H

#include <xc.h> 


void I2CNack(void);
void I2CStop(void);
void I2CStart(void);
void I2CWaitACK(void);
void I2CIdle(void);
void I2CWrite(unsigned char c);
void I2CRead(void);
int MPU_READ(void);
void Write(unsigned char addr,unsigned char reg,unsigned char value);
int Read(char addr, char reg);
#endif