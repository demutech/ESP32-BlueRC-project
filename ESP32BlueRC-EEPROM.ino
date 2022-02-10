/*******************************************************************
   ESP32 BlueRC Arduino Sketch V1.0
   Copyright Jan.2022 Xin Wang
   IR code of buttons are stored in EEPROM(Arduino) or Flash(ESP32)!!
   It works together with "BlueRC" android app. You can download it from
   my Github space:
   https://github.com/demutech/BlueRC-app
 *******************************************************************/

/* Library used:
   IRremoteESP8266: IRsendDemo - demonstrates sending IR codes with IRsend.

   Version 1.1 January, 2019
   Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009,
   Copyright 2009 Ken Shirriff, http://arcfn.com

   An IR LED circuit *MUST* be connected to the ESP8266 on a pin
   as specified by kIrLed below.

   TL;DR: The IR LED needs to be driven by a transistor for a good result.

   Suggested circuit:
       https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-sending

*/

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <assert.h>
#include <IRrecv.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>

// ==================== start of TUNEABLE PARAMETERS ====================
// An IR detector/demodulator is connected to GPIO pin 14
// e.g. D5 on a NodeMCU board.
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
const uint16_t kRecvPin = 27;
const uint16_t kIrLed = 14;  // ESP32 GPIO pin to use. On BlueRC shield: 7 (D14).
const uint32_t kBaudRate = 115200;
const uint16_t kCaptureBufferSize = 1024;

#if DECODE_AC
// Some A/C units have gaps in their protocols of ~40ms. e.g. Kelvinator
// A value this large may swallow repeats of some protocols
const uint8_t kTimeout = 50;
#else   // DECODE_AC
// Suits most messages, while not swallowing many repeats.
const uint8_t kTimeout = 15;
#endif  // DECODE_AC

const uint16_t kMinUnknownSize = 12;
const uint8_t kTolerancePercentage = kTolerance;  // kTolerance is normally 25%

#define LEGACY_TIMING_INFO false
// ==================== end of TUNEABLE PARAMETERS ====================

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.
// Use turn on the save buffer feature for more complete capture coverage.
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);

#include <BluetoothSerial.h>
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
  #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// include library to read and write from flash memory
#if defined(__AVR__) // Arduino
  #include <EEPROM.h>
#else  // ESP8266 or ESP32
  #include <Preferences.h>
  Preferences preferences;
#endif  // ESP8266

decode_results results;

/* Android App bluetooth command for button
  'u': // P+ pressed
  'd': // P- pressed
  'l': // V- pressed
  'r': // V+ pressed
  'f': // fav pressed
  'i': // info pressed
  'm': // mute pressed
  'y': // return pressed
  '0': // 0 pressed
  '1': // 1 pressed
  '2': // 2 pressed
  '3': // 3 pressed
  '4': // 4 pressed
  '5': // 5 pressed
  '6': // 6 pressed
  '7': // 7 pressed
  '8': // 8 pressed
  '9': // 9 pressed
  'n': // tv on pressed
*/

int readPref(char Button) {
  int irCode;

  switch (Button) {
    case 'u': // P+ pressed
      irCode = preferences.getUInt("u", 0);
      break;
    case 'd': // P- pressed
      irCode = preferences.getUInt("d", 0);
      break;
    case 'l': // V- pressed
      irCode = preferences.getUInt("l", 0);
      break;
    case 'r': // V+ pressed
      irCode = preferences.getUInt("r", 0);
      break;
    case 'f': // fav pressed
      irCode = preferences.getUInt("f", 0);
      break;
    case 'i': // info pressed
      irCode = preferences.getUInt("i", 0);
      break;
    case 'm': // mute pressed
      irCode = preferences.getUInt("m", 0);
      break;
    case 'y': // return pressed
      irCode = preferences.getUInt("y", 0);
      break;
    case '0': // 0 pressed
      irCode = preferences.getUInt("0", 0);
      break;
    case '1': // 1 pressed
      irCode = preferences.getUInt("1", 0);
      break;
    case '2': // 2 pressed
      irCode = preferences.getUInt("2", 0);
      break;
    case '3': // 3 pressed
      irCode = preferences.getUInt("3", 0);
      break;
    case '4': // 4 pressed
      irCode = preferences.getUInt("4", 0);
      break;
    case '5': // 5 pressed
      irCode = preferences.getUInt("5", 0);
      break;
    case '6': // 6 pressed
      irCode = preferences.getUInt("6", 0);
      break;
    case '7': // 7 pressed
      irCode = preferences.getUInt("7", 0);
      break;
    case '8': // 8 pressed
      irCode = preferences.getUInt("8", 0);
      break;
    case '9': // 9 pressed
      irCode = preferences.getUInt("9", 0);
      break;
    case 'n': // tv on pressed
      irCode = preferences.getUInt("n", 0);
      break;

    default:
      irCode = 0;
      break;
  }
  return irCode;
}

void writePref(char Button, int irCode) {
  switch (Button) {
    case 'u': // up(P+) pressed
      preferences.putUInt("u", irCode);
      break;
    case 'd': // down(P-) pressed
      preferences.putUInt("d", irCode);
      break;
    case 'l': // left(V-) pressed
      preferences.putUInt("l", irCode);
      break;
    case 'r': // right(V+) pressed
      preferences.putUInt("r", irCode);
      break;
    case 'f': // fav pressed
      preferences.putUInt("f", irCode);
      break;
    case 'i': // info pressed
      preferences.putUInt("i", irCode);
      break;
    case 'm': // mute pressed
      preferences.putUInt("m", irCode);
      break;
    case 'y': // return pressed
      preferences.putUInt("y", irCode);
      break;
    case '0': // 0 pressed
      preferences.putUInt("0", irCode);
      break;
    case '1': // 1 pressed
      preferences.putUInt("1", irCode);
      break;
    case '2': // 2 pressed
      preferences.putUInt("2", irCode);
      break;
    case '3': // 3 pressed
      preferences.putUInt("3", irCode);
      break;
    case '4': // 4 pressed
      preferences.putUInt("4", irCode);
      break;
    case '5': // 5 pressed
      preferences.putUInt("5", irCode);
      break;
    case '6': // 6 pressed
      preferences.putUInt("6", irCode);
      break;
    case '7': // 7 pressed
      preferences.putUInt("7", irCode);
      break;
    case '8': // 8 pressed
      preferences.putUInt("8", irCode);
      break;
    case '9': // 9 pressed
      preferences.putUInt("9", irCode);
      break;
    case 'n': // tv on pressed
      preferences.putUInt("n", irCode);
      break;
  }
}

void storeIRCode(char Button, int irCode) {
  int tempIRCode;

#if defined(__AVR__) // Arduino
  EEPROM.get(Button << 1, tempIRCode);
#else  // ESP8266 or ESP32
  tempIRCode = readPref(Button);
#endif  // ESP8266
  Serial.print("irCode read from EEPROM: "); Serial.println(tempIRCode, HEX);
  if (tempIRCode != irCode) // if received ir code different then stored
  {

#if defined(__AVR__) // Arduino
    // Button<<1 as EEPROM adr.!
    EEPROM.put(Button << 1, irCode);
#else  // ESP8266 or ESP32
    // Store ir code as file in flash of ESP32
    writePref(Button, irCode);
#endif  // ESP8266

    Serial.print("irCode from bluetooth: "); Serial.println(irCode, HEX);
    Serial.println("Store code successful!");
  }
  else
  {
    Serial.println("Code already exists, store failed!");
  }
}

void sendIRCode(char Button) {
  int irCode;

#if defined(__AVR__) // Arduino
  EEPROM.get(Button << 1, irCode);
#else  // ESP8266 or ESP32
  irCode = readPref(Button);
#endif  // ESP8266
  Serial.print("irCode read from EEPROM: "); Serial.println(irCode, HEX);

  irsend.sendRC5(irCode, 12, 0);
  delay(20);
}

void setup()
{
  // Prepare storeage for ir code
#if defined(__AVR__) // Arduino
  EEPROM.begin(256);
#else  // ESP8266 or ESP32
  preferences.begin("IR-Codes", false);
#endif  // ESP8266

  // begin hardware serial for monitoring
#if defined(ESP8266)
  Serial.begin(kBaudRate, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(kBaudRate, SERIAL_8N1);
#endif  // ESP8266

  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  // begin bluetooth serial
  SerialBT.begin("ESP32BlueRC-Shield"); //Bluetooth device name

  // Perform a low level sanity checks that the compiler performs bit field
  // packing as we expect and Endianness is as we expect.
  assert(irutils::lowLevelSanityCheck() == 0);

#if DECODE_HASH
  // Ignore messages with less than minimum on or off pulses.
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif  // DECODE_HASH
  irrecv.setTolerance(kTolerancePercentage);  // Override the default tolerance.
  //irrecv.enableIRIn();  // Start the receiver
  irsend.begin();
}

void loop()
{
  char button;

  if (SerialBT.available() > 0) // Incoming bluetooth message
  {
    if (SerialBT.available() > 1) // if more than one char received, learn!
    {
      SerialBT.read();
      delay(10);
      button = SerialBT.read();
      Serial.println("Button=" + String(button));
      delay(10);
      if ((button > 47) && (button < 123)) // check received bluetooth command
      {
        Serial.println("Store code started...");
        irrecv.enableIRIn();  // Start ir receiver
        delay(10);
        while (!irrecv.decode(&results)) // waiting for ir signal
        {
          yield(); // reset watchdog
        }
        Serial.println("Value: "); Serial.println(results.value);
        storeIRCode(button, results.value);
        irrecv.disableIRIn();  // Stop ir receiver
      }
      else
      {
        Serial.println("Wrong button data received!");
      }
    }
    else // if only one char is received, send ir code
    {
      button = SerialBT.read();
      delay(10);
      Serial.println("Button " + String(button) + " is pressed");
      sendIRCode(button);
    }
  }
}
