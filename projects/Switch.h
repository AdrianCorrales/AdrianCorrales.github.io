/*
 * Switch.h
 *
 *  Created on: Nov 5, 2023
 *      Author: jonat
 */

#ifndef SWITCH_H_
#define SWITCH_H_

extern uint32_t temp;
extern int32_t rightb;
extern int32_t leftb;

// initialize your switches
void Switch_Init(void);

// return current state of switches
uint32_t Switch_In(void);


#endif /* SWITCH_H_ */
