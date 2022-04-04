/*
 * File:   game.c
 * Author: camfi
 *
 * Created on March 18, 2022, 12:11 AM
 */


#include "xc.h"
#include "game.h"

int gameScore = 0;

int starPower = 0;
int failCount = 0;

#define MAX_STAR_POWER 3

void awardPoints(int pts){
    gameScore += pts;
}

void incrementStarPower(){
    if(starPower >= MAX_STAR_POWER){
        return;
    }
    starPower += 1;
}

void missNote(Note* pNote){
    starPower = 0;
    failCount += 1;
    pNote->hit = 1;
}

void hitNote(int score){
    incrementStarPower();
    gameScore += score;
}

char starPowerReady(){
    if(starPower >= MAX_STAR_POWER){
        return 1;
    }
    return 0;
}

int getGameScore(){
    return gameScore;
}
