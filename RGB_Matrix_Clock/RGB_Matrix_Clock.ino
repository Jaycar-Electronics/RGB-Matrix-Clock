//RGB matrix as digital clock
//overlapped hour/minute digits distinguished by different colours
//flashing second dot, like an analog second hand
//time kept in RTC

byte nums[10][3]={                   //bitmap of numbers
  {31,17,31},
  {0,0,31},
  {29,21,23},
  {21,21,31},
  {7,4,31},
  {23,21,29},
  {31,21,29},
  {1,1,31},
  {31,21,31},
  {23,21,31}
};



//define colours of parts of displays r,g,b format
#define HOURS 0,0,255
#define MINUTES 0,255,0
#define SECONDS 255,255,255

#include <Colorduino.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;

void setup() {
  Colorduino.Init(); // initialize the board
  unsigned char whiteBalVal[3] = {36,63,63};  //adjust these values for white balance
  Colorduino.SetWhiteBal(whiteBalVal);
  rtc.begin();
}

void loop() {
  DateTime now = rtc.now();
  //int h=(millis()/3600000L)%24;   //get the time
  //int m=(millis()/60000L)%60;
  //int s=(millis()/1000)%60;
  int h=now.hour();
  int m=now.minute();
  int s=now.second();
  
  for(char x=0;x<8;x++){          //clear the display buffer
    for(char y=0;y<8;y++){
       Colorduino.SetPixel(x,y,0,0,0);
    }
  }
  if(h>9){
    drawnum(h/10,0,0,HOURS);      //if not blank, draw 10's of hours
  }
  drawnum(h%10,0,4,HOURS);        //draw units of hours
  drawnum(m/10,3,1,MINUTES);      //draw tens of minutes
  drawnum(m%10,3,5,MINUTES);      //draw units of minutes
  if((millis()/500)&1){seconddot(s);}   //second dot, flashing at 1Hz
  Colorduino.FlipPage();          //draw buffer out
  delay(100);                     //wait a bit to give everything a break
}

void drawnum(byte n, byte x, byte y, byte r, byte g, byte b){
  //draw number n starting at x,y with colour rgb
  byte u,v;
  for(u=0;u<5;u++){
    for(v=0;v<3;v++){
      if(nums[n%10][v]&(1<<u)){
        Colorduino.SetPixel(x+u,y+v,r,g,b);
      }
    }
  }  
}

void seconddot(byte s){
  byte x,y;
  //draws a second dot (as though it were a second hand on a clockface)
  //0==> middle top moving clockwise
  s=(s+8)%60;       //clip, zero now means top left
  switch(s){
    case 0 ... 14: y=s/2;x=0;break;         //on the top line
    case 15 ... 29: y=7;x=(s-15)/2;break;   //down the right side
    case 30 ... 44: y=(45-s)/2;x=7;break;   //along the bottom
    case 45 ... 59: y=0;x=(60-s)/2;break;   //left edge
  }
  Colorduino.SetPixel(x,y,SECONDS);         //draw the dot
}

