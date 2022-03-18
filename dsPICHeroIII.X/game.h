#ifndef GAME_H
#define GAME_H


typedef struct note {
    char lane;
    int time;
    int freq;
    signed char hit;
} Note;

#define HIT_NONE 0
#define HIT_OK 1
#define HIT_GOOD 2
#define HIT_MISS -1

void awardPoints(int);

void incrementStarPower();

void missNote(Note*);

void hitNote(Note*,int);

#endif