#include <EnableInterrupt.h>
#include <Rotary.h>
#include <TM1637Display.h>


TM1637Display display(10,11);  //clk and dio 

Rotary r = Rotary(8,9); //rotary on these pins
 volatile int counter;   //this will be on time for relay
 volatile int btpress;  //just a button state integer
 

void setup() {
  Serial.begin(9600);

  /* Original interrupt setup:
  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
  sei();
  */
  pinMode(6,INPUT_PULLUP);
  display.setBrightness(6);  
 counter=50;   //start value for on time of relay,you could aditionaly restrict it but i cant bother
  enableInterrupt(8, interruptFunction, CHANGE);   //interrupts for rotary
  enableInterrupt(9, interruptFunction, CHANGE);
  enableInterrupt(12, btnpr,FALLING);   //interrupt for button
   pinMode(13,OUTPUT);               //relay pin
}

void loop() {
display.setBrightness(0x0f);         //max brightness
display.showNumberDec(counter,false);   //display setup withouth decimal point


if (btpress==1) 
           {digitalWrite(13,LOW);delay(counter);digitalWrite(13,HIGH);btpress=0;} //this turns on the relay for set time
}

void interruptFunction() {        //main interrupt for rotary pins

  

  unsigned char result = r.process();
  if (result == DIR_NONE) {
    // do nothing
  }
  else if (result == DIR_CW) {
    //Serial.println("ClockWise");
    counter=counter+5;
    Serial.println(counter);
  }
  else if (result == DIR_CCW) {
    //Serial.println("CounterClockWise");
    counter=counter-5;
    Serial.println(counter);
  }
}

void btnpr(){                 //this sets btnstate to high when it detects a high 
static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (interrupt_time - last_interrupt_time > 200) 
 {btpress=1;Serial.println('BTN');}
 else {btpress=0;}
 last_interrupt_time = interrupt_time;                              
  
  }
