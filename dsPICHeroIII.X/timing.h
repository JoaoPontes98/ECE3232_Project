#ifndef TIMER_H
#define TIMER_H

#define C4 1224
#define Db4 1065
#define D4 927
#define Eb4 806
#define E4 701
#define F4 610
#define Gb4 531
#define G4 462
#define Ab5 402
#define A5 350
#define Bb5 305
#define B5 265
#define C5 230

#define QUARTER_BEAT 15625L
#define GOOD_THRESHOLD 7500L
#define BAD_THRESHOLD 22500L

//60BPM, 70BPM, ... 120BPM
static int BPMS[] = {31250,26786,23438,20833,18750,17045,15625};

#endif