#ifndef TLGRM_API
#define TLGRM_API

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const uint8_t fingerprint[20] = {0xBB, 0xDC, 0x45, 0x2A, 0x07, 0xE3, 0x4A, 0x71,
  0x33, 0x40, 0x32, 0xDA, 0xBE, 0x81, 0xF7, 0x72, 0x6F, 0x4A, 0x2B, 0x6B};
#define EL_RESET D1
#define BOT_SSID "YOURSSID"    // your network SSID (name)
#define  PASSWORD "YOURPASSWORD"// your network key
class TelegramBot {

  private:
    DynamicJsonDocument * response;
    ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
    BearSSL::WiFiClientSecure * client;
    HTTPClient https;
    unsigned int update_offset;
    unsigned int reset_pin;
    void resetCallback(unsigned long timeout);
  public:
    TelegramBot();
    ~TelegramBot();
    void getTelegramUpdates(unsigned long timeout);
    void setUpdateOffset(String update);
    int getResetPin(void);
};
#endif
