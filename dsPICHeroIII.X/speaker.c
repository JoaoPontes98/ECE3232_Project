//Define FCY for system
#define FCY 16000000UL
#include "xc.h"
#include "speaker.h"
#include "joystick.h"
#include "libpic30.h"
#include "math.h"

void make_note(int noteNum,int y_value){
   // int vol_divider = 1;
   
   
   int notes[] = {1224,917,612};
   int cycles = notes[noteNum];
   
   //length of note
   int x = 16000/cycles;
   x = x*5;
   
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
           modulate = get_whammy_value(y_value);
       }else{
           modulate = 1;
       }
        
        __delay32((cycles-50)*modulate); 
       } // end for
       i = 0;
       count++;
   } // end while 1
}