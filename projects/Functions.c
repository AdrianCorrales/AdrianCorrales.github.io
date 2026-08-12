/*
 * Functions.c
 *
 *  Created on: Apr 21, 2024
 *      Author: ryank
 */

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "Arrays.h"
#include "Functions.h"
#include "../inc/Clock.h"
#include "../inc/ST7735.h"

#define floorcollision 4;
#define wallcollision 3;
#define charactercollisionright 2;
#define charactercollisionleft 1;
#define goalcollision 5;
#define charactercollisiontop 6;
#define charactercollisionbottom 7;

int32_t LateralVelocity(int32_t n){
    int32_t velo = (3*(n-2048))/2048;
    return velo;
}

void MoveLeft(void){
    yl = yl+vyl; // integration
    xl = xl+vxl; // integration
    if(yl <= 30){
        yl = 30;
    }
    if(yl>=150){
        yl = 150;
    }
}

void MoveRight(void){
    yr = yr+vyr; // integration
    xr = xr+vxr; // integration
    if(yr <= 30){
        yr = 30;
    }
    if(yr >= 150){
        yr = 150;
    }
}

void checkcharactercollision(void){
    if ((yr- 20)<= yl | yl >= (yr - 20)){
        vyl = 0;
        vyr = 0;
        yr = yr + 5;
        yl = yl - 5;
    }
}

void JumpLeft(void){
    if (leftb == 1){
        jumpflagl = 1;
        vxl = 5;
    }
}

void JumpRight(void){
    if (rightb == 1){
        jumpflagr = 1;
        vxr = 5;
    }
}

void gravityl(void){
    if (jumpflagl == 1){
        vxl = vxl - 1;
        leftb = 0;
        if (xl <= 10){
            vxl = 0;
            xl = 10;
            jumpflagl = 0;
        }
    }
}

void gravityr(void){
    if (jumpflagr == 1){
        vxr = vxr - 1;
        rightb = 0;
        if (xr <= 10){
            vxr = 0;
            xr = 10;
            jumpflagr = 0;
        }
    }
}









void checkcollisionc(int32_t xc, int32_t yc){



    if(xBall <= xc +10){
        if(xBall >= xc ){
            if(yBall <= yc){
                if(yBall >= yc-22){
                    collisionflag = charactercollisionbottom;
                }
            }
        }

    }
    if(xBall <= xc + 40){
        if(xBall >= xc){
            if(yBall >= yc-22){
                if(yBall <= yc-11){
                    collisionflag = charactercollisionleft;
                }
            }
        }

    }
    if(xBall <= xc + 40){
        if(xBall >= xc){
            if(yBall <= yc){
                if(yBall >= yc-10){
                    collisionflag = charactercollisionright;
                }
            }
        }

    }
    if(xBall <= xc + 40){
        if(xBall >= xc +32){
            if(yBall <= yc){
                if(yBall >= yc-22){
                    collisionflag = charactercollisiontop;
                }
            }
        }

    }

}

void checkcollisionwall(void){
    if(xBall <= 15){
        collisionflag = floorcollision;
    }
    if(xBall >= 128){
        collisionflag = wallcollision;
    }
    if(yBall >= 160){
        collisionflag = wallcollision;
    }
    if(yBall <= 0){
        collisionflag = wallcollision;
    }
}
void moveball(void){
    if (ballvy >=3){
        ballvy = 3;
    }
    if (ballvx >= 9){
        ballvx = 9;
    }
    if (ballvy <= -3){
        ballvy = -3;
    }
    if (ballvx <= -9){
        ballvx = -9;
    }
    yBall = yBall+ballvy; // integration
    xBall = xBall+ballvx; // integration
}
void ballvelocity(void){
    if(collisionflag == 3){//wall collision
        ballvx = -1*ballvx;
        ballvy = -1*ballvy;
    }
    if (collisionflag == 4){//floor collision
        ballvx = -1*ballvx +(2 + (1/2));
        ballvy = ballvy;
    }
    if (collisionflag == 1){//character left collision
        ballvx = -1*ballvx;
        ballvy = -1*ballvy;
        if(charactercollisionflag == 2){
            yBall = yr-22;
            if (jumpflagl == 1){
            ballvy = ballvy - 4;
            ballvx = ballvx + 3;
        }
        else{
            ballvy = ballvy -2;
            ballvx = ballvx +1;
        }
       }
        if(charactercollisionflag == 1){
                    yBall = yl-22;
                    if (jumpflagr == 1){
                    ballvy = ballvy - 4;
                    ballvx = ballvx +3;
                }
                else{
                    ballvy = ballvy -2;
                    ballvx = ballvx +1;
                }
                }
    }
    if (collisionflag == 2){//character right collision
        ballvx = -1*ballvx;
        ballvy = -1*ballvy;
        if(charactercollisionflag == 1){
            yBall = yl;
            if (jumpflagl == 1){
            ballvy = ballvy + 4;
            ballvx = ballvx +3;
        }
            else{
            ballvy = ballvy +2;
            ballvx = ballvx +1;
        }
      }
        if(charactercollisionflag == 2){
                    yBall = yr;
                    if (jumpflagr == 1){
                    ballvy = ballvy + 4;
                    ballvx = ballvx +3;
                }
                    else{
                    ballvy = ballvy +2;
                    ballvx = ballvx +1;
                }
              }
    }
    if (collisionflag == 6){//character top collision
        ballvx = ballvx + 1;
        ballvy = -1*ballvy;
        if (charactercollisionflag == 1){
            if(yBall > (yl-11)){
                ballvy=(ballvy+3);
                xBall = xl +41;
            }else{
                ballvy=(ballvy-3);
                xBall = xl +41;
            }

            if (jumpflagl == 1){
            ballvy = ballvy + 4;
            ballvx = ballvx + 2;
        }
        else{
            ballvy = ballvy +2;

        }
       }
        if (charactercollisionflag ==2){
            if(yBall > (yr-11)){
                ballvy=(ballvy+3);
                xBall = xr +41;
            }else{
                ballvy=(ballvy-3);
                xBall = xr +41;
            }

            if (jumpflagr == 1){
                ballvy = ballvy + 4;
            }
            else{
                ballvy = ballvy +2;
            }

        }
    }
    if (collisionflag == 7){//character bottom collision
            ballvx = 0;
            ballvy = -1*ballvy;
            if(charactercollisionflag == 1){
                yBall = yl;
                xBall = xl;
                if (jumpflagl == 1){
                ballvy = ballvy + 4;
            }
            else{
                ballvy = ballvy +2;
            }
           }
            if(charactercollisionflag == 2){
                            yBall = yr - 22;
                            xBall = xr;
                            if (jumpflagr == 1){
                            ballvy = ballvy + 4;
                        }
                        else{
                            ballvy = ballvy +2;
                        }
                       }
        }

    ballvx = ballvx - 1;

}






















void DrawSprite(int32_t xlo, int32_t ylo, int32_t xlonew, int32_t ylonew, const unsigned short *image, uint32_t width, uint32_t length){
    unsigned short tempbufl[width*length];
    uint32_t buflpos=0;
        for(int32_t j=(length-1); j >= 0; j--){
            for(uint32_t i=0; i<width; i++){
                if(ylo%2==0){
                    tempbufl[buflpos]=Background[((160-(ylo-22)+1-j)*128)+xlo+i];
                }else{
                    tempbufl[buflpos]=Background[((160-(ylo-22)-j)*128)+xlo+i];
                }
                buflpos++;
            }
        }

        ST7735_DrawBitmap(xlo,ylo,tempbufl,width,length);

        buflpos=0;
                for(int32_t j=(length-1); j >= 0; j--){
                    for(uint32_t i=0; i<width; i++){
                        if(ylonew%2==0){
                            tempbufl[buflpos]=Background[((160-(ylonew-22)+1-j)*128)+xlonew+i];
                        }else{
                            tempbufl[buflpos]=Background[((160-(ylonew-22)-j)*128)+xlonew+i];
                        }
                        buflpos++;
                    }
                }

        for(uint32_t i=0; i<(width*length); i++){
                if(image[i] != 0x0000){
                    tempbufl[i]=image[i];
                }
            }

        ST7735_DrawBitmap(xlonew,ylonew,tempbufl,width,length);
}

void BlacktoBackground(uint32_t xlo, uint32_t ylo, const unsigned short *image, uint32_t width, uint32_t length){
    unsigned short goaltempbuf[(width*length)];
    uint32_t goalbufpos=0;
            for(int32_t j=(length-1); j >= 0; j--){
                for(uint32_t i=0; i<(width); i++){
                    if(ylo%2==0){
                        goaltempbuf[goalbufpos]=Background[((160-(ylo-22)+1-j)*128)+xlo+i];
                    }else{
                        goaltempbuf[goalbufpos]=Background[((160-(ylo-22)-j)*128)+xlo+i];
                    }
                    goalbufpos++;
                }
            }

    for(uint32_t i=0; i<(width*length); i++){
        if(image[i] > 0x0000){
            goaltempbuf[i]=image[i];
        }
    }

    ST7735_DrawBitmap(xlo,ylo,goaltempbuf,width,length);
}

void Countdown(void){
    BlacktoBackground(46, 92, Letter3, 36, 24);
    Clock_Delay1ms(1000);
    BlacktoBackground(46, 92, Letter2, 36, 24);
    Clock_Delay1ms(1000);
    BlacktoBackground(46, 92, Letter1, 36, 24);
    Clock_Delay1ms(1000);
}

void InitPositions(void){
    xr = 10;
    yr = 130;
    xl = 10;
    yl = 50;
    xBall=10;
    yBall=85;
    ballvy=0;
    ballvx=0;
}

void PrintEngGoal(void){
    BlacktoBackground(46, 44, LetterG, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(46, 68, LetterO, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(46, 92, LetterA, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(46, 116, LetterL, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(46, 140, LetterExc, 36, 24);
    Clock_Delay1ms(3000);
}

void PrintSpnGoal(){
    BlacktoBackground(46, 56, LetterG, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(46, 80, LetterO, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(46, 104, LetterL, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(46, 128, LetterExc, 36, 24);
    Clock_Delay1ms(3000);
}

void PrintOneWonEng(void){
    BlacktoBackground(64, 44, LetterM, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(64, 68, LetterE, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(64, 92, LetterS, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(64, 116, LetterS, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(64, 140, LetterI, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(28, 68, LetterW, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(28, 92, LetterO, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(28, 116, LetterN, 36, 24);
    Clock_Delay1ms(5000);
}

void PrintTwoWonEng(void){
    BlacktoBackground(64, 56, LetterH, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(64, 80, LetterO, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(64, 104, LetterL, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(64, 128, LetterT, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(28, 68, LetterW, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(28, 92, LetterO, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(28, 116, LetterN, 36, 24);
    Clock_Delay1ms(5000);
}

void PrintTieEng(void){
    BlacktoBackground(46, 68, LetterT, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(46, 92, LetterI, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(46, 116, LetterE, 36, 24);
    Clock_Delay1ms(5000);
}

void PrintOneWonSpn(void){
    BlacktoBackground(64, 44, LetterM, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(64, 68, LetterE, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(64, 92, LetterS, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(64, 116, LetterS, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(64, 140, LetterI, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(28, 56, LetterG, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(28, 80, LetterA, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(28, 104, LetterN, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(28, 128, LetterOAcc, 36, 24);
    Clock_Delay1ms(5000);
}

void PrintTwoWonSpn(void){
    BlacktoBackground(64, 56, LetterH, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(64, 80, LetterO, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(64, 104, LetterL, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(64, 128, LetterT, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(28, 56, LetterG, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(28, 80, LetterA, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(28, 104, LetterN, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(28, 128, LetterOAcc, 36, 24);
    Clock_Delay1ms(5000);
}

void PrintTieSpn(void){
    BlacktoBackground(46, 32, LetterE, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(46, 56, LetterM, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(46, 80, LetterP, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(46, 104, LetterA, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(46, 128, LetterT, 36, 24);
    Clock_Delay1ms(800);
    BlacktoBackground(46, 152, LetterE, 36, 24);
    Clock_Delay1ms(5000);
}

void PrintTitle(void){
    BlacktoBackground(87, 56, LetterH, 36, 24);
    BlacktoBackground(87, 80, LetterE, 36, 24);
    BlacktoBackground(87, 104, LetterA, 36, 24);
    BlacktoBackground(87, 128, LetterD, 36, 24);
    BlacktoBackground(51, 32, LetterS, 36, 24);
    BlacktoBackground(51, 56, LetterO, 36, 24);
    BlacktoBackground(51, 80, LetterC, 36, 24);
    BlacktoBackground(51, 104, LetterC, 36, 24);
    BlacktoBackground(51, 128, LetterE, 36, 24);
    BlacktoBackground(51, 152, LetterR, 36, 24);
}

void ClickButtonEng(void){
    ST7735_DrawBitmap(20,59,SmallLetterCcap,10,9);
    ST7735_DrawBitmap(20,63,SmallLetterL,10,4);
    ST7735_DrawBitmap(20,67,SmallLetterI,10,4);
    ST7735_DrawBitmap(20,73,SmallLetterC,10,6);
    ST7735_DrawBitmap(20,80,SmallLetterK,10,7);
    ST7735_DrawBitmap(20,85,SmallLetterSpace,10,5);
    ST7735_DrawBitmap(20,95,SmallLetterAcap,10,10);
    ST7735_DrawBitmap(20,102,SmallLetterN,10,7);
    ST7735_DrawBitmap(20,109,SmallLetterY,10,7);
    ST7735_DrawBitmap(10,44,SmallLetterBcap,10,9);
    ST7735_DrawBitmap(10,51,SmallLetterU,10,7);
    ST7735_DrawBitmap(10,56,SmallLetterT,10,5);
    ST7735_DrawBitmap(10,61,SmallLetterT,10,5);
    ST7735_DrawBitmap(10,67,SmallLetterO,10,6);
    ST7735_DrawBitmap(10,74,SmallLetterN,10,7);
    ST7735_DrawBitmap(10,79,SmallLetterSpace,10,5);
    ST7735_DrawBitmap(10,84,SmallLetterT,10,5);
    ST7735_DrawBitmap(10,90,SmallLetterO,10,6);
    ST7735_DrawBitmap(10,95,SmallLetterSpace,10,5);
    ST7735_DrawBitmap(10,102,SmallLetterScap,10,7);
    ST7735_DrawBitmap(10,107,SmallLetterT,10,5);
    ST7735_DrawBitmap(10,114,SmallLetterA,10,7);
    ST7735_DrawBitmap(10,120,SmallLetterR,10,6);
    ST7735_DrawBitmap(10,125,SmallLetterT,10,5);
}

void ClickButtonSpn(void){
    ST7735_DrawBitmap(30,43,SmallLetterEcap,10,9);
    ST7735_DrawBitmap(30,47,SmallLetterL,10,4);
    ST7735_DrawBitmap(30,51,SmallLetterI,10,4);
    ST7735_DrawBitmap(30,58,SmallLetterG,10,7);
    ST7735_DrawBitmap(30,64,SmallLetterE,10,6);
    ST7735_DrawBitmap(30,69,SmallLetterSpace,10,5);
    ST7735_DrawBitmap(30,78,SmallLetterCcap,10,9);
    ST7735_DrawBitmap(30,85,SmallLetterU,10,7);
    ST7735_DrawBitmap(30,92,SmallLetterA,10,7);
    ST7735_DrawBitmap(30,96,SmallLetterL,10,4);
    ST7735_DrawBitmap(30,103,SmallLetterQ,10,7);
    ST7735_DrawBitmap(30,110,SmallLetterU,10,7);
    ST7735_DrawBitmap(30,114,SmallLetterI,10,4);
    ST7735_DrawBitmap(30,120,SmallLetterE,10,6);
    ST7735_DrawBitmap(30,126,SmallLetterR,10,6);
    ST7735_DrawBitmap(20,56,SmallLetterBcap,10,9);
    ST7735_DrawBitmap(20,62,SmallLetterO,10,6);
    ST7735_DrawBitmap(20,67,SmallLetterT,10,5);
    ST7735_DrawBitmap(20,73,SmallLetterOAcc,10,6);
    ST7735_DrawBitmap(20,80,SmallLetterN,10,7);
    ST7735_DrawBitmap(20,85,SmallLetterSpace,10,5);
    ST7735_DrawBitmap(20,92,SmallLetterP,10,7);
    ST7735_DrawBitmap(20,99,SmallLetterA,10,7);
    ST7735_DrawBitmap(20,105,SmallLetterR,10,6);
    ST7735_DrawBitmap(20,112,SmallLetterA,10,7);
    ST7735_DrawBitmap(10,60,SmallLetterCcap,10,9);
    ST7735_DrawBitmap(10,66,SmallLetterO,10,6);
    ST7735_DrawBitmap(10,77,SmallLetterM,10,11);
    ST7735_DrawBitmap(10,83,SmallLetterE,10,6);
    ST7735_DrawBitmap(10,90,SmallLetterN,10,7);
    ST7735_DrawBitmap(10,96,SmallLetterZ,10,6);
    ST7735_DrawBitmap(10,103,SmallLetterA,10,7);
    ST7735_DrawBitmap(10,109,SmallLetterR,10,6);
}
