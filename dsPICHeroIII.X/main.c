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
void homeState();
void playGame();

int isGameOn = 0;
int goodLedTimer = 0;
int okayLedTimer = 0;
int missLedTimer = 0;
int totalMissed = 0;
int comboNotes = 0;

int main(void) {
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
        playGame();
    }

    return 0;
}

// Run when waiting to start game
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
    isGameOn = 1;
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
    
    while(totalMissed < 5){
        if(comboNotes > 2){
            //Star power LED on
            //LATBbits.LATB12 = 1;
            
//            if(MPU_READ() == 1){
//                // Make star power LEDs blink
//                LATBbits.LATB12 = 0;
//            }
        }
    }
    
    totalMissed = 0;
    isGameOn = 0;
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
    
    //Turn performance indicator LEDs on
    if(goodLedTimer == 0){
        //green LED off
        LATCbits.LATC7 = 0;
    } else {
        goodLedTimer--;
        comboNotes++;
        LATCbits.LATC7 = 1;
        //green LED on
    }
    if(okayLedTimer == 0){
        //yellow LED off
        LATCbits.LATC6 = 0;
    } else {
        okayLedTimer--;
        comboNotes++;
        LATCbits.LATC6 = 1;
        //yellow LED on
    }
    if(missLedTimer == 0){
        //red LED off
        LATCbits.LATC12 = 0;
    } else {
        missLedTimer--;
        totalMissed++;
        comboNotes = 0; 
        
        LATBbits.LATB12 = 0; // Star Power LED off
        LATCbits.LATC12 = 1; // red LED on
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
//    Nop();
//    Nop();
//    Nop();
//    Nop();
//    Nop();
    LAT_RCLK = 0;
//    Nop();
//    Nop();
//    Nop();
//    Nop();
//    Nop();
    LAT_SRCLK = 1;
//    Nop();
//    Nop();
//    Nop();
//    Nop();
//    Nop();
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
    if(isGameOn){
        make_note(0);
    }
    
    handleInput(0,inputTime);
    IFS0bits.INT0IF = 0;
    
}
void __attribute__((__interrupt__(auto_psv))) _INT1Interrupt(void) {
    int inputTime = TMR1;
    if(isGameOn){
        make_note(1);
    }
    handleInput(1,inputTime);
    IFS0bits.INT1IF = 0;

}

void __attribute__((__interrupt__(auto_psv))) _INT2Interrupt(void) {
    int inputTime = TMR1;
    if(isGameOn){
        make_note(2);
    }
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
    
    int offset;
    do {
        int timeDiff = pBottom->time - songTime;
        //filter out all that can't possibly be hits (too late)
        if(timeDiff < 0) {
            offset = inputTime - QUARTER_BEAT + (timeDiff * QUARTER_BEAT);
        } else {
            offset = inputTime + (timeDiff * QUARTER_BEAT);
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
        hitNote(1);
        okayLedTimer = 2;
        pBottom->hit = 2;
        pBottom++;
    } else if (offset <= GOOD_THRESHOLD) {
        //Great
        hitNote(2);
        goodLedTimer = 2;
        pBottom->hit = 3;
        pBottom++;
    } else if (offset <= BAD_THRESHOLD) {
        //Okay & early
        hitNote(1);
        okayLedTimer = 2;
        pBottom->hit = 4;
        pBottom++;
    }

}