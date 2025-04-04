#include "Arduino.h"



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, world!");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello, world!");
  printf("Hello, world!Printf\n");
  delay(1000);
}