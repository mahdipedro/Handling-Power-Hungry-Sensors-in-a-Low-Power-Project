//This code is developed to sample sensor data that includes MQ135, MQ137, Gravity Infrared CO2, Accelerometer, Magnetometer, and electret microphone.
//We turn on gas sensors once in a while using TPS61023. Then we wait for 5 minutes so the outputs of the sensors become stable and then we sample it.
//Sampled data stroed into the SD card and it can also be observed through serial monitor.

// The load resistance on the board
#define RLOAD 22.0 //MQ135
#include "MQ135.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include "DFRobot_SHT20.h"
#include <SD.h>
#define RL 47  //The value of resistor RL is 47K (MQ137)
#define m -0.263 //Enter calculated Slope (MQ137)
#define b 0.42 //Enter calculated intercept (MQ137)
#define Ro 37 //Enter found Ro value (MQ137)
#define MQ_sensor A0 //Sensor is connected to A0
#define cardSelect 4
MQ135 gasSensor = MQ135(A1);
String delim = "," ;
int sensorIn = A4;
int val;
int sensorPin = A1;
int sensorValue = 0;
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
const int gasInterval = 1800000;
unsigned long lastGasTime = 0;
const int gasInterval1 =2100000;
unsigned long lastGasTime1 = 0;

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

DFRobot_SHT20    sht20;
unsigned long myTime;
void setup() {

  pinMode(9, OUTPUT);    // sets the digital pin 13 as output
  pinMode(12, OUTPUT);
  digitalWrite(9, LOW);
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  mag.enableAutoRange(true);
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
    sht20.initSHT20();                                  // Init SHT20 Sensor
    delay(100);
    sht20.checkSHT20(); 
    
    if (!SD.begin(cardSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
}
void loop() {

  
  float humd = sht20.readHumidity();                  // Read Humidity
  float temp = sht20.readTemperature();               // Read Temperature

  myTime = millis();
  
  sensors_event_t event; //accel 
  sensors_event_t event1; //mag    
  mag.getEvent(&event);
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
//  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
//  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
//  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
 // sensors_event_t event;
 
  accel.getEvent(&event1);
  /* Display the results (acceleration is measured in m/s^2) */
 // Serial.print("X: "); Serial.print(event1.acceleration.x); Serial.print("  ");
 // Serial.print("Y: "); Serial.print(event1.acceleration.y); Serial.print("  ");
 // Serial.print("Z: "); Serial.print(event1.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");

  //microphone
  digitalWrite(12, HIGH);
  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
   //digitalWrite(47, HIGH);
      // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(3);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 3.3) / 1024;  // convert to volts

   File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (millis() - lastGasTime >= gasInterval) {
  digitalWrite(9, HIGH); // sets the digital pin 13 on
  
    if (dataFile) {

    dataFile.println(myTime + delim +event1.acceleration.x+ delim + event1.acceleration.y+ delim + event1.acceleration.z+ delim+ event.magnetic.x+ delim + event.magnetic.y+ delim + event.magnetic.z+ delim+ volts + delim + temp + delim + humd);

  }
  Serial.println(myTime + delim + event1.acceleration.x+ delim + event1.acceleration.y+ delim + event1.acceleration.z+ delim+ event.magnetic.x+ delim + event.magnetic.y+ delim + event.magnetic.z+ delim+ volts + delim + temp + delim + humd);
  delay(100);
    if (millis() - lastGasTime1 >= gasInterval1) {
      
        int sensorValue = analogRead(sensorIn);
        float voltage = sensorValue*(3300/1024.0);
        int voltage_diference=voltage-400;
        float concentration=voltage_diference*50.0/16.0;

  
        float VRL; //Voltage drop across the MQ sensor
        float Rs; //Sensor resistance at gas concentration 
        float ratio; //Define variable for ratio
   
        VRL = analogRead(MQ_sensor)*(3.3/1023.0); //Measure the voltage drop and convert to 0-5V
        Rs = ((3.3*RL)/VRL)-RL; //Use formula to get Rs value
        ratio = Rs/Ro;  // find ratio Rs/Ro
        float ppm1 = pow(10, ((log10(ratio)-b)/m)); //use formula to calculate ppm
        
        //val = analogRead(A1);
        float ppm = gasSensor.getPPM();

      
      
        digitalWrite(9, LOW);
            if (dataFile) {

    dataFile.println(myTime + delim +event1.acceleration.x+ delim + event1.acceleration.y+ delim + event1.acceleration.z+ delim+ event.magnetic.x+ delim + event.magnetic.y+ delim + event.magnetic.z+ delim+ volts + delim + temp + delim + humd+ delim + ppm + delim + ppm1 +delim +concentration);

  }
        Serial.println(myTime + delim +event1.acceleration.x+ delim + event1.acceleration.y+ delim + event1.acceleration.z+ delim+ event.magnetic.x+ delim + event.magnetic.y+ delim + event.magnetic.z+ delim+ volts + delim + temp + delim + humd + delim + ppm + delim + ppm1 +delim +concentration);
        
        delay(100);
        lastGasTime1 = millis();
        lastGasTime = millis();
    
     }
     else {
            if (dataFile) {

    dataFile.println(myTime + delim +event1.acceleration.x+ delim + event1.acceleration.y+ delim + event1.acceleration.z+ delim+ event.magnetic.x+ delim + event.magnetic.y+ delim + event.magnetic.z+ delim+ volts + delim + temp + delim + humd);

                    }
      Serial.println(myTime + delim +event1.acceleration.x+ delim + event1.acceleration.y+ delim + event1.acceleration.z+ delim+ event.magnetic.x+ delim + event.magnetic.y+ delim + event.magnetic.z+ delim+ volts + delim + temp + delim + humd);
     }
  
}
else {
      if (dataFile) {

    dataFile.println(myTime + delim +event1.acceleration.x+ delim + event1.acceleration.y+ delim + event1.acceleration.z+ delim+ event.magnetic.x+ delim + event.magnetic.y+ delim + event.magnetic.z+ delim+ volts + delim + temp + delim + humd);

                    }
  Serial.println(myTime + delim +event1.acceleration.x+ delim + event1.acceleration.y+ delim + event1.acceleration.z+ delim+ event.magnetic.x+ delim + event.magnetic.y+ delim + event.magnetic.z+ delim+ volts + delim + temp + delim + humd);
}
dataFile.close();
}
