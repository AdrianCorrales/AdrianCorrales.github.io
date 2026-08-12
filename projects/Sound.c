// Sound.c
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// Jonathan Valvano
// 11/15/2021 
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"
#include "Arrays.h"

const uint8_t *CurrSound;
static uint32_t SoundPosition=0, SoundFlag=0, TotLength;

void SysTick_IntArm(uint32_t period, uint32_t priority){
  // write
    SysTick->CTRL=0x00;
    SysTick->LOAD=period-1;
    SCB->SHP[1]=(SCB->SHP[1]&(~0xC0000000))|priority<<30;
    SysTick->VAL=0;
    SysTick->CTRL=0x07;
}
// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC

void Sound_Init(void){
// write this
  DAC5_Init();
  SysTick_IntArm(7256,0);
}

void SysTick_Handler(void){ // called at 11 kHz
  // output one value to DAC if a sound is active
    if(SoundFlag==1){
        DAC5_Out(CurrSound[SoundPosition]);
        SoundPosition++;
        if(SoundPosition == TotLength){
            DAC5_Out(0);
            SoundFlag=0;
        }
    }
}

//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement

void Sound_Start(const uint8_t *pt, uint32_t count){
// write this
  CurrSound = pt;
  SoundPosition = 0;
  TotLength = count;
  SoundFlag=1;
  
  // In main, define KickCount=2304 and GoalCount=34048
}

void Sound_Kick(void){
// write this
  
}
void Sound_Goal(void){
// write this
  
}
void Sound_Explosion(void){
// write this
 
}

void Sound_Fastinvader1(void){
  
}
void Sound_Fastinvader2(void){
  
}
void Sound_Fastinvader3(void){
  
}
void Sound_Fastinvader4(void){
  
}
void Sound_Highpitch(void){
  
}
