#include <Arduino.h>
#include <Wire.h>
#include <MPU_personalized_functions.h>
#include <Wifi_personalized_functions.h>


void setup() 
{
  Serial.begin(115200);

  Wire.begin(); // Iniciar I2C

  taskWifi() ; // Inicializar WiFi and websocket client

  delay(2000);

  InitializeMPU(); // Call imu.begin() to verify communication and initialize
  
}

void loop() 
{
  Send_constinously() ; 
  delay(50) ; 
}