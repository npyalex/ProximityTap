/****************************************************************
Proximity Wearable
by Nick Alexander @ OCADU Digital Futures
based on I2C_ZX_Demo.ino
XYZ Interactive ZX Sensor
Shawn Hymel @ SparkFun Electronics

Takes sensor readings along the Z and X axes
Nudges the wearer to indicate proximity by vibrating

Intended for a wearable that rests the vibe motors on the wearer's clavicle
Sensor positioned on the back of the neck

Tested with Arduino Uno

Hardware Connections:
 
 Arduino Pin  ZX Sensor Board  Function
 ---------------------------------------
 5V           VCC              Power
 GND          GND              Ground
 A4           DA               I2C Data
 A5           CL               I2C Clock

Resources:
Include Wire.h and ZX_Sensor.h

****************************************************************/

#include <Wire.h>
#include <ZX_Sensor.h>

// Constants
const int ZX_ADDR = 0x10;  // ZX Sensor I2C address
const int vibePinMid = 3; // vibration motor pins
const int vibePinLeft = 5;
const int vibePinRight = 6;

// Global Variables
ZX_Sensor zx_sensor = ZX_Sensor(ZX_ADDR);
uint8_t x_pos;
uint8_t z_pos;

int vibeStrength;
int nullReading;
void setup() {

  pinMode(vibePinMid, OUTPUT);
  pinMode(vibePinLeft, OUTPUT);
  pinMode(vibePinRight,OUTPUT);
  uint8_t ver;

  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println("-----------------------------------");
  Serial.println("SparkFun/GestureSense - I2C ZX Demo");
  Serial.println("-----------------------------------");

  // Initialize ZX Sensor (configure I2C and read model ID)
  if ( zx_sensor.init() ) {
    Serial.println("ZX Sensor initialization complete");
  } else {
    Serial.println("Something went wrong during ZX Sensor init!");
  }
  
  // Read the model version number and ensure the library will work
  ver = zx_sensor.getModelVersion();
  if ( ver == ZX_ERROR ) {
    Serial.println("Error reading model version number");
  } else {
    Serial.print("Model version: ");
    Serial.println(ver);
  }
  if ( ver != ZX_MODEL_VER ) {
    Serial.print("Model version needs to be ");
    Serial.print(ZX_MODEL_VER);
    Serial.print(" to work with this library. Stopping.");
    while(1);
  }
  
  // Read the register map version and ensure the library will work
  ver = zx_sensor.getRegMapVersion();
  if ( ver == ZX_ERROR ) {
    Serial.println("Error reading register map version number");
  } else {
    Serial.print("Register Map Version: ");
    Serial.println(ver);
  }
  if ( ver != ZX_REG_MAP_VER ) {
    Serial.print("Register map version needs to be ");
    Serial.print(ZX_REG_MAP_VER);
    Serial.print(" to work with this library. Stopping.");
    while(1);
  }
}

void loop() {
  // map the Z position of the sensor
  // to the PWM vibration of the motor
  // If there is position data available, read and print it
  if ( zx_sensor.positionAvailable() ) {
    nullReading = 0;
    x_pos = zx_sensor.readX(); // 240 is right and 1 is left
    if ( x_pos != ZX_ERROR ) {
      Serial.print("X: ");
      Serial.print(x_pos);
    if (x_pos <=80){
      analogWrite(vibePinLeft, vibeStrength);
      } else {
      analogWrite(vibePinLeft, 0);
      }
    if ((x_pos >=81)&&(x_pos <=160)){
      analogWrite(vibePinMid, vibeStrength);
      } else {
      analogWrite(vibePinMid, 0);
      }
    if (x_pos >=161){
      analogWrite(vibePinRight, vibeStrength);
      } else {
      analogWrite(vibePinRight, 0);
      }
    }
    z_pos = zx_sensor.readZ();
    if ( z_pos != ZX_ERROR ) {
      Serial.print(" Z: ");
      Serial.println(z_pos);
    vibeStrength = map(z_pos, 240, 0, 0, 255);
    }

  }
  if (!zx_sensor.positionAvailable() ){
      nullReading++;
  }
  if (nullReading>=500){
    analogWrite(vibePinMid, 0);
    analogWrite(vibePinLeft, 0);
    analogWrite(vibePinRight, 0);
  }
}
