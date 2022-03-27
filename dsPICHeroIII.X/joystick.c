/*
 * File:   joystick.c
 * Author: olee
 *
 * Created on March 25, 2022, 4:09 AM
 */


#include "xc.h"
#include "joystick.h"
#include "mcc_generated_files/adc1.h"
#include "mcc_generated_files/system.h"

#define FCY 16000000UL
#include <libpic30.h>


//Function will give you the reading of the VRX pin of Joystick
// Example Code referenced and function definitions are from the MPLAB Code Configurator
int get_joystick_x(){
    ADC1_Enable();
    ADC1_ChannelSelect(x_axis);
    ADC1_SoftwareTriggerEnable();
    __delay_ms(100);
    ADC1_SoftwareTriggerDisable();
    while (!ADC1_IsConversionComplete(x_axis));
    int x_value=ADC1_ConversionResultGet(x_axis);
    ADC1_Disable();
    return x_value;
}

//Function will give you the reading of the VRY pin of Joystick
// Example Code referenced and function definitions are from the MPLAB Code Configurator
int get_joystick_y(){
    ADC1_Enable();
    ADC1_ChannelSelect(y_axis);
    ADC1_SoftwareTriggerEnable();
    __delay_ms(100);
    ADC1_SoftwareTriggerDisable();
    while (!ADC1_IsConversionComplete(y_axis));
    int y_value=ADC1_ConversionResultGet(y_axis);
    ADC1_Disable();
    return y_value;
}

// Function to get the whammy scaling factor to modulate the note frequency
// will translate y axis value from 0-4095 to a proportional value 0.5-1.5
double get_whammy_value(int input){
    int y_max=4095;
    int y_min=0;
    int mod_max=1.5;
    int mod_min=0.5;
    
    double slope = (y_max-y_min) / (mod_max - mod_min);
    double output=mod_min+slope*(input-y_min);
    return output;
}