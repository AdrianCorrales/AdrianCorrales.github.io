/*
 * Switch.c
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "Arrays.h"
#include "Functions.h"
#include "../inc/Clock.h"
#include "../inc/ST7735.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    // write this
    IOMUX->SECCFG.PINCM[PB16INDEX]=0x00040081;
    IOMUX->SECCFG.PINCM[PB17INDEX]=0x00040081;
  
}
// return current state of switches
void Switch_In(void){
    // write this
    temp = GPIOB->DIN31_0;
    temp &= 0x00030000;
    rightb = temp >> 16;
    leftb = temp >> 17;
}
