#ifndef TIMER_H
#define TIMER_H

#define C4 1224
#define Db4 1155
#define D4 1090
#define Eb4 1029
#define E4 971
#define F4 917
#define Gb4 866
#define G4 817
#define Ab5 771
#define A5 728
#define Bb5 687
#define B5 648
#define C5 612

#define QUARTER_BEAT 15625L
#define GOOD_THRESHOLD 7500L
#define BAD_THRESHOLD 22500L

//60BPM, 70BPM, ... 120BPM
static int BPMS[] = {31250,26786,23438,20833,18750,17045,15625};

#endif