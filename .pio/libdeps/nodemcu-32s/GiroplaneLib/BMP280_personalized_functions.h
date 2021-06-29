#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; // I2C

void inicializeBMP280(Adafruit_BMP280 &bmp){
/* This function inicialize the presure and temperature sensor BMP280 */

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void print_BMP280_values(Adafruit_BMP280 &bmp){
    /* This function print the values obtained from the 
        BMP280 sensor */
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
    Serial.println(" m");

    Serial.println();
}

void *GetHeightPreassureTemperature(Adafruit_BMP280 &bmp, float HPT[3]){
    /* This function print the values obtained from the 
        BMP280 sensor */

    HPT[0] = bmp.readAltitude(1013.25) ; 
    HPT[1] = bmp.readPressure() ; 
    HPT[2] = bmp.readTemperature() ; 
}