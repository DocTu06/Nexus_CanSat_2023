#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "Adafruit_BME680.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Melopero_SAM_M8Q.h>
#include <LoRa.h>
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
Adafruit_BME680 bme;
Melopero_SAM_M8Q gps;
float datta[21];//ox,oy,oz,ax,ay,az,gx,gy,gz,mx,my,mz,temp,prs,hum,gas,lati,longi,hour,min,sec
int datta_num = 21;//the number of parameters measured
void setup() {
  Wire.begin();
  bno.begin();
  bme.begin(0x76);
  Serial.begin(9600);
  LoRa.setPins(5, 25, 26);
  if (!LoRa.begin(866E6))
  {
    while (1)
    {
      Serial.println("ok");
    }
  }
  gps.initI2C();
  gps.setCommunicationToUbxOnly();
  gps.waitForAcknowledge(CFG_CLASS, CFG_PRT);
  gps.setMeasurementFrequency(50, 2);
  gps.waitForAcknowledge(CFG_CLASS, CFG_RATE);
  gps.setMessageSendRate(NAV_CLASS, NAV_PVT, 1);
  gps.waitForAcknowledge(CFG_CLASS, CFG_MSG);
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
  SD.begin(27);
}

void loop() {
  bme.beginReading();
  gps.updatePVT();
  sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  datta[0] = orientationData.orientation.x;
  datta[1] = orientationData.orientation.y;
  datta[2] = orientationData.orientation.z;
  datta[3] = linearAccelData.acceleration.x;
  datta[4] = linearAccelData.acceleration.y;
  datta[5] = linearAccelData.acceleration.z;
  datta[6] = angVelocityData.gyro.x;
  datta[7] = angVelocityData.gyro.y;
  datta[8] = angVelocityData.gyro.z;
  datta[9] = magnetometerData.magnetic.x;
  datta[10] = magnetometerData.magnetic.y;
  datta[11] = magnetometerData.magnetic.z;
  datta[12] = gps.pvtData.hour;
  datta[13] = gps.pvtData.min;
  datta[14] = gps.pvtData.sec;
  datta[15] = gps.pvtData.latitude;
  datta[16] = gps.pvtData.longitude;
  File file = SD.open("/cansat.csv", FILE_APPEND);
  LoRa.beginPacket();
  for (int i = 0; i < 17; i++)
  {
    LoRa.print(datta[i]);
    LoRa.print(",");
    file.print(datta[i]);
    file.print(",");
  }
  bme.endReading();
  datta[17] = bme.temperature;
  datta[18] = bme.pressure / 100.0;
  datta[19] = bme.humidity;
  datta[20] = bme.gas_resistance / 1000.0;
  for (int i = 17; i < 21; i++)
  {
    LoRa.print(datta[i]);
    LoRa.print(",");
    file.print(datta[i]);
    file.print(",");
  } 
  LoRa.endPacket();
  Serial.println("ok");
  file.print("\n");
  file.close();
}
