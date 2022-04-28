#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Ticker.h>
#include "spektrum_xbus.h"

#define ledAzul D4 //Led Azul de la placa
#define ledRojo 16 // Led Rojo de la placa#define D0 16 // Led Rojo de la placa

const uint8_t spek_data_size = 16;
byte spek_data_1[spek_data_size];
byte spek_data_2[spek_data_size];
bool write_buff_1 = true;

void setup() {
  Serial.begin(115200);
  pinMode(ledAzul,OUTPUT); //Led Azul de la placa
  pinMode(ledRojo,OUTPUT);// Led rojo de la placa
  digitalWrite(ledRojo,HIGH);

  //Begin I2C setup
  //setupI2C();
}

void loop() {
  // put your main code here, to run repeatedly:
  readCells();

  digitalWrite(ledAzul,HIGH);
  delay(500);
  digitalWrite(ledAzul,LOW);
  delay(500);
}
void setupI2C(){

  // Start the client I2C device
  Wire.onRequest(i2cRequestEvent);
  Wire.begin(TELE_DEVICE_LIPOMON);

}
void i2cRequestEvent(){

  // Write the LipoInfo data to the I2C bus
  if (write_buff_1){
    Wire.write(spek_data_2, 16);
  } else {
    Wire.write(spek_data_1, 16);
  }

  // Alternate the LED to show there are I2C events
  digitalWrite(ledAzul, write_buff_1);
  
}

void readCells(){
  float voltage=0;
  voltage = (analogRead(A0)/1024.0)*25.0;
  Serial.print("Voltage : ");
  Serial.println(voltage);
}
