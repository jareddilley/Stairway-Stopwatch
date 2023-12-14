#include <LiquidCrystal_I2C.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SoftwareSerial.h>
#include <SPI.h>

#include "Font_Data.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 3

#define SPEED_TIME  75
#define PAUSE_TIME  0
#define MAX_MESG  20

int brightness = 1;

const int trigPin = 8;
const int echoPin = 9;
long duration = 0;
int distance = 100;
     
int count = 0;
int m=0;     
int s=0;     
int ms=0;   
int total = 0;
int best = 0;
const int startPin = 5;    
const  int stopPin = 4;    
const int resetPin = 2; 
char szMinSec[9];    // mm:ss\0
char szMillisec[4];  // ms
char bestTime[14];   // mm:ss:ms
bool f = true;       // blinker for :
bool manual = false;

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

LiquidCrystal_I2C lcd(0x27,16,2);
SoftwareSerial HC12(0, 1); // TX, RX

/*
Updates the LED matix display
*/
void updateDisplay()
{
  sprintf(szMinSec, "%02d%c%02d", m, (f ? ':' : ' '), s);
  sprintf(szMillisec, "%02d", ms); 
  sprintf(bestTime, "%02d:%02d:%02d", m,s,ms);
  myDisplay.displayAnimate();
  myDisplay.displayReset(0);
  myDisplay.displayReset(1); 

  return;
}

/*
Updates the TIME TO BEAT display
*/
void updateBestTime()
{ 
  lcd.clear();   
  lcd.setCursor(2,0);  
  lcd.print("TIME TO BEAT"); 
  lcd.setCursor(4,1); 
  lcd.print(bestTime);

  return;
}

/*
Main Stopwatch increment loop
*/
void stopwatch()
{
  ms++;
  total++;

  if(ms==100)   // sec increment       
  {
    ms=0,s++; 
  }
  if(ms%50 == 0)   // : ticker      
  {
    f=!f;
  }
  if(s==60)     // min increment
  { 
    s=0,m++; 
  }

  updateDisplay();

  // utrasonic sensor measurement 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);  
  digitalWrite(trigPin, LOW);
  duration += pulseIn(echoPin, HIGH);
  count++;
  distance = 100;

  if(count == 4){ // moving average
    distance = (duration * 0.034 / (2 * 4));
    count = 0;
    duration = 0;
  }

  if((digitalRead(stopPin) == LOW) || (distance < 50 && !manual))    // stop button or motion sensor tripped
  {
    f=true;
    if((total < best || best==0 ) && total > 60)
    {
      best = total;
      updateBestTime();
    }
    manual = false;
    stopwatch_stop();
  } 
  else if (digitalRead(resetPin) == LOW)  // reset button
  {
    stopwatch_reset();
    loop();
  }
  else{
    stopwatch(); 
  }
}

/*
Stopwatch stopped loop
*/
void stopwatch_stop()
{
  updateDisplay();
  
  
  if (digitalRead(startPin) == LOW) // start button
  {
    manual = true;
    stopwatch(); 
  } 
  if(digitalRead(resetPin) == LOW)  // reset button
  {
    stopwatch_reset();  
    loop();
  } 
  if(HC12.available()){ // start rf signal recieved 
    int input = HC12.read();
    if(input == '1'){ 
      stopwatch_reset();
      stopwatch();
    }
  }
  stopwatch_stop();
}

/*
Reset stopwatch and displays
*/
void stopwatch_reset()
{
  m=0;
  s=0;
  ms=0;
  total = 0;
  count = 0;
  duration = 0;
  distance = 100;
  manual = false;
  f = true;
  
  updateDisplay();

  return;
}

void setup() {
  myDisplay.begin(3);

  // Set up the two zones for LEd matrix
  myDisplay.setZone(0, 0, 0);   // Zone 0 - ms
  myDisplay.setZone(1, 1, 3);   // Zone 1 - min : sec
  myDisplay.setFont(0, numeric7Seg);  // smaller font for ms
  myDisplay.setFont(1, numeric7Se);
  myDisplay.displayZoneText(0, szMillisec, PA_LEFT, SPEED_TIME, 0, PA_PRINT, PA_NO_EFFECT);
  myDisplay.displayZoneText(1, szMinSec, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
  
  // Change brightness (0-15)
  myDisplay.setIntensity(brightness);

  sprintf(szMinSec, "%02d%c%02d", m, (f ? ':' : ' '), s);
  sprintf(szMillisec, "%02d", ms);
  sprintf(bestTime, "%02d:%02d:%02d", m,s,ms);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  pinMode(startPin, INPUT);
  pinMode(stopPin, INPUT);
  pinMode(resetPin, INPUT);

  // Init TIME TO BEAT display
  lcd.init();
  lcd.backlight(); 
  updateBestTime();

  HC12.begin(9600);
}

void loop() {
  myDisplay.displayAnimate();
  myDisplay.displayReset(0);
  myDisplay.displayReset(1);
  
  if(HC12.available()){ // start rf signal recieved 
    int input = HC12.read();
    if(input == '1'){ 
      stopwatch();
    } 
  } 
  if(digitalRead(startPin) == LOW){  // start button
    manual = true;
    stopwatch();
  }

  //HC12.flush();
  //delay(20);
}