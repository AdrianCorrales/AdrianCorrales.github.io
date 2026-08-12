// Lab9Main.c
// Runs on MSPM0G3507
// Lab 9 ECE319K
// Ryan Kehtar-Navaz & Adrian Corrales
// Last Modified: 12/31/2023

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/ADC1.h"
#include "../inc/DAC5.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images/images.h"
#include "Arrays.h"
#include "Functions.h"

// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

uint32_t M=1;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}



int32_t rightb = 0;
int32_t leftb = 0;
int32_t rightsp = 0;
int32_t leftsp = 0;
int32_t vxr;
int32_t vxl;
int32_t vyr;
int32_t vyl;
int32_t oldxr=10;
int32_t oldyr=130;
int32_t oldxl=10;
int32_t oldyl=50;
int32_t xBall=85, yBall=85, oldxBall=85, oldyBall=85;
int32_t coverxr, coveryr, coverxl, coveryl;
int32_t xr = 10;
int32_t yr = 130;
int32_t xl = 10;
int32_t yl = 50;
uint8_t positionflag = 0, BackgroundFlagL=0, BackgroundFlagR=0, BackgroundFlagBall=0;
uint8_t jumpflagl = 0;
uint8_t jumpflagr = 0;
uint8_t lvlflag = 0, leftchanged=0, rightchanged=0;
uint8_t languageflag = 0;
uint32_t temp, LeftScore=0, RightScore=0;
int32_t timer=121;
#define KICKCOUNT 2304
#define GOALCOUNT 6735

#define floorcollision 4;
#define wallcollision 3;
#define charactercollisionright 2;
#define charactercollisionleft 1;
#define goalcollision 5;
#define charactercollisiontop 6;
int32_t ballvx;
int32_t ballvy;
uint8_t collisionflag = 0;
uint8_t charactercollisionflag=0;

uint8_t timerfortimer=30, timerflag=0;

const char *EnglishStart = "Click Any Button to Start";
const char *SpanishStart = "Elige Cualquier Bot\u00F3n para Comenzar";

// games  engine runs at 30Hz
void TIMG12_IRQHandler(void){uint32_t pos,msg;
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
// game engine goes here
    // 1) sample slide pot

    timerfortimer--;
    if(timerfortimer==0){
        timerfortimer=30;
        timerflag=1;
    }

    rightsp = ADC1in();
    leftsp = ADC0in();
    vyr = LateralVelocity(rightsp);
    vyl = LateralVelocity(leftsp);
    Switch_In(); // 2) read input switches
    // 2) read input switches
    // 3) move sprites

    if(BackgroundFlagL){
        oldyl = yl;
        oldxl = xl;
        BackgroundFlagL=0;
    }
    if(BackgroundFlagR){
        oldyr = yr;
        oldxr = xr;
        BackgroundFlagR=0;
    }
    if(BackgroundFlagBall){
        oldxBall = xBall;
        oldyBall = yBall;
        BackgroundFlagBall=0;
    }
    MoveLeft();
    MoveRight();

    checkcharactercollision();
    collisionflag=0;
    checkcollisionc(xr, yr);
        if(charactercollisionflag != 1){
            if(collisionflag>0){
                charactercollisionflag=2;
            }
        }

    checkcollisionc(xl, yl);
        if (charactercollisionflag !=2){
            if(collisionflag>0){
                charactercollisionflag=1;
            }
        }

    gravityl();
    gravityr();
    JumpLeft();
    JumpRight();

    checkcollisionwall();
    ballvelocity();
    moveball();
    charactercollisionflag = 0;
    positionflag=1;

    // 4) start sounds
    // 5) set semaphore
    // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }
}

uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}



// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
 int main(void){ // final main
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ADC1init();     //PB18 = ADC1 channel 5, slidepot
  ADC0init();     //PB18 = ADC1 channel 5, slidepot
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(80000000/30,2);
  // initialize all data structures
  __enable_irq();

  while(1){
      TimerG12_IntArm(80000000/30,2);
      ST7735_FillScreen(0x0000);
      ST7735_SetCursor(1, 1);
      ST7735_OutString("Click Left for");
      ST7735_SetCursor(1, 2);
      ST7735_OutString("English");
      ST7735_SetCursor(1, 4);
      ST7735_OutString("Click Right for");
      ST7735_SetCursor(1, 5);
      ST7735_OutString("Spanish");

      while(lvlflag == 0){
          if(leftb == 1){
              languageflag = 1;
              lvlflag = 2;
          }
          if(rightb == 1){
              languageflag = 2;
              lvlflag = 2;
          }
      }

      ST7735_DrawBitmap(0,160,Background,128,160);
      PrintTitle();
      if(languageflag==1){
          ClickButtonEng();
      }else{
          ClickButtonSpn();
      }

      while(lvlflag==2){
          if((leftb==1)||(rightb==1)){
              lvlflag=1;
          }
      }

      timer=121;
      LeftScore=0;
      RightScore=0;
      ST7735_DrawBitmap(0,160,Background,128,160);
      Countdown();
      ST7735_DrawBitmap(0,160,Background,128,160);
      InitPositions();

      while(lvlflag == 1){
          SmallFont_OutHorizontal(LeftScore, 122, 30);
          SmallFont_OutHorizontal(RightScore, 122, 120);
          if(timerflag==1){
              timerflag=0;
              timer--;
              SmallFont_OutHorizontal(timer, 122, 75);
          }

          while(positionflag == 0){}// wait for semaphore
          positionflag = 0;

          DrawSprite(oldxl, oldyl, xl, yl, Messi, 40,22);
          BackgroundFlagL=1;

          DrawSprite(oldxr, oldyr, xr, yr, Holt, 40,22);
          BackgroundFlagR=1;

          DrawSprite(oldxBall, oldyBall, xBall, yBall, Ball, 10, 10);
          BackgroundFlagBall=1;

          if(((yBall<=20)&&(xBall >= 10)&&(xBall <= 80))||((yBall>=150)&&(xBall >= 10)&&(xBall <= 80))){
              TimerG12_IntArm(0,2);
              Sound_Start(Goal, GOALCOUNT);
              ST7735_DrawBitmap(0,160,Background,128,160);

            if(((yBall<=20)&&(xBall >= 10)&&(xBall <= 80))){
              RightScore++;
            }else{
              LeftScore++;
            }

            if(languageflag==1){
                PrintEngGoal();
            }else{
                PrintSpnGoal();
            }

                ST7735_DrawBitmap(0,160,Background,128,160);
                InitPositions();
                SmallFont_OutHorizontal(timer, 122, 75);
                TimerG12_IntArm(80000000/30,2);

            }




if((collisionflag == 1)||(collisionflag ==2)||(collisionflag == 6)){
     Sound_Start(Kick, KICKCOUNT);
}





    if(timer <= 0){
          TimerG12_IntArm(0,2);
        ST7735_DrawBitmap(0,160,Background,128,160);
        Sound_Start(Goal, GOALCOUNT);
        if(LeftScore>RightScore){
            if(languageflag==1){
                PrintOneWonEng();
            }else{
                PrintOneWonSpn();
            }
        }
        if(RightScore>LeftScore){
            if(languageflag==1){
                PrintTwoWonEng();
            }else{
                PrintTwoWonSpn();
            }
        }
        if(RightScore==LeftScore){
            if(languageflag==1){
                PrintTieEng();
            }else{
                PrintTieSpn();
            }
        }
        lvlflag=0;
    }

     // clear semaphore
     // update ST7735R
     // check for end game or level switch
        }
     }
  }





















typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};
// use main1 to observe special characters
int main1(void){ // main1
    ADC0init();
    ADC1init();
    char l;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(0x0000);            // set screen to black
  for(phrase_t myPhrase=HELLO; myPhrase<= GOODBYE; myPhrase++){
    for(Language_t myL=English; myL<= French; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Clock_Delay1ms(3000);
  ST7735_FillScreen(0x0000);       // set screen to black
  l = 128;
  while(1){
    Clock_Delay1ms(2000);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }
}

// use main2 to observe graphics
int main2(void){ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
//  ST7735_FillScreen(ST7735_BLACK);
//  ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
//  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
//  ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
//  ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
//  ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom
//  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
//  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
//  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
//  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
//  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);

//  ST7735_DrawBitmap(0,160,Background,128,160);
//  BlacktoBackground(32,44,LetterG,75,24);
//  Clock_Delay1ms(800);
//  BlacktoBackground(32,68,LetterO,75,24);
//  Clock_Delay1ms(800);
//  BlacktoBackground(32,92,LetterA,75,24);
//  Clock_Delay1ms(800);
//  BlacktoBackground(32,116,LetterL,75,24);
//  Clock_Delay1ms(800);
//  BlacktoBackground(32,140,LetterExc,75,24);
//  Clock_Delay1ms(800);
//  ST7735_DrawBitmap(30,30,Messi,40,22);
//DrawSprite(30,30, 40, 40, Messi, 40, 22);


  for(uint32_t t=500;t>0;t=t-5){
    SmallFont_OutVertical(t,104,6); // top left
    Clock_Delay1ms(5000);              // delay 50 msec
  }
  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  ST7735_OutUDec(1234);
  while(1){
  }
}

// use main3 to test switches and LEDs
int main3(void){ // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  while(1){
    // write code to test switches and LEDs
    
  }
}
// use main4 to test sound outputs
int main4(void){ uint32_t last=0,now;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(ADC0,6,0); // ADC1 channel 6 is PB20, TExaS scope
  __enable_irq();
  while(1){
    Switch_In();
    now = rightb; // one of your buttons
    if((last == 0)&&(now == 1)){
      Sound_Start(Kick, KICKCOUNT); // call one of your sounds
    }
//    if((last == 0)&&(now == 2)){
//      Sound_Killed(); // call one of your sounds
//    }
//    if((last == 0)&&(now == 4)){
//      Sound_Explosion(); // call one of your sounds
//    }
//    if((last == 0)&&(now == 8)){
//      Sound_Fastinvader1(); // call one of your sounds
//    }
    // modify this to test all your sounds
  }
}





