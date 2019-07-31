#include  <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include "TelegramMethods/TelegramSendMessage.h"
#include "TelegramMethods/TelegramGetUpdates.h"

ESP8266WiFiMulti WiFiMulti;
bool NotifyReset = true;
bool SignalOpenGate = false;
bool SignalOpenDoor = false;
long UpdateId = 0;
// Initialize Telegram BOT

//long mainUserid = ;
String token = "841668198:AAGaobM1atB1iYCwseTHV14MPNUl1Vc5948";
// Initialize Wifi connection to the router
char ssid[] = "C3P";//"paralelo"; //"XXXXXX";     // your network SSID (name)
char password[] = "trespatios";//"37672027"; //"YYYYYY"; // your network key

#define EL_RESET D1



void setup(){

  Serial.begin(115200);
  pinMode(EL_RESET,OUTPUT);
  delay(2);
  digitalWrite(EL_RESET, LOW);

  ConnectionBase::SetToken(token);
  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1500);
  }
  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  // attempt to connect to Wifi network:
  //

}

void HandleUpdate(String &jsonResult)
{
    Serial.println("Got UPDATE!");
    Serial.println(ESP.getFreeHeap());
    //StaticJsonBuffer<800> jsonBuffer;
    StaticJsonDocument<800> jsonBuffer;
    JsonObject root = jsonBuffer.to<JsonObject>(); 
    //JsonObject& root = jsonBuffer.parseObject(jsonResult, 5);
    /*
    if (!root.success()) {
        Serial.println("Nooo - Failed to parse:");
        return;
    }
    */
    Serial.println("Parsed Object:");
//    serializeJson(jsonResult, Serial);
    //JsonArray& resultsArray = root["result"];
    /*
    for(JsonVariant& updateJson : resultsArray)
    {
        TelegramUpdateObject update = TelegramUpdateObject(updateJson.asObject());
        Serial.print("Current update id = ");
        Serial.println(update.m_UpdateId);
        UpdateId = update.m_UpdateId + 1;
        Serial.println(update.m_Message.m_Text);
        if(update.m_Message.m_Text.equals("Open Gate"))
        {
            SignalOpenGate = true;
        }
        if(update.m_Message.m_Text.equals("Open Door"))
        {
            SignalOpenDoor = true;
        }
    }
    **/
}
void loop(){

  if((WiFiMulti.run() == WL_CONNECTED)) {
    if(NotifyReset)
    {
      NotifyReset = false;
      TelegramSendMessage::Builder().SetText( "I'm online. Last reset = " + ESP.getResetReason()).SendMessage();
    }
    if(SignalOpenDoor)
    {
      SignalOpenDoor = false;
      //TelegramSendMessage::Builder().SetChatId(mainUserid).SetText("Opening the door, master").SendMessage();
      TelegramSendMessage::Builder().SetText("Opening the door, master").SendMessage();
      //OpenGate();
    }
    if(SignalOpenGate)
    {
      SignalOpenGate = false;
      TelegramSendMessage::Builder().SetText("Opening the gate, master").SendMessage();
      //OpenDoor();
    }
    TelegramGetUpdates::Builder().SetOffset(UpdateId).SetTimeout(10).SetLimit(1).RegisterResponseHandler(std::bind(HandleUpdate, std::placeholders::_1)).GetUpdates();
  }


  /*
  digitalWrite(EL_RESET,LOW);
  delay(1000);
  digitalWrite(EL_RESET, HIGH);
  delay(1000);
*/
}
