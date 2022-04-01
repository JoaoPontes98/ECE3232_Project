/*
 * File:   setup.c
 * Author: camfi
 *
 * Created on March 18, 2022, 12:57 AM
 */

#include "setup.h"
#include "ledpins.h"
#include "mcc_generated_files/adc1.h"
#include "timing.h"
void setupSccp() {
    
    //Input Capture Mode
    CCP1CON1H = 0x4080;
    CCP1CON1L = 0x80B1;

}

void setupPins() {
    TRIS_SER1 = TRIS_OUT;
    TRIS_SER2 = TRIS_OUT;
    TRIS_SER3 = TRIS_OUT;
    TRIS_RCLK = TRIS_OUT;
    TRIS_SRCLK = TRIS_OUT;
    TRIS_SRCLR = TRIS_OUT;
    ANSEL_SER1 = 0;
    ANSEL_SER2 = 0;
    ANSEL_SER3 = 0;
    LAT_SRCLR = 0;
    LAT_SER1 = 0;
    LAT_SER2 = 0;
    LAT_SER3 = 0;
    LAT_RCLK = 0;
    LAT_SRCLK = 0;
    LAT_SRCLR = 1;
}

void setupDigital(){
    //LEDS~~~~~~~~~~~~~~~~~~~~~~~~~
    //RC12 = redLED
    TRISCbits.TRISC12=0;
    //RC6 = yellow LED
    TRISCbits.TRISC6=0;
    //RC7 = green LED
    TRISCbits.TRISC7=0;
    //RB12 = star power LED's
    TRISBbits.TRISB12 = 0;
    
    //BUTTONS~~~~~~~~~~~~~~~~~~~~~~~
    //RB7 = red button
    TRISBbits.TRISB7 = 1;
    //RB8 = yellow button
    TRISBbits.TRISB8 = 1;
    //RB9 = green button
    TRISBbits.TRISB9 = 1;
}

void setupInterrupts(){
    //Timer interrupts
    IPC0bits.T1IP = 5;
    IEC0bits.T1IE = 1;
    //External interrupts (INT0, INT1, INT2)
    IPC0bits.INT0IP = 6;
    IEC0bits.INT0IE = 1;
}

void setupTimer(){
    //BPM Timer
    // The main ranges in this word that we care about are:
    // b15      - 0     - Timer off
    // b9-8     - 11    - Select FRC Clock
    // b5-4     - 10    - 1:64 input clock prescale (1 timer count = 64 clock cycles)
    // b2       - 1     - synchronizes External Clock input
    // b1       - 1     - Use External Clock selected with b9-8
    T1CON = 0x0326;
    // 1 timer count = 0.000008s
    PR1 = QUARTER_BEAT;        //0.125s
}

// Function will set up the joystick and the adc so that functions in joystick.c can be used
void setupJoystick(){
    ADC1_Initialize();
    
     // RD10 = x_axis
    ANSELDbits.ANSELD10 = 1; 
    TRISDbits.TRISD10 = 1;
    // RD11 = y_axis
    ANSELDbits.ANSELD11 = 1; 
    TRISDbits.TRISD11 = 1;
}