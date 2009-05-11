/*
  CPU Usage - http://forums.devkitpro.org/viewtopic.php?f=6&t=415

  original Source by eKid
  adapted by Ryouarashi and Weirdfox

  ---
  Usage:

  // Main Loop
  while(1)
  {
    int CPUPercent;

    // The arguments represent the two timers to use, make sure they are not already
    // used by your software.
    CPU_StartTest( 0, 1 );

    // do you stuff...

    CPU_Percent = CPU_EndTest();

    swiWaitForVBL();
  }
*/


#ifndef CPU_USAGE_H_INCLUDED
#define CPU_USAGE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void CPU_StartTest(int timer1, int timer2);
int  CPU_EndTest();

#ifdef __cplusplus
}
#endif

#endif
