/*
 * File:   MPU.c
 * Author: benja
 *
 * Created on March 26, 2022, 5:08 PM
 */


#include "xc.h"
#include "MPU.h"
#define FCY 16000000UL
#include <libpic30.h>
//int main(void){
//setup_MPU();
//MPU_READ();
//
//}

int MPU_READ(void){
    int buffer1 = 0;
    int buffer2 = 0;
    int buffer = 0;
//    while(1){ //While Loop Used for testing
    buffer1 = Read(208,71)* 256;
    buffer2 = Read(208,72);
    buffer = buffer1 + buffer2;
        if(buffer > 0x5000){
            return 1;
            
        }
//    }
    
    return 0;
}

 

 
void I2CNack(void)
{
    I2C1CONLbits.ACKDT = 1;      // Send NACK
    I2C1CONLbits.ACKEN = 1;      // Initiate Acknowledge and transmit ACKDT
    while(I2C1CONLbits.ACKEN);  
}
 
void I2CStop(void)
{
    I2C1CONLbits.RCEN = 0;       // receive mode not in progress
    I2C1CONLbits.PEN = 1;        // Stop condition
    while(I2C1CONLbits.PEN);
}
 
void I2CStart(void)
{
    I2C1CONLbits.SEN = 1;        // Start
    while(I2C1CONLbits.SEN);
    I2C1STATbits.TBF = 0;
}
 

 
void I2CWaitACK(void)
{
    while(I2C1STATbits.ACKSTAT);
}
 
void I2CIdle(void)
{
    while(I2C1STATbits.TRSTAT);
}
 
void I2CWrite(unsigned char c)
{   
    I2C1TRN = c;
    while(I2C1STATbits.TBF);
    I2CWaitACK();
    
}
 
void I2CRead(void)
{   
    I2C1CONLbits.RCEN = 1;
    while(!I2C1STATbits.RBF);
    
}
 
void Write(unsigned char addr, unsigned char reg,unsigned char value)
{
    // Start Condition
    I2CStart();
    //  Addr
    I2CWrite(addr);
    I2CIdle();
    // RA Byte
    I2CWrite(reg);
    I2CIdle();
    // Value
    I2CWrite(value);
    I2CIdle();
    // Stop
    I2CStop();
    I2CIdle(); 
}
 

int Read(char addr, char reg)
{
    int buffer = 0;
    // Start Condition
    I2CStart();
    //  Addr
    I2CWrite(addr);
    I2CIdle();
    // RA Byte
    I2CWrite(reg);
    I2CIdle();
    // Restart
    I2CStop();
    I2CStart();
    I2CWrite(209);
    I2CIdle();  
    I2CRead(); 
    buffer= I2C1RCV;
    I2CNack();
    I2CStop();  
    I2CIdle();  
    
    return buffer;
}
