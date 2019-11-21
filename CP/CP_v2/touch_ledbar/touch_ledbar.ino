#include "touch.h"
#include "spread.h"
#include "modeSet.h"
#include "Simbol.h"


void setup()
{
  randomSeed(analogRead(6));
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(150);
  strip.show();
  resetColor();
  setPosMat();
  setdisMat();
  AA.cc.setRGB(255, 148, 255);
  //BB.cc.setRGB(148,255,255);
  //CC.cc.setRGB(255,255,148);
  
  MsTimer2::set(ShowDelay,rShow);
  MsTimer2::start();
}

void loop()
{
  unsigned long cmil = millis(); 
  if(cmil - Tmil>=TouchDelay){
    Tmil = cmil;
    if(!menu)isPushedMenu();
    detectLocation();
    viewMenu(0);
    settingFnc();
    //AA.draw();
    //BB.draw();
    //CC.draw();
  }
    
  if(!isPushed) {
    pushedTime=0;
    setNextColor();
    viewMenu(1);
    ms.resetGauge();
    screenSaverTimer++;
    if(screenSaverTimer>200) {
      screenSaverTimer=210; 
      AA.draw();
    }
  }
  else {
    screenSaverTimer=1;
    pushedTime = constrain(pushedTime++,0,radLim);
    spread(pushedTime);
  }
  dimm++;
}

void viewMenu(int i) {
  if(menu&&(i?true:ypos<18)) timer++;
  else if(menu&&ypos>17) timer=1;
  if(timer==0){ menu=0;timer++;ms.onState();}
}


  
