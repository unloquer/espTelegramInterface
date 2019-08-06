#include "telegramapi.h"

String BASE_URL = "https://api.telegram.org"; // telegram base api
String TOKEN = "your_bot_token";

TelegramBot::TelegramBot ()
{
  pinMode(EL_RESET,OUTPUT);
  delay(2);
  digitalWrite(EL_RESET, HIGH);

  wifiMulti.addAP(BOT_SSID, PASSWORD);

  Serial.println("Connecting ...");
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.print('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
  Serial.print('\n');
}

TelegramBot::~TelegramBot () 
{
  Serial.println("ON telegram bot desctructor");
  //delete client;
}

void TelegramBot::getTelegramUpdates (unsigned long timeout)
{
  unsigned long start = millis();
  do {
    if (wifiMulti.run() == WL_CONNECTED)
    {
      client = new BearSSL::WiFiClientSecure();
      client->setFingerprint(fingerprint);
      String url = BASE_URL+"/bot"+TOKEN+"/getUpdates?offset="+String(this->update_offset)+"&limit=1";
      if (https.begin(*client, url))
      {
        Serial.print("[HTTP] GET...\n");
        Serial.println(ESP.getFreeHeap());
        int httpCode = https.GET();

        if (httpCode > 0)
        {
          if (httpCode == HTTP_CODE_OK || httpCode ==
              HTTP_CODE_MOVED_PERMANENTLY)
          {
            response = new DynamicJsonDocument(700);
            String payload = https.getString();

            DeserializationError error = deserializeJson(*response, payload);

            // Test if parsing succeeds.
            if (error) {
              Serial.print(F("deserializeJson() failed: "));
              Serial.println(error.c_str());
              return;
            }
            JsonObject obj = response->as<JsonObject>();
            String hasResult = obj["result"][0];
            String chat_offset = obj["result"][0]["update_id"];
            setUpdateOffset(chat_offset);
            if (hasResult != "null")
            {
              String msg = obj["result"][0]["message"]["text"];
              if (msg == "reset" || msg == "Reset") {
                digitalWrite(EL_RESET,LOW);
                delay(3000);
                digitalWrite(EL_RESET, HIGH);

              } else {

                Serial.print("got message:  ");
                Serial.println(msg);

              }

            }
            Serial.print("[HTTP] GET success...\n");
            serializeJsonPretty(*response, Serial);
            Serial.println(ESP.getFreeHeap());
          }

        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        Serial.printf("freeing response, ending https....\n");
        delete response;
        https.end();
      } else {
        Serial.printf("HTTPS UNABLE TO CONNECT\n");
      }
      Serial.printf("freeing client,....\n");
      delete client;
      yield();

    }

  } while (millis() - start < timeout);

}

void TelegramBot::setUpdateOffset(String update)
{
  int tmp = update.toInt();

  if(tmp > 0) {
    this->update_offset = tmp + 1;

  } else {
    this->update_offset = 0;
  }
}

int TelegramBot::getResetPin(void)
{
  return this->reset_pin;
}

void TelegramBot::resetCallback(unsigned long timeout)
{

  this->reset_pin = LOW;
  delay(timeout);
  this->reset_pin = HIGH;


}
