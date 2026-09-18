
#include <Arduino.h>
#include <SPI.h>
#define  LEDpin 2

/* Globals */
uint16_t analog_val;
uint8_t lower_val, upper_val;
int y;
int time=0;

void setup() {
  Serial.begin(9600); // opens and configures the USB serial port for baudrate 115200
  /* This does some magic which is necessary on the Nano Every */
  SPI.begin();
  /* Enable SPI, put it in Slave mode, set MSB first */
  SPI0.CTRLA = (SPI_DORD_bm & (SPI_ENABLE_bm & (~SPI_MASTER_bm)));  
  /* disable "Slave Select Disable" --> to ensure it will stay slave */
  SPI0.CTRLB &= ~(SPI_SSD_bm); 
  /* Set Mode 0 (is default though) */
  SPI0.CTRLB |= (SPI_MODE_0_gc); 
  SPI0.INTCTRL = SPI_IE_bm;        /* SPI Interrupt enable */
  

  pinMode(MISO, OUTPUT);
  pinMode(SCK, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(SS, INPUT); 
}

void loop() {
  analog_val = analogRead(A1); 
  //analog_val =784;
  lower_val = analog_val;
  upper_val = (analog_val >> 8) ;             
  digitalWrite(LEDpin, HIGH);
  delay(time);
  digitalWrite(LEDpin, LOW);
  delay(time);                  
  delay(80);

}

// SPI interrupt routine
ISR(SPI0_INT_vect) {
  byte c = SPI0.DATA;

  switch(c){
  case(0x83):
    {
      time = 1000;
      break;
    } 

  case(0x82):
     {
       time = 750;
       break;
     }   


    case(0x81):
     {
       time = 500;
       break;
     }

     case(0x80):
     {
       time = 250;
       //Serial.println("pin always high");
       break;
     }
     case(0x0):
     {
      SPI0.DATA = lower_val;
      //Serial.println("lower val");
      break;
     }
     case(0x1):
     {
      SPI0.DATA = (analog_val >> 8);
      //Serial.println("upper_val");
     } 
     break;
  }

  SPI0.INTFLAGS = SPI_IF_bm; /* Clear the Interrupt flag by writing 1 */
}