#include <Wire.h>
#include<stdio.h>
#define LED_PIN 13
#define analog_pin A1
 
byte RxByte;
byte TxByte = 0;
byte reg_address = 0;
byte lower_val = 0x0;
byte upper_val = 0x0;
byte comp_val[2];
uint16_t analog_data;
int time = 0;
String analog_data_str;
 
void I2C_RxHandler(byte numBytes)
{
  while(Wire.available()) {  // Read Any Received Data
    RxByte = Wire.read();
     Serial.println(RxByte);
      
    if (RxByte == 0x80)
    {
     //analogWrite(LED_PIN, 0);
     time = 0;
      //Serial.println("first mode");
    }
    else if (RxByte == 0x81)
    {
      //analogWrite(LED_PIN, 64);
      time = 250;
      //Serial.println("second mode");

    }
    else if (RxByte == 0x82)
    {
      //analogWrite(LED_PIN, 127);
      time = 500;
       //Serial.println("third mode");
    }
    else if (RxByte == 0x83)
    {
      //analogWrite(LED_PIN, 255);
      time = 1000;
       //Serial.println("forth mode");
    }
    else
    {
      reg_address = RxByte;
    }
  }
}
 
void I2C_TxHandler(void)
{
   if (reg_address == 0x0)
    {
      //Serial.println(reg_address);
      //Serial.println("printing analog");
      Wire.write(lower_val);
      Wire.write(upper_val);
      //Wire.write(comp_val*);
      //Wire.write(24);
    }
    else if (reg_address == 0x1)
    { //Serial.println("printing yolo");
      //Serial.println(RxByte);
      Wire.write("RTCA");
    }
  

}
 
void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Wire.begin(0x55); // Initialize I2C (Slave Mode: address=0x55 )
  Wire.onReceive(I2C_RxHandler);
  Wire.onRequest(I2C_TxHandler);
}
 
void loop() {
  analog_data = analogRead(analog_pin);;
  lower_val = analog_data;
  upper_val = (analog_data >> 8) ;
  //analogRead(analog_pin);
  digitalWrite (LED_PIN, HIGH);
  delay(time);
  digitalWrite (LED_PIN, LOW);
  delay(time);
  


 



}