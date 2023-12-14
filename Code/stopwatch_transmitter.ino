#include <SoftwareSerial.h>

#define trigPin 9
#define echoPin 8
#define Button 3
#define LED 2

long duration;
int distance;
bool ready = false;

SoftwareSerial HC12(1, 0); // TX, RX

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(Button, INPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED, OUTPUT);

  HC12.begin(9600);
}

void loop() {
  if (digitalRead(Button) == LOW){
    digitalWrite(LED, HIGH); // turn on ready LED
    ready = true;
    delay(2000); //safety delay
  }
  
  while(ready) // ready button clicked
  {
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = 0;
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(5);  
    digitalWrite(trigPin, LOW);
    duration += pulseIn(echoPin, HIGH);
    distance = (duration * 0.034 / 2);

    if (distance < 50) // motion sensor tripped
    {
      HC12.print('1');
      delay(20);
      digitalWrite(LED, LOW);
      ready = false;
    }
  }
}