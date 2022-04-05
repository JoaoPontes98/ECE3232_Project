#ifndef GAME_H
#define GAME_H


typedef struct note {
    char lane;
    int time;
    int freq;
    signed char hit;
} Note;
#define HIT_NONE 0

void awardPoints(int);

void incrementStarPower();

void missNote(Note*);

void hitNote(int);

int getGameScore();

void setGameScore(int num);

int getFailCount();

void setFailCount(int num);

void resetStarPower();

#endif