/*
  Name: Gravistimulation device

  Purpose: Control the rotation of a servo to 170 degrees after a countdown of at least 30 minutes, up to 24 hours maximum.

  The circuit:
  one or two servos connected to D9
  an LCD screen connected to D4, D5, D6, D7, D11 and D12
  an LED connected to D3 with a 220Ω resistor
  a pushbutton connected to D2
  two potentiometers (one for the LCD screen backlight, the other for setting the countdown)
  two capacitors

  Created 14 March 2017
  @author Marion Louveaux
  Modified 25 September 2017
  By Marion Louveaux

  @version 1.0 25/09/2017

  References:
  This programm is based on:
    Project n°5 "Mood cue" from Arduino starter kit book (servo)
    Project n°11 "Crystal Ball" from Arduino starter kit book (LCD screen)
    https://openclassrooms.com/courses/programmez-vos-premiers-montages-avec-arduino/le-bouton-poussoir (button/light switch)
    http://colin.pitrat.free.fr/?p=393 (countdown system)

*/


#include <Servo.h>
#include <LiquidCrystal.h>

Servo myServo;

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

int const potPin = A0;
int potVal;
int angle;
double duration;
unsigned long dur_mill = 0;


// Countdown variables
#define SECONDS_IN_HOUR 3600.0 // N.B.: Replace 3600.0 by 30.0 to debug the code/test the setup (1mn will last in 1 second)
unsigned long end_millis = 0; // Time in milliseconds at which countdown stops
double remainingSeconds;
double RMinutes;
double remainingMinutes;
double RHours;
double remainingHours;

// Button variables
int pinButton, pinLed;
boolean lightingStatus;

void setup() {
  myServo.attach(9);

  Serial.begin(9600);

  // Initialisation of the button and LED variables
  pinButton = 2;
  pinLed = 3;
  lightingStatus = 0;
  angle = 0;

  // Definition of modes for the button and the LED
  pinMode(pinButton, INPUT_PULLUP);
  pinMode(pinLed, OUTPUT);


  // Servo reset
  myServo.write(angle);

  // Start display
  lcd.begin(16, 2);
  lcd.print("Hello");
  lcd.setCursor(0, 1);
  lcd.print("AG Maizel :)");
  delay(3000);
  lcd.clear();
  lcd.print("Set time:");

} // end of void setup

void loop() {


  // Read and store button state
  boolean pinButtonStatus = digitalRead(pinButton);
  Serial.println("pinButtonStatus: ");
  Serial.println(pinButtonStatus);

  // Conditional test

  if (!pinButtonStatus) // if the button is pushed (in this case pin is set to 0 -INPUT_PULLUP mode-)
  {
    if (lightingStatus) // if lightingStatus equal to 1
    {
      lightingStatus = 0; // then lightingStatus is set to 0
    }
    else
    {
      lightingStatus = 1; // otherwise lightingStatus is set to 1
    }
  }


  Serial.print("Light: ");
  Serial.print(lightingStatus);

  Serial.print("end_millis: ");
  Serial.print(end_millis);

  Serial.print("millis: ");
  Serial.print(millis());


  if (lightingStatus) // test if lightingStatus is set to 1
  {
    digitalWrite(pinLed, HIGH); // LED is turned on

    // Display is refreshed and modified
    lcd.clear();
    lcd.print("Time left:");


    if (millis() >= end_millis)
    {
      angle = 170; // above 170 degrees, servo is noisy
      duration = 0;
      myServo.write(angle);
      lcd.setCursor(0, 1);
      lcd.print("Done!");
    } else {

      RHours = (end_millis - millis()) / (1000.0 * SECONDS_IN_HOUR);
      remainingHours = floor(RHours);
      RMinutes = (RHours - remainingHours) * 60;
      remainingMinutes = floor(RMinutes);
      remainingSeconds = floor( (RMinutes - remainingMinutes) * 60 );


      lcd.clear();
      lcd.print("Time left:");
      lcd.setCursor(0, 1);
      lcd.print(static_cast<unsigned long>(remainingHours));
      lcd.print("h ");
      lcd.print(static_cast<unsigned long>(remainingMinutes));
      lcd.print("mn ");
      lcd.print(static_cast<unsigned long>(remainingSeconds));
      lcd.print("s");



      Serial.print(", RHours: ");
      Serial.print(RHours);

      Serial.print(", remainingHours: ");
      Serial.print(remainingHours);

      Serial.print(", RMinutes: ");
      Serial.print(RMinutes);

      Serial.print(", remainingMinutes: ");
      Serial.print(remainingMinutes);

      Serial.print(", remainingSeconds: ");
      Serial.print(remainingSeconds);

    }

    delay(200);

  }
  else
  {
    digitalWrite(pinLed, LOW); // LED is turned off
    angle = 0;
    myServo.write(angle);

    // Programming duration before gravistimulation
    dur_mill = map(potVal, 0, 1023, 1, 48); // from 30 minutes to 24h, with 30 minutes increment
    duration = dur_mill / 2.00;

    Serial.print(", dur_mill: ");
    Serial.print(dur_mill);


    Serial.print(", duration: ");
    Serial.print(duration);

    potVal = analogRead(potPin);
    Serial.print("potVal: ");
    Serial.print(potVal);

    end_millis = millis() + duration * SECONDS_IN_HOUR * 1000;

    lcd.clear();
    lcd.print("Set time (hours)");
    lcd.setCursor(0, 1);
    lcd.print(duration);


    delay(200);
  }




} // end of void loop
