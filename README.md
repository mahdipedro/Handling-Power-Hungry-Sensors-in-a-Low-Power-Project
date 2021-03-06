# Handling Power Hungry Sensors in a Low-Power Project
Most people use 3.3V microcontrollers and they still want to use 5v devices such as gas sensors, ultrasonic sensors, and different types of actuators. In addition, some of these typically used sensors such as MQ series gas sensors draw a current around 160mA @ 5v which is equal to 242 mA @ 3.3V which is significantly high. We need to include other components to provide 5V and at the same time they need to be capable of handling such a current. For example, Adafruit Adalogger (The development baord that is used in this project) 3.3V regulator peack current is 500mA @ 3.3V which cannot be drawn continuously because it will overheat the regulator. 

There is a solution that I found pretty useful for this challenge. We can use voltage booster such as TPS61023 which can recive 3.7V (Lipo battery voltage) and generate 5V. This chip can supply 1A current as well which is needed if you have multiple power hungry sensors that work with 5V. I have been working on a project that required us to deploy multiple gas sensors inlcuding MQ135, MQ137, and Gravity Analog Infrared CO2 Sensors. The total current draw of these three sensors is 390mA(150mA + 150mA + 90mA)! The system  was portable and supplied with a Liopo battery which makes it challenging to provide that kind of power continuously. Therefore, we decided to sample these sensors once in a while. Note that TPS61023 has enable pin which can be controlled using the microcontroller. According to the [data sheet](https://www.ti.com/lit/ds/symlink/tps61023.pdf?ts=1626311355044&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTPS61023), when Enable pin is pulled to a voltage above 1.2V the TPS61023 is enabled. Therefore, it can be controlled by a 3.3V microcontroller. You can control the operation of the sensors by using the same method that you used for providing 5V.
Another challenge is to shift 5V output of the sensors to 3.3V for the microcontroller. The outcome of these sensors are analog signals between 0-5V. Therefore, we need a voltage divider to shift this to 0-3.3V. 

![divider](https://user-images.githubusercontent.com/45086751/125552094-addf8c34-e63d-4cb5-a948-1256d4dd0c2f.JPG)

The above equation computes the voltage that we feed to the microcontroller. Vout will be 3.3V if Vin(sensor output) is equal to 5V. However, we need to check out the datasheet of the sensors to know the exact output range of each sensor. For instance, while the infrared CO2 sensor gets 5V power supply, it generates 0.4-2V analog value, corresponding to 0~5000 ppm. Therefore, we do not need a voltage divider for this sensor and we can just directly connect the output pin to the 3.3V microcontroller.
Below circuit shows how you can use multiple analog 5V sensors using a 3.3V microcontroller. The components are TPS61023 (Adafruit MiniBoost 5V @ 1A), MQ137 and MQ135 Gas Sensors, Gravity Infrared CO2 Sensor, LSM303 Motion sensor, SHT20 I2C temperature sensor, Electret Microphone, 500mAh 3.7V Lipo Battery, four resistors, and Adafruit Feather M0 Adalogger. We can turn the gas sensors on by activating TPS61023 which can be done through pin 12. We utilized TPS61023 like a Mosfet transistor which is also capable of boosting the voltage up to 5V. We also need to ensure that power-hungry sensors are supplied directly through battery and TPS61023. This can be done by placing the sensors close to the battery and connect them directly to the TPS61023 (follow the gnd and vcc of the gas sensors in the circuit). We need to be careful because if we pass that kind of current through microcontroller board, it will damage the board!

![githubv5](https://user-images.githubusercontent.com/45086751/125892856-0bb11355-6390-47eb-a019-555b48dbdef8.jpg)



The developed program reads and stores the data of microphone, motion and temperature sensors continuously. However, we may not use microphone all the time so we connect its Vdd to pin A3 to turn it on or off if needed. Microphone requires [3mA current](https://learn.adafruit.com/adafruit-agc-electret-microphone-amplifier-max9814/overview) which can be taken care of by the microcontroller. Because we know from the [SAMD21 microcontroller data sheet](https://cdn-learn.adafruit.com/assets/assets/000/046/243/original/adafruit_products_Feather_M0_Adalogger_v2.2-1.png?1504885273) that each pin can provide a constant current of 7mA safely.
We read the gas sensors outputs once in a while by using digital pin 12 which enables TPS61023. We turn on the gas sensors every 30 minutes and wait for 5 minutes (Gas sensors need this time so their output could become stable) and then we store the values. This can be done by using millis() function which gives us the number of milliseconds passed since the Arduino board began running the current program.


```
const int gasInterval = 1800000; 
unsigned long lastGasTime = 0;
const int gasReading =2100000;

if (millis() - lastGasTime >= gasInterval) {  //GasInterval is the time that we set to wake up the sensors (in this case it is every 30 minutes or 1800000 ms)
          // we turn on the sensors or other stuff that need to be done
      if (millis() - lastGasTime >= gasReading) {  //gasReading is the time that we sample the sensor. gasReading - gasInterval = time that we wait before sampling sensor (in this case it is 300000ms which is 5 minutes)
            // we read the sensros values or other stuff that need to be done
   lastGasTime = millis(); // go to the next cycle
       }
 }
```
