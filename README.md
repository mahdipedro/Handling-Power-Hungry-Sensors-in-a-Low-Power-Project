# Handling Power Hungry Sensors in a Low-Power Project
Most people use 3.3v microcontrollers and they still want to use 5v devices such as gas sensors, ultrasonic sensors, and different types of actuators. There is a solution that I found pretty useful for this challenge. We can use voltage booster such as TPS61023 which can recive 3.7V (Lipo battery voltage) and generate 5V. This chip can supply 1A current as well which is needed if you have multiple power hungry sensors that work with 5V. I have been working on a project that required us to deploy multiple gas sensors inlcuding MQ135, MQ137, and Gravity Analog Infrared CO2 Sensors. They total current draw of these three sensors is (150mA + 150mA + 90ma) 390mA! The system  was portable and worked with Liopo battery which makes it challenging to provide that kind of power continuously. Therefore, we decided to sample these sensors once in a while. Luckily TPS61023 has enable pin which can be controlled using the microcontroller. You can control the operation of the sensors by using the same method that you used for providing 5V.
Another challenge is to shift 5V output of the sensors to 3.3V for the microcontroller. The outcome of these sensors are analog signals between 0-5V. Therefore, we need a voltage divider to shift this to 0-3.3V. 

![divider](https://user-images.githubusercontent.com/45086751/125552094-addf8c34-e63d-4cb5-a948-1256d4dd0c2f.JPG)

The above equation computes the voltage that we feed to the microcontroller. Vout will be 3.3V if Vin(sensor output) is equal to 5V. However, we need to check out the datasheet of the sensors to know the exact output range of each sensor. For instance, While the infrared CO2 sensor gets 5V power supply, it generates 0.4-2V analog value, corresponding to 0~5000 ppm. Therefore, we do not need a voltage divider for this sensor and we can just directly connect the output pin to the 3.3V microcontroller.
Below circuit shows how you can use multiple analog 5V sensors using a 3.3 microcontroller. The components are TPS61023 (Adafruit MiniBoost 5V @ 1A), MQ137 and MQ135 Gas Sensors, Gravity Infrared CO2 Sensoe, LSM303 Motion sensor, 3.7V Lipo Battery, four resistors and Adafruit Feather M0 Adalogger. We can turn the gas sensors on by activating TPS61023 which can be done through pin 12. We utilized TPS61023 like a Mosfet transistor which is also capable of boosting the voltage up to 5V. 


![girhubv3](https://user-images.githubusercontent.com/45086751/125665012-7fd2879e-12d1-4b00-a14e-72767db8391d.jpg)




