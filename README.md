# Handling Power Hungry Sensors in a Low-Power Project
Most people use 3.3V microcontrollers and they still want to use 5v devices such as gas sensors, ultrasonic sensors, and different types of actuators. In addition, some of these typically used sensors such as MQ series gas sensors draw a current around 160mA @ 5v which is equal to 242 mA @ 3.3V which is significantly high. We need to include other components to our system to handle such a current. For example, Adafruit Adalogger (The development baord that is used in this project) 3.3V regulator peack current is 500mA @ 3.3V which cannot be drawn continuously because it will overheat the regulator. 

There is a solution that I found pretty useful for this challenge. We can use voltage booster such as TPS61023 which can recive 3.7V (Lipo battery voltage) and generate 5V. This chip can supply 1A current as well which is needed if you have multiple power hungry sensors that work with 5V. I have been working on a project that required us to deploy multiple gas sensors inlcuding MQ135, MQ137, and Gravity Analog Infrared CO2 Sensors. The total current draw of these three sensors is (150mA + 150mA + 90ma) 390mA! The system  was portable and supplied with a Liopo battery which makes it challenging to provide that kind of power continuously. Therefore, we decided to sample these sensors once in a while. Luckily TPS61023 has enable pin which can be controlled using the microcontroller. According to the [data sheet](https://www.ti.com/lit/ds/symlink/tps61023.pdf?ts=1626311355044&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTPS61023), when Enable pin is pulled to a voltage above 1.2V the TPS61023 is enabled. Therefore, it can be controlled by a 3.3V microcontroller. You can control the operation of the sensors by using the same method that you used for providing 5V.
Another challenge is to shift 5V output of the sensors to 3.3V for the microcontroller. The outcome of these sensors are analog signals between 0-5V. Therefore, we need a voltage divider to shift this to 0-3.3V. 

![divider](https://user-images.githubusercontent.com/45086751/125552094-addf8c34-e63d-4cb5-a948-1256d4dd0c2f.JPG)

The above equation computes the voltage that we feed to the microcontroller. Vout will be 3.3V if Vin(sensor output) is equal to 5V. However, we need to check out the datasheet of the sensors to know the exact output range of each sensor. For instance, while the infrared CO2 sensor gets 5V power supply, it generates 0.4-2V analog value, corresponding to 0~5000 ppm. Therefore, we do not need a voltage divider for this sensor and we can just directly connect the output pin to the 3.3V microcontroller.
Below circuit shows how you can use multiple analog 5V sensors using a 3.3V microcontroller. The components are TPS61023 (Adafruit MiniBoost 5V @ 1A), MQ137 and MQ135 Gas Sensors, Gravity Infrared CO2 Sensor, LSM303 Motion sensor, SHT20 I2C temperature sensor, 500mAh 3.7V Lipo Battery, four resistors, and Adafruit Feather M0 Adalogger. We can turn the gas sensors on by activating TPS61023 which can be done through pin 12. We utilized TPS61023 like a Mosfet transistor which is also capable of boosting the voltage up to 5V. 


![githubv4](https://user-images.githubusercontent.com/45086751/125672955-8b26df32-c16b-4bf8-b240-8143354b2fd6.jpg)



The developed program reads and stores the data of motion and temperature sensors continuously. We read the gas sensors outputs once in a while by using digital pin 12 which enables TPS61023. We turn on the gas sensors every 30 minutes and wait for 5 minutes (Gas sensors need this time so their output could become stable) and then we store the values. This can be done by using millis() function which gives us the number of milliseconds passed since the Arduino board began running the current program.
//GasInterval is the time that we set to wake up the sensors (in this case it is every 30 minutes or 1800000 ms)
//gasReading is the time that we sample the sensor. gasReading - gasInterval = time that we wait before sampling sensor (in this case it is 300000ms which is 5 minutes)
```
unsigned long lastGasTime = 0;
const int gasInterval1 =2100000;

if (millis() - lastGasTime >= gasInterval) { 

   if (millis() - lastGasTime >= gasReading) {  
      
   lastGasTime = millis(); // go to the next cycle
       }
 }
```
