/*
 * File:   main.c
 * Author: camfi
 *
 * Created on February 26, 2022, 11:56 PM
 */

#define FCY 16000000UL
#include "xc.h"
#include <libpic30.h>
#include "config.h"
#include "ledpins.h"
#include "game.h"
#include "setup.h"
#include "timing.h"
#include "joystick.h"
#include "MPU.h"


#define SONG_LENGTH 17

Note* pTop;
Note* pBottom;
int songTime = 0;
int x_value, y_value;
int sine_i = 0;
int sine_count = 0;
int sine_cycles;
int gyro_flag = 0;
void handleInput(char,int);
void startTimer();

int main(void) {
    setupPins();
    setupTimer();
    setupInterrupts();
    setupJoystick();
    __delay_ms(30);
    setup_MPU();
   
    //Hot Crossed Buns
    Note song[SONG_LENGTH] = { 
        {.lane = 2, .time = 0 , .freq = E4, .hit = 0},
        {.lane = 1, .time = 4 , .freq = D4, .hit = 0 },
        {.lane = 0, .time = 8 , .freq = C4, .hit = 0 },
        {.lane = 2, .time = 16, .freq = E4, .hit = 0 },
        {.lane = 1, .time = 20, .freq = D4, .hit = 0 },
        {.lane = 0, .time = 24, .freq = C4, .hit = 0 },
        {.lane = 0, .time = 32, .freq = C4, .hit = 0 },
        {.lane = 0, .time = 34, .freq = C4, .hit = 0 },
        {.lane = 0, .time = 36, .freq = C4, .hit = 0 },
        {.lane = 0, .time = 38, .freq = C4, .hit = 0 },
        {.lane = 1, .time = 40, .freq = D4, .hit = 0 },
        {.lane = 1, .time = 42, .freq = D4, .hit = 0 },
        {.lane = 1, .time = 44, .freq = D4, .hit = 0 },
        {.lane = 1, .time = 46, .freq = D4, .hit = 0 },
        {.lane = 2, .time = 48, .freq = E4, .hit = 0 },
        {.lane = 1, .time = 52, .freq = D4, .hit = 0 },
        {.lane = 0, .time = 56, .freq = C4, .hit = 0 }
     };
    while(1){
        songTime = 0;
    
        pTop = song;
        pBottom = song;

        startTimer();
        while(songTime < 65){
            Nop();
            
            //Poll gyro
            //Log max gryo value since last interrupt
            if(MPU_READ() == 1){
                gyro_flag = 1;
            }
            
        }
    }
    return 0;
}



void startTimer(){
    T1CONbits.TON = 1;
}

void __attribute__((__interrupt__(auto_psv))) _T1Interrupt(void) {
//    Get all notes from the note queue
//	Output notes
//		Notes are written to digital I/O
//		RCLK pulse
//		SRCLK pulse
    
    songTime++;
    
    while(pTop->time < songTime){
        pTop++;
    } 
    LAT_SER1 = 0;
    LAT_SER2 = 0;
    LAT_SER3 = 0;
    while(pTop->time == songTime){
        switch (pTop->lane) {
            case 0:
                LAT_SER1 = 1;
                break;
            case 1:
                LAT_SER2 = 1;
                break;
            case 2:
                LAT_SER3 = 1;
                break;
            default:
                break;
        }
        pTop++;
    }
    LAT_RCLK = 1;
    LAT_RCLK = 0;

    LAT_SRCLK = 1;
    LAT_SRCLK = 0;

//	if (star power ready):
//		Check MPU
//		if(Star power ok):
//			transition to star power
    if(gyro_flag == 1){
        //Star power LED on
        LATBbits.LATB12 = 0;
        
    } else {
        LATBbits.LATB12 = 1;
        //Star power LED off
    }
    gyro_flag = 0;
    
    IFS0bits.T1IF = 0;
}

void __attribute__((__interrupt__(auto_psv))) _INT0Interrupt(void) {
    int inputTime = TMR1;
    
    handleInput(0,inputTime);
    
    IFS0bits.INT0IF = 0;
}
void __attribute__((__interrupt__(auto_psv))) _INT1Interrupt(void) {
    int inputTime = TMR1;
    
    handleInput(1,inputTime);
    
    IFS0bits.INT1IF = 0;
}

void __attribute__((__interrupt__(auto_psv))) _INT2Interrupt(void) {
    int inputTime = TMR1;
    
    handleInput(2,inputTime);
    IFS1bits.INT2IF = 0;
}

void handleInput(char lane, int inputTime){
    //Check note queue
    //If hit:
    //  mark note as hit
    //  Calculate score based on accuracy
    //  Award points
    //Take earliest note possible
    
    long offset;
    do {
        int timeDiff = 8 + pBottom->time - songTime;
        //filter out all that can't possibly be hits (too late)
        if(timeDiff < 0) {
            offset = inputTime - QUARTER_BEAT + ((long)timeDiff * QUARTER_BEAT);
        } else {
            offset = inputTime + (timeDiff * QUARTER_BEAT);
        }

        if(offset <= (BAD_THRESHOLD * -1)){
            //miss, check again
            missNote(pBottom);
            pBottom++;
        }
    } while (offset <= (BAD_THRESHOLD * -1));
    
    if(pBottom->lane != lane){
        return;
    }
    
    if (offset <= (GOOD_THRESHOLD * -1)){
        //Okay & late
        hitNote(1);
        pBottom->hit = 2;
        pBottom++;
    } else if (offset <= GOOD_THRESHOLD) {
        //Great
        hitNote(2);
        pBottom->hit = 3;
        pBottom++;
    } else if (offset <= BAD_THRESHOLD) {
        //Okay & early
        hitNote(1);
        pBottom->hit = 4;
        pBottom++;
    }

}

//void __attribute__((__interrupt__(auto_psv))) _CCP1Interrupt(void) {
//
//    IFS0bits.CCP1IF = 0;
//}