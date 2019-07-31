#include  <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
// #include <WiFiClientSecure.h>   // Include the HTTPS library
#include <WiFiClientSecureBearSSL.h>
//#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Initialize Telegram BOT
ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

// telegram sha-1 fingerprint
// BB DC 45 2A 07 E3 4A 71 33 40 32 DA BE 81 F7 72
// 6F 4A 2B 6B
const uint8_t fingerprint[20] = {0xBB, 0xDC, 0x45, 0x2A, 0x07, 0xE3, 0x4A, 0x71,
  0x33, 0x40, 0x32, 0xDA, 0xBE, 0x81, 0xF7, 0x72, 0x6F, 0x4A, 0x2B, 0x6B};
// urls config
String BASE_URL = "https://api.telegram.org"; // https://api.telegram.org
String token =  // the telegram bse url
String bot_url = BASE_URL+"/bot"+token;
//long mainUserid = ;
// Initialize Wifi connection to the router
char ssid[] = //"paralelo"; //"XXXXXX";     // your network SSID (name)
char password[] = //"37672027"; //"YYYYYY"; // your network key
// client
// WiFiClientSecure client;
#define EL_RESET D4
DynamicJsonDocument * response;
int offset = 0;

void setup(){

  Serial.begin(115200);
  pinMode(EL_RESET,OUTPUT);
  delay(2);
  digitalWrite(EL_RESET, HIGH);

  Serial.print(BASE_URL);
  // attempt to connect to Wifi network:
  //
  wifiMulti.addAP(ssid, password);

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
// Connect with api.telegram.org       


  Serial.println("going to loop now");
}

void loop(){

  if(wifiMulti.run() == WL_CONNECTED) {
//    String url = bot_url+"/getMe";
    // WiFiClientSecure client;
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    client->setFingerprint(fingerprint);

    HTTPClient https;
    String url = bot_url+"/getUpdates?offset="+String(offset)+"&limit=1";
//    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, url)) {
        Serial.print("[HTTP] GET...\n");
        Serial.println(url);
        int httpCode = https.GET();
          // DynamicJsonDocument(1000) * doc;
//          StaticJsonDocument<1000> res;
        response = new DynamicJsonDocument(1000);
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode ==
            HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          // Deserialize the JSON document
          DeserializationError error = deserializeJson(*response, payload);
 //         DeserializationError error = deserializeJson(res, payload);

          // Test if parsing succeeds.
          if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
            return;
          }
          JsonObject obj = response->as<JsonObject>();
          String hasResult = obj["result"][0];
          /*
          String hasResult = res["result"][0];
*/
          if (hasResult == "null") {
            //means there is no update, reset offset en return
            // res.clear();
            response->clear();
            offset = 0;
            https.end();
            Serial.println("no new messages.......skipping");
            return;
          }
         String chat_offset = obj["result"][0]["update_id"];
         Serial.print("new url offset:  ");
         Serial.println(chat_offset.toInt()+1);
         String msg = obj["result"][0]["message"]["text"];
         offset = chat_offset.toInt() + 1;

         if (msg == "reset" || msg == "Reset") {
           Serial.println("reset meeeeeeee!");
           digitalWrite(EL_RESET,LOW);
           delay(3000);
           digitalWrite(EL_RESET, HIGH);

         } else {

           Serial.print("got message:  ");
           Serial.println(msg);

         }
          // JsonObject root = response->as<JsonObject>();
//          char * result = &response[0];//*response["result"]["update_id"];
          /// const char * result = root["result"];
//          String result = res["result"]["update_id"];
   //       Serial.printf("result response: %s\n", result );
          /*
          String name = doc["result"]["first_name"];

          Serial.print("Parsed name with ArduinoJson:  ");
          Serial.println(name);
          */
         // Serial.print("response: ");
          // serializeJsonPretty(*response, Serial);
          // res.clear();
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      Serial.println("clearing json buffer, ending https");

      response->clear();
      https.end();
      
    } else {
      Serial.print("WTF it din't connect");
      Serial.println(BASE_URL);
    }
    //wait one second and do it again

//    delay(1000);
  }
  /*
     long now;
     if (client.connect(BASE_URL,443)) {
     Serial.println("Connected to telegram!....");
     String command = "/getMe";
     String msg = "";
     now = millis();
     client.println("GET"+bot_url+command);
    while (millis() - now < 5000) {
      while (client.available()) {
        char c = client.read();
        msg+=c;
        Serial.print("el message: ");
        Serial.println(msg);
      }
    }
  } else {
    Serial.print("WTF it din't connect");
    Serial.println(BASE_URL);
  }
*/

  /*
  digitalWrite(EL_RESET,LOW);
  delay(1000);
  digitalWrite(EL_RESET, HIGH);
  delay(1000);
*/
}
