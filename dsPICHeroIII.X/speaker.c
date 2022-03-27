// Define FCY for system
#define FCY 16000000UL
#include "xc.h"
#include "speaker.h"
#include "joystick.h"
#include "libpic30.h"
#include "math.h"
//
//#pragma config ICS = 2          // set to PGC2/PGD2
//#pragma config FNOSC = PRI      // Oscillator Source Selection (Primary Oscillator (XT, HS, EC))
//#pragma config POSCMD = HS      // Primary Oscillator Mode Select bits (HS Crystal Oscillator Mode)



void setupSpeaker(){
    // setting registers for speaker
    // set pin RD15 to an output - spk_enable = output
    TRISDbits.TRISD15 = 0;
    // A3 is speaker output
    TRISAbits.TRISA3 = 0;
    // enable speaker
    LATDbits.LATD15 = 1;
    // Enable DAC modules
    DACCTRL1Lbits.DACON = 1;
    //set clock for DAC, 2 for PLL
    DACCTRL1Lbits.CLKSEL = 11;
    //set no division for clock
    DACCTRL1Lbits.CLKDIV = 00;
    // Enable DAC 1
    DAC1CONLbits.DACEN = 1;
    // Enable DAC 1 output buffer
    DAC1CONLbits.DACOEN = 1;
}

void make_note(int noteNum){
    int vol_divider = 1;
    double modulate;
    int notes[] = {1224,917,300};
    int cycles = notes[noteNum];
    
    //length of note
    int x = 16000/cycles;
    x = x*5;
    
    int count = 0;
    int i = 0;

    while(count <= x){
        int y_value=get_joystick_y();
         // 3050-3100 considered neutral position
        if(y_value>3050&&y_value<3100){
            modulate=0;
        }
        else{
            modulate = get_whammy_value(y_value);
        }
        for (i = 0; i<=99; i++){
         DAC1DATHbits.DACDATH = (sine[i])+205;
        // argument of delay is cycles, f = 16Mhz/(100*cycles), the {-50} is a
        // correction factor to account for the clock cycles for all instructions
        // in the loop, and is subject to change if any instructions in loop change. 
        // Octave is either 1 or 2, dividing cycles by 2 in delay
        // function doubles the frequency.
         
         __delay32((cycles-50)-modulate); 
        } // end for
        i = 0;
        count++;
    } // end while 1
}
