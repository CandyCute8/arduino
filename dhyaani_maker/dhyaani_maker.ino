// coded by yasin Dhyaani Shah (candycute8)

/*
----------------------------------
7 segment pins
---------------------------------- 
pin 10 is connected to the DataIn 
pin 12 is connected to the CLK 
pin 11 is connected to LOAD 
*/

#define LM35_PIN A3

#include <Wire.h>
#include <LedControl.h>
#include <LiquidTWI2.h>
#include "pitches.h"

LiquidTWI2 lcd(0x20);

LedControl lc=LedControl(10,12,11,1);

int temp = 0;
int temp_f = 0;
int temp_first_digit = 0;
int temp_second_digit = 0;
const int button_pin = 9; 
const int temp_pin = 8;
int buttonState = 0;
const int led_pin =  13;  
int tempState = 0;

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int note_durations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup() {
  pinMode(button_pin, INPUT);
  pinMode(temp_pin, INPUT);
  // set the LCD type
  lcd.setMCPType(LTI_TYPE_MCP23008); 
  
  // set up the LCD's number of rows and columns:
  lcd.begin(16, 2);
  Serial.begin(9600);

  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
  
  delay(300);
}

void convertTwoDigits(int number, int *first, int *second ) {
  if(number>=0 && number<10){
    *first = 0;
    *second = number;
  }else{
    *first = number / 10;
    *second = number % 10;
  }
}

void loop() {
  buttonState = digitalRead(button_pin);
  if (buttonState == LOW) {
    // turn LED on:
    digitalWrite(led_pin, HIGH);
    buzz();
  }
  
  // turn on LCD backlight
  lcd.setBacklight(HIGH);
 
  // what will write first line
  lcd.print("  DHYAANI SHAH  ");
 
  lcd.setCursor(0, 1);
 
  // what will write second line
  lcd.print("   I AM MAKER   ");

  temp = (5.0 * analogRead(LM35_PIN) * 100.0) / 1024;
  
  
  tempState = digitalRead(temp_pin);

  if(tempState == LOW) {
    lc.clearDisplay(0);
    convertTwoDigits(temp,&temp_first_digit,&temp_second_digit);
    lc.setDigit(0,0,temp_first_digit,false);
    lc.setDigit(0,1,temp_second_digit,false);
    lc.setRow(0,2,B01100011);
    lc.setRow(0,3,B01001110);
  } else {
    // temperature as fahrenheit
    temp_f = ((temp * 9) / 5) + 32;
    lc.clearDisplay(0);
    convertTwoDigits(temp_f,&temp_first_digit,&temp_second_digit);
    lc.setDigit(0,0,temp_first_digit,false);
    lc.setDigit(0,1,temp_second_digit,false);
    lc.setRow(0,2,B01100011);
    lc.setRow(0,3,B01000111);
  }

  delay(1000);
}

void buzz() {
  for (int this_note = 0; this_note < 8; this_note++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int note_duration = 1000 / note_durations[this_note];
    tone(A1, melody[this_note], note_duration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pause_between_notes = note_duration * 1.30;
    delay(pause_between_notes);
    // stop the tone playing:
    noTone(A1);
  }
}
  
  
