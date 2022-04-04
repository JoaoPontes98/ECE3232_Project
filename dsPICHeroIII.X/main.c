/*
 * File:   main.c
 * Author: camfi
 *
 * Created on February 26, 2022, 11:56 PM
 */


#include "xc.h"
#include <libpic30.h>
#include "config.h"
#include "ledpins.h"
#include "game.h"
#include "setup.h"
#include "MPU.h"
#include "timing.h"
#include "joystick.h"
#include "speaker.h"
#define FCY 16000000UL

#define SONG_LENGTH 17

Note* pTop;
Note* pBottom;
int songTime = 0;
int gyro_flag = 0;
int x_value, y_value, modulate;
void handleInput(char,int);
void startTimer();
int goodLedTimer = 0;
int okayLedTimer = 0;
int missLedTimer = 0;
int bpmIndex = 0;
int inStarPower = 0;

int main(void) {
    TRISCbits.TRISC13 = 0;
    TRISCbits.TRISC14 = 0;
    TRISCbits.TRISC15 = 0;
    
    setupPins();
    setupTimer();
    setupInterrupts();
    setupJoystick();
    setupSpeaker();
    setupDigital();
    setup_MPU();

    // Main Game loop
    while(1){
        homeState();
        metronome(4, 60);
        playGame();
    }

    return 0;
}

void homeState(){
    // Turn all LEDs on
    LATCbits.LATC12 = 1;
    LATCbits.LATC6 = 1;
    LATCbits.LATC7 = 1;
    
    while(1){
        if((PORTBbits.RB2 == 0 || PORTBbits.RB8 == 0)
          ||PORTBbits.RB9 == 0){ // any button is pressed
            //turn off all LEDs
            LATCbits.LATC12 = 0;
            LATCbits.LATC6 = 0;
            LATCbits.LATC7 = 0;
            
            break;
        }
    }
    
    return;
}

void playGame(){
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
    pTop = song;
    pBottom = song;
    startTimer();

    while(1){
        while(songTime < 65){
            //Poll gyro
            //Log max gryo value since last interrupt
            //Should only be polled when the star power mode is ready   
            
            if(starPowerReady() && inStarPower == 0){
                LATBbits.LATB12 = 1;
                if(MPU_READ() == 1){
                    gyro_flag = 1;
                }
                if(gyro_flag){
                    inStarPower = 1;
                }
            }else if(!inStarPower){
                LATBbits.LATB12 = 0;
            }
            
            if(inStarPower>0 && inStarPower < 30){
                if(inStarPower % 2 == 0){
                    LATBbits.LATB12 = 1;
                }else{
                    LATBbits.LATB12 = 0;
                }
            }else{
                LATBbits.LATB12 = 0;
                inStarPower=0;
            }
        }
        //Start song from beginning
        songTime = 0;
        pTop = song;
        pBottom = song;
        //Reset note state (waiting to be hit)
        for(int i = 0; i < SONG_LENGTH; i++){
            song[i].hit = 0;
        }
        //Increase BPM to a maximum BPM of 120
        if(bpmIndex < 6){
            bpmIndex++;
            PR1 = BPMS[bpmIndex];
        }
    }
}

void displayScore(int gameScore){
    int temp = gameScore % 100;
    
    if(temp < 25)
    {
        LATCbits.LATC13 = 0; //75
        LATCbits.LATC14 = 0; //50
        LATCbits.LATC15 = 0; //25
    }
    else{
    LATCbits.LATC15 = 1;// Light up 25 pt LED
    if(temp >= 50){LATCbits.LATC14 = 1; }
    if(temp >= 75){LATCbits.LATC13 = 1; }
    }
    
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
                    
    if(inStarPower){
        inStarPower++;
    }

    //Turn performance indicator LEDs on
    if(goodLedTimer == 0){
        //green LED off
        LATCbits.LATC7 = 0;
    } else {
        goodLedTimer--;
        //green LED on
        LATCbits.LATC7 = 1;
    }
    if(okayLedTimer == 0){
        //yellow LED off
        LATCbits.LATC6 = 0;
    } else {
        okayLedTimer--;
        //yellow LED on
        LATCbits.LATC6 = 1;
    }
    if(missLedTimer == 0){
        //red LED off
        LATCbits.LATC12 = 0;
    } else {
        missLedTimer--;
        //red LED on
        LATCbits.LATC12 = 1;
    }
    
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
    if(gyro_flag == 1){
        //Star power LED on
        LATBbits.LATB12 = 1;
        
    } else {
        LATBbits.LATB12 = 0;
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
            offset = inputTime - BPMS[bpmIndex] + ((long)timeDiff * BPMS[bpmIndex]);
        } else {
            offset = inputTime + ((long)timeDiff * BPMS[bpmIndex]);
        }

        if(offset <= (BAD_THRESHOLD * -1)){
            //miss, check again
            missNote(pBottom);
            missLedTimer = 2;
            pBottom++;
        }
    } while (offset <= (BAD_THRESHOLD * -1));
    
    if(pBottom->lane != lane){
        return;
    }
    
    if (offset <= (GOOD_THRESHOLD * -1)){
        //Okay & late
        
        if(!inStarPower){hitNote(1);}
        else{hitNote(4);}
        okayLedTimer = 2;
        pBottom->hit = 2;
        pBottom++;
    } else if (offset <= GOOD_THRESHOLD) {
        //Great
        if(!inStarPower){hitNote(2);}
        else{hitNote(4);}
        goodLedTimer = 2;
        pBottom->hit = 3;
        pBottom++;
    } else if (offset <= BAD_THRESHOLD) {
        //Okay & early
        if(!inStarPower){hitNote(1);}
        else{hitNote(4);}
        okayLedTimer = 2;
        pBottom->hit = 4;
        pBottom++;
    }
    
    displayScore(getGameScore());
    make_note(lane);
}