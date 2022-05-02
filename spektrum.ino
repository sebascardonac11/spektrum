#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Ticker.h>
#include <MCP3008.h>
#include "spektrum_xbus.h"

#define ledAzul D4 //Led Azul de la placa
#define ledRojo 16 // Led Rojo de la placa#define D0 16 // Led Rojo de la placa
// define pin connections MCP
#define CS_PIN D8
#define CLOCK_PIN D5
#define MOSI_PIN D7
#define MISO_PIN D6

// put pins inside MCP3008 constructor
MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);

uint16_t lipoCell[6]={3.7,3.7,3.7,3.7,3.7,3.7};
const uint8_t spek_data_size = 16;
byte spek_data_1[spek_data_size];
byte spek_data_2[spek_data_size];
bool write_buff_1 = true;
byte * buff_ptr;
STRU_TELE_LIPOMON alt_struct_Lipo;


float Vref[]= {4.13,5.21,5.0,4.23,5.0,4.35,4.10,4.14}; 

void setup() {
  Serial.begin(115200);
  pinMode(ledAzul,OUTPUT); //Led Azul de la placa
  pinMode(ledRojo,OUTPUT);// Led rojo de la placa
  digitalWrite(ledRojo,HIGH);
  //Begin I2C setup
  alt_struct_Lipo.identifier = TELE_DEVICE_LIPOMON;
  alt_struct_Lipo.sID=0;
  memcpy(alt_struct_Lipo.cell,lipoCell,6);
  alt_struct_Lipo.temp=0;
  setupI2C();
}

void loop() {
  // put your main code here, to run repeatedly:
  readCells();
  memcpy(spek_data_1, &alt_struct_Lipo, sizeof(alt_struct_Lipo));

  digitalWrite(ledAzul,HIGH);
  delay(500);
  digitalWrite(ledAzul,LOW);
  delay(500);
}
void setupI2C(){

  // Start the client I2C device
   Wire.begin(TELE_DEVICE_LIPOMON);
  Wire.onRequest(i2cRequestEvent);
 

}
void i2cRequestEvent(){

  Wire.write(spek_data_1, 16);
  Serial.println("Tamano ");
  Serial.print(sizeof(alt_struct_Lipo));
}

void readCells(){
  int val = adc.readADC(0); // read Chanel 0 from MCP3008 ADC
  float voltage=0;
  voltage = (analogRead(A0)/1024.0)*25.0;
  alt_struct_Lipo.cell[0]=(adc.readADC(0)/1024.0)*Vref[0];
  alt_struct_Lipo.cell[1]=(adc.readADC(1)/1024.0)*Vref[1];
  alt_struct_Lipo.cell[2]=(adc.readADC(3)/1024.0)*Vref[3];
  alt_struct_Lipo.cell[3]=(adc.readADC(5)/1024.0)*Vref[5];
  alt_struct_Lipo.cell[4]=(adc.readADC(6)/1024.0)*Vref[6];
  alt_struct_Lipo.cell[5]=(adc.readADC(7)/1024.0)*Vref[7];
Serial.print("cells ");
  for (int i=0; i<6; i++) {
     Serial.print("(");
    Serial.print(i);
    Serial.print(")");
    Serial.print(alt_struct_Lipo.cell[i]);
    Serial.print("\t");
    }
    Serial.print("\n");
   }
