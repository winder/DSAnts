/*
  CPU Usage - http://forums.devkitpro.org/viewtopic.php?f=6&t=415

  original Source by eKid
  adapted by Ryouarashi and Weirdfox
*/

#include "cpu_usage.h"
#include "nds/timers.h"

int localTimer1 = 0;
int localTimer2 = 1;

void CPU_StartTest(int timer1, int timer2) {
    localTimer1 = timer1;
    localTimer2 = timer2;

    TIMER_CR(timer1) = 0;
    TIMER_CR(timer2) = 0;

    TIMER_DATA(timer1) = 0;
    TIMER_DATA(timer2) = 0;

    TIMER_CR(timer2) = TIMER_CASCADE | TIMER_ENABLE;
    TIMER_CR(timer1) = TIMER_ENABLE;
}

int CPU_EndTest() {
    TIMER_CR(localTimer1) = 0;
    TIMER_CR(localTimer2) = 0;

    //return TIMER0_DATA | (localTimer1_DATA<<16);
    return ((TIMER_DATA(localTimer1) | (TIMER_DATA(localTimer2)<<16)) * 100 + (560190/2)) / 560190;
}
