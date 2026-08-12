/*
 * Functions.h
 *
 *  Created on: Apr 21, 2024
 *      Author: ryank
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

extern int32_t rightb;
extern int32_t leftb;
extern int32_t rightsp;
extern int32_t leftsp;
extern int32_t vxr;
extern int32_t vxl;
extern int32_t vyr;
extern int32_t vyl;
extern int32_t oldxr;
extern int32_t oldyr;
extern int32_t oldxl;
extern int32_t oldyl;
extern int32_t xBall, yBall, oldxBall, oldyBall;
extern int32_t coverxr, coveryr, coverxl, coveryl;
extern int32_t xr;
extern int32_t yr;
extern int32_t xl;
extern int32_t yl;
extern uint8_t positionflag, BackgroundFlagL, BackgroundFlagR, BackgroundFlagBall;
extern uint8_t jumpflagl;
extern uint8_t jumpflagr;
extern uint8_t lvlflag, leftchanged, rightchanged;
extern uint8_t languageflag;
extern uint32_t temp, LeftScore, RightScore;
extern int32_t timer;
extern int32_t ballvx;
extern int32_t ballvy;
extern uint8_t collisionflag;
extern uint8_t charactercollisionflag;

int32_t LateralVelocity(int32_t n);
void MoveLeft(void);
void MoveRight(void);
void JumpLeft(void);
void JumpRight(void);
void gravityl(void);
void gravityr(void);

void checkcollisionc(int32_t xc, int32_t yc);
void checkcollisionwall(void);
void moveball(void);
void ballvelocity(void);
void checkcharactercollision(void);

void DrawSprite(int32_t xlo, int32_t ylo, int32_t xlonew, int32_t ylonew, const unsigned short *image, uint32_t width, uint32_t length);
void BlacktoBackground(uint32_t xlo, uint32_t ylo, const unsigned short *image, uint32_t width, uint32_t length);

void Countdown(void);
void InitPositions(void);
void PrintEngGoal(void);
void PrintSpnGoal(void);
void PrintOneWonEng(void);
void PrintTwoWonEng(void);
void PrintTieEng(void);
void PrintOneWonSpn(void);
void PrintTwoWonSpn(void);
void PrintTieSpn(void);
void PrintTitle(void);
void ClickButtonEng(void);
void ClickButtonSpn(void);

#endif /* FUNCTIONS_H_ */
