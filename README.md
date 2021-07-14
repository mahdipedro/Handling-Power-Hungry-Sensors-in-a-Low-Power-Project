# Handling Power Hungry Sensors in a Low-Power Project
Most people use 3.3v microcontrollers and they still want to use 5v devices such as gas sensors, ultrasonic sensors, and different types of actuators. There is a solution that I found pretty useful for this challenge. We can use voltage booster such as TPS61023 which can recive 3.7V (Lipo battery voltage) and generate 5V. This chip can supply 1A current as well which is needed if you have multiple power hungry sensors that work with 5V. I have been working on a project that required us to deploy multiple gas sensors inlcuding MQ135, MQ137, and Gravity Analog Infrared CO2 Sensors. They current draw of these three sensor is (150mA + 150mA + 90ma) 390mA! The system  was portable and worked with Liopo battery which makes it challenging to provide that kind of power. Therefore, we decided to sample these sensors once in a while. Luckily TPS61023 has enable pin which can be controlled using the microcontroller. You can control the operation of the sensors by using the same method that you used for providing 5V.
Another challenge is to shift 5V output of the sensors to 3.3V for the microcontroller. The outcome of these sensors are analog signals between 0-5V. Therefore, we need a voltage divider to shift this to 0-3.3V. ![divider](https://user-images.githubusercontent.com/45086751/125548651-ebf3d046-755d-48a1-8860-e0b61aab56ae.JPG)

The above equation computes the voltage that we feed to the microcontroller. Vout will be 3.3V if Vin(sensor output) is equal to 5V.
![github_booster](https://user-images.githubusercontent.com/45086751/125549253-9073aa93-1b7d-4cac-92b8-63810835e5d8.jpg)



