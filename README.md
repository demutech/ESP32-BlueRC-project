# ESP32-BlueRC-project
An adaptive universal ir remote control by using Bluetooth with ESP32 or Arduino Uno R3, Arduino IDE and Android app.

![WEMOS-D1-R32](https://user-images.githubusercontent.com/13086712/153417037-3587a04c-3ead-46b8-a382-e709bd21e123.JPG)
![Sensor-shield-with-ir-transmitter-module](https://user-images.githubusercontent.com/13086712/153416974-a9ecbab6-4202-4918-b1ad-7d55bfd659fb.JPG)

I would like to introduce you this adaptive universal infrared remote control. The technology is based on ESP32 Bluetooth and operated via an Android app.

Before I started this project, i compared several projects on the web. Most of them work with predefined keys, that means the IR codes must be pre-read and stored in an array. I'm trying to build a fully learning remote control. The IR codes should be stored in real time in ESP32.

I chose Bluetooth for communication because it is low power and has a short latency time then Wi-Fi. 

ESP32 has both Bluetooth and WiFi, and works well with Arduino IDE. I tried the IR library for Arduino first. It works fine, unfortunately not fully supported on ESP32. ESP32 uses RMT for sending and receiving IR signals. After many try and errors, i finally found a fully working IR library for ESP32 “IRremoteESP8266” by Ken Shirriff. Thank you Ken. Link of library as follows,
https://www.arduino.cc/reference/en/libraries/irremoteesp8266/
 
For the hardware, i was looking for a commercially available solution, but unfortunately found nothing that was 100% suitable. I decided for a partial solution. I took the D1 R32 from Wemos as developing board, which has a similar pin layout to the Arduino UNO. A multiple Sensor Shield is placed on top of it. Unfortunately, the Shield only has an IR receiver, no transmitter. So I have to build my own transmitter. With the motto "Simple and Dirty", a plug-in IR transmitter module with driver function is quickly made. It consists of only 3 components, a 3 pin header socket, a BC547 NPN transistor and an IR transmitter LED, circuit as below. I use io-port D14 to control the ir tranmitter. The IR receiver is connected to D27 by default. 

The firmware is also relatively simple. ESP32 acts as Bluetooth server, Android app as client. ESP32 is waiting for the messages from app. There is minimal communication between the app and the ESP32, all buttons are sent to the ESP32 as a single character via Bluetooth. Originally I wanted to store the IR codes in EEPROM. After I implemented it, I first noticed that ESP32 has no EEPROM at all, instead there is a possibility to save data permanently in Flash via Preferences. Since my remote control has RC5 coding, i only implemented RC5. You may have to modify the Arduino sketch if your own remote control is different coded.

Have fun with it.
