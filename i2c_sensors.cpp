#include <Arduino.h>
#include <Wire.h>
#include "packets.h"
#include "bluetooth.h"

#define I2C_ADDR_TEMP 0x48
#define I2C_ADDR_IMU  0x68

static unsigned long lastSend = 0;
const unsigned long i2cInterval = 100;

void i2cInit() {
  Wire.begin(21,22);
}

uint16_t readTemp() {
  Wire.beginTransmission(I2C_ADDR_TEMP);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDR_TEMP,2);
  return (Wire.read()<<8)|Wire.read();
}

void readIMU(int16_t &ax,int16_t &ay,int16_t &az) {
  Wire.beginTransmission(I2C_ADDR_IMU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(I2C_ADDR_IMU,6);
  ax = (Wire.read()<<8)|Wire.read();
  ay = (Wire.read()<<8)|Wire.read();
  az = (Wire.read()<<8)|Wire.read();
}

void sendI2CTelemetry() {
  if(!SerialBT.hasClient()) return;
  if(millis()-lastSend<i2cInterval) return;
  lastSend=millis();

  int16_t ax,ay,az;

  i2cPacket.h1=0xCC;
  i2cPacket.h2=0x66;
  i2cPacket.temp = readTemp();
  readIMU(ax,ay,az);

  i2cPacket.ax=ax;
  i2cPacket.ay=ay;
  i2cPacket.az=az;
  i2cPacket.sensorFlags = 0b00000011;

  byte c=0;
  for(int i=2;i<I2C_PACKET_SIZE-1;i++)
    c+=((byte*)&i2cPacket)[i];

  i2cPacket.checksum=c;
  SerialBT.write((byte*)&i2cPacket,I2C_PACKET_SIZE);
}