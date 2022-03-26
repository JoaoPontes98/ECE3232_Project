
#ifndef JOYSTICK_H
#define	JOYSTICK_H

#include <xc.h> // include processor files - each processor file is guarded. 

// Function to get the reading of the x-axis variable resistor of the joystick
int get_joystick_x();

// Function to get the reading of the x-axis variable resistor of the joystick
int get_joystick_y();

// Function to get the whammy scaling factor to modulate the note frequency
// will translate y axis value from 0-4095 to a proportional value 0.5-1.5
double get_whammy_value(int input);
#endif



