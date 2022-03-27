//Define FCY for system
#define FCY 16000000UL
#include "xc.h"
#include "speaker.h"
#include "joystick.h"
#include "libpic30.h"
#include "math.h"

//#pragma config ICS = 2          // set to PGC2/PGD2
//#pragma config FNOSC = PRI      // Oscillator Source Selection (Primary Oscillator (XT, HS, EC))
//#pragma config POSCMD = HS      // Primary Oscillator Mode Select bits (HS Crystal Oscillator Mode)

int sine[] = {
1843,1958,2073,2188,2301,2412,2521,2627,
2730,2830,2925,3017,3104,3186,3262,3333,
3398,3457,3510,3556,3595,3627,3652,3670,
3681,3685,3681,3670,3652,3627,3595,3556,
3510,3457,3398,3333,3262,3186,3104,3017,
2925,2830,2730,2627,2521,2412,2301,2188,
2073,1958,1843,1727,1612,1497,1384,1273,
1164,1058,955,855,760,668,581,499,
423,352,287,228,175,129,90,58,
33,15,4,0,4,15,33,58,
90,129,175,228,287,352,423,499,
581,668,760,855,955,1058,1164,1273,
1384,1497,1612,1727,1843
}; // size 100

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
   
   int notes[] = {1224,917,612};
   int cycles = notes[noteNum];
   
   //length of note
   int x = 16000/cycles;
   x = x*10;
   
   int count = 0;
   int i = 0;
   int modulate = 0;

   while(count <= x){
       
       for (i = 0; i<=99; i++){
        DAC1DATHbits.DACDATH = (sine[i])+205;
       // argument of delay is cycles, f = 16Mhz/(100*cycles), the {-50} is a
       // correction factor to account for the clock cycles for all instructions
       // in the loop, and is subject to change if any instructions in loop change. 
       // Octave is either 1 or 2, dividing cycles by 2 in delay
       // function doubles the frequency.
        
       // joystick input would go here
       if(PORTBbits.RB12 == 0){ // checks for modulation input
           modulate = 50;
       }else{
           modulate = 0;
       }
        
        __delay32((cycles-50)+modulate); 
       } // end for
       i = 0;
       count++;
   } // end while 1
}