// DSPIC33CK256MP506 Configuration Bit Settings

// 'C' source line config statements

// FSEC
#pragma config BWRP = OFF               // Boot Segment Write-Protect bit (Boot Segment may be written)
#pragma config BSS = DISABLED           // Boot Segment Code-Protect Level bits (No Protection (other than BWRP))
#pragma config BSEN = OFF               // Boot Segment Control bit (No Boot Segment)
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GSS = DISABLED           // General Segment Code-Protect Level bits (No Protection (other than GWRP))
#pragma config CWRP = OFF               // Configuration Segment Write-Protect bit (Configuration Segment may be written)
#pragma config CSS = DISABLED           // Configuration Segment Code-Protect Level bits (No Protection (other than CWRP))
#pragma config AIVTDIS = OFF            // Alternate Interrupt Vector Table bit (Disabled AIVT)

// FBSLIM
#pragma config BSLIM = 0x1FFF           // Boot Segment Flash Page Address Limit bits (Enter Hexadecimal value)

// FSIGN

// FOSCSEL
#pragma config FNOSC = FRCDIVN          // Oscillator Source Selection (Internal Fast RC (FRC) Oscillator with postscaler)
#pragma config IESO = OFF                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)
#pragma config PLLKEN = ON              // PLL Lock Status Control (PLL lock signal will be used to disable PLL clock output if lock is lost)
#pragma config XTCFG = G3               // XT Config (24-32 MHz crystals)
#pragma config XTBST = ENABLE           // XT Boost (Boost the kick-start)

// FWDT
// RWDTPS = No Setting
#pragma config RCLKSEL = LPRC           // Watchdog Timer Clock Select bits (Always use LPRC)
#pragma config WINDIS = ON              // Watchdog Timer Window Enable bit (Watchdog Timer operates in Non-Window mode)
#pragma config WDTWIN = WIN25           // Watchdog Timer Window Select bits (WDT Window is 25% of WDT period)
// SWDTPS = No Setting
#pragma config FWDTEN = ON              // Watchdog Timer Enable bit (WDT enabled in hardware)

// FPOR
#pragma config BISTDIS = DISABLED       // Memory BIST Feature Disable (mBIST on reset feature disabled)

// FICD
#pragma config ICS = PGD2               // ICD Communication Channel Select bits (Communicate on PGC1 and PGD1)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)
#pragma config NOBTSWP = DISABLED       // BOOTSWP instruction disable bit (BOOTSWP instruction is disabled)

// FDMTIVTL
#pragma config DMTIVTL = 0xFFFF         // Dead Man Timer Interval low word (Enter Hexadecimal value)

// FDMTIVTH
#pragma config DMTIVTH = 0xFFFF         // Dead Man Timer Interval high word (Enter Hexadecimal value)

// FDMTCNTL
#pragma config DMTCNTL = 0xFFFF         // Lower 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF) (Enter Hexadecimal value)

// FDMTCNTH
#pragma config DMTCNTH = 0xFFFF         // Upper 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF) (Enter Hexadecimal value)

// FDMT
#pragma config DMTDIS = OFF             // Dead Man Timer Disable bit (Dead Man Timer is Disabled and can be enabled by software)

// FDEVOPT
#pragma config ALTI2C1 = OFF            // Alternate I2C1 Pin bit (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF            // Alternate I2C2 Pin bit (I2C2 mapped to SDA2/SCL2 pins)
#pragma config ALTI2C3 = OFF            // Alternate I2C3 Pin bit (I2C3 mapped to SDA3/SCL3 pins)
#pragma config SMBEN = SMBUS            // SM Bus Enable (SMBus input threshold is enabled)
#pragma config SPI2PIN = PPS            // SPI2 Pin Select bit (SPI2 uses I/O remap (PPS) pins)

// FALTREG
#pragma config CTXT1 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits (Not Assigned)
#pragma config CTXT2 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 2 bits (Not Assigned)
#pragma config CTXT3 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 3 bits (Not Assigned)
#pragma config CTXT4 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 4 bits (Not Assigned)

// FBTSEQ
#pragma config BSEQ = 0xFFF             // Relative value defining which partition will be active after device Reset; the partition containing a lower boot number will be active (Enter Hexadecimal value)
#pragma config IBSEQ = 0xFFF            // The one's complement of BSEQ; must be calculated by the user and written during device programming. (Enter Hexadecimal value)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include "xc.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define FCY 2000000UL
#define FOSC 4000000UL // FRC divided by 2 from 8MHz to 4MHz
#define RAND_MAX 4

#include <libpic30.h>

int main(void) {
    setupSpeaker();
    TRISCbits.TRISC15 = 1; //RX1 = btn input
    TRISBbits.TRISB14 = 0; //RX2 = led output
    
    while(1){
        if(PORTCbits.RC15 == 0){
            LATDbits.LATD15 = 1;
            LATBbits.LATB14 = 1;
        }else{
            LATDbits.LATD15 = 0;
            LATBbits.LATB14 = 0;
        }
//        LATDbits.LATD15 = 1;
//        __delay_ms(1000);
//        LATDbits.LATD15 = 0;
    }
    
    return 0;
}
void setupSpeaker()
{
    //Speaker
    TRISDbits.TRISD15 = 0; //D15 is output (Speaker)
    
    //Configure the source clock for the APLL
    ACLKCON1bits.FRCSEL = 1;
    // Configure the APLL prescalar, APLL feedback divider, and both APLL postscalars
    ACLKCON1bits.APLLPRE = 1; //N1 = 1
    APLLFBD1bits.APLLFBDIV = 125; // M = 125
    APLLDIV1bits.APOST1DIV = 2; // N2 = 2
    APLLDIV1bits.APOST2DIV = 1; // N3 = 1
    // Enable APLL
    ACLKCON1bits.APLLEN = 1;
    
    //DAC
    DACCTRL1Lbits.CLKSEL = 2;
    DACCTRL1Lbits.DACON = 1;
    DAC1CONLbits.DACEN = 1;
    DAC1CONLbits.DACOEN = 1;
    //Triangle Wave mode
    DAC1DATLbits.DACLOW = 0xCD; // Lower data value
    DAC1DATHbits.DACDAT = 0xF32; // Upper data value
    SLP1DATbits.SLPDAT = 0x1; // Slope rate, counts per step
    SLP1CONHbits.TWME = 1;
    SLP1CONHbits.SLOPEN = 1;
}
