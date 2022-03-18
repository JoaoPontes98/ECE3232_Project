/*
 * File:   sccp.c
 * Author: camfi
 *
 * Created on February 26, 2022, 11:56 PM
 */


#include "xc.h"

int init_sccp(void) {
//    CCP1CON1H = 0x4080;
//    CCP1CON1L = 0x80A1; 
    
    //Input Capture Mode
    CCP1CON1H = 0x4080;
    CCP1CON1L = 0x80B1;
    
    //BPM Timer
    T1CON = 0x0326;
    PR1 = 15625;
    //Start timer
//    T1CONbits.TON = 1;
    
    
    
    return 0;
}

void start_game_timer(){
    T1CONbits.TON = 1;      //Start Timer
                            //enable interrupt flag??
}