#include <Arduino.h>
#include <Wire.h>
#include <MPU_personalized_functions.h>
#include <Read_buttons.h> // El orden de invocación de liberías es indispensable
#include <Wifi_personalized_functions.h>



void setup() 
{
  Serial.begin(115200);

  Wire.begin(); // Iniciar I2C

  taskWifi() ; // Inicializar WiFi and websocket client
  Serial.println("Don't move the device for some seconds");

  delay(2000);

  InitializeMPU(); // Call imu.begin() to verify communication and initialize
  Serial.println("MPU has been correctly initialized!");
}

void loop() 
{
  Serial.println(wsconnected) ; 
  Send_each_second() ;
}