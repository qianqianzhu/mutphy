
/* -------------------------------------
                 header
 --------------------------------------*/

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class BreathLED{
public:
    BreathLED(void);
    void on(void);
    void off(void);
		void resetLedState(void);
    int on_with_brightness(int brightness);
    void start_breath(void);
    void stop_breath(void);
    boolean is_button_clicked(void);
		void attachInterruptHandler(void (*ISR)(void));
		void removeInterruptHandler(void);
		volatile int ledState;
private:
    const int _pbutton=3;
    const int _pled=9;
    unsigned long lastDebounceTime;
    unsigned long deBounceDelay;
		int lastButtonState;
};


/* -------------------------------------
                 implementation
 --------------------------------------*/

BreathLED::BreathLED(void){
  ledState=LOW;
  lastButtonState=LOW;
  lastDebounceTime=0;
  deBounceDelay=50;
  pinMode(3, INPUT_PULLUP);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  Serial.begin(9600);
}

void BreathLED::resetLedState(void){
  digitalWrite(9, LOW);
	ledState=LOW;
}

void BreathLED::on(void){
  digitalWrite(9, HIGH);
}

void BreathLED::off(void){
  digitalWrite(9, LOW);
}

int BreathLED::on_with_brightness(int brightness){
  if(brightness>=0 && brightness<=255){
    analogWrite(9, brightness);
  }else{
    Serial.println("please enter an integer in [0,255]!");
    brightness=-1;
  }
  return brightness;
}

boolean BreathLED::is_button_clicked(void){
  boolean isClicked=false;
  int buttonState;
  int reading=digitalRead(3);
  if(reading!=lastButtonState){
    lastDebounceTime = millis();
  }
  delay(deBounceDelay);
  if(reading!=lastButtonState){
      buttonState=reading;
  }
  // only toggle the LED if the new button state is HIGH
  if(buttonState == HIGH) {
    isClicked = true;
    Serial.println("button is clicked");
  }

  lastButtonState = reading;
  return isClicked;
}

void BreathLED::start_breath(void){
  int brightness; 
  //Serial.println("start breathing");
  // set the brightness of pin 9:
  for(brightness=0;brightness<=255;brightness = brightness+5){
    analogWrite(9, brightness);
    delay(30);
  }

  for(;brightness>=0;brightness = brightness-5){
    analogWrite(9, brightness);
    delay(30);
  }
  //Serial.println("finish one loop");
}

void BreathLED::stop_breath(void){
  digitalWrite(9, LOW);
  //Serial.println("stop breathing");
}

void BreathLED::attachInterruptHandler(void (*ISR)(void)){
		attachInterrupt(digitalPinToInterrupt(3),ISR,RISING);
}

void BreathLED::removeInterruptHandler(void){
		detachInterrupt(digitalPinToInterrupt(3));
}
