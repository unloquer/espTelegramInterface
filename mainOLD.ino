#include  <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <ESP8266TelegramBOT.h>

// Initialize Telegram BOT
#define BOTtoken "841668198:AAGaobM1atB1iYCwseTHV14MPNUl1Vc5948"
#define BOTname "Esp_iotBot"
#define BOTusername "EspTcBot"
TelegramBOT bot(BOTtoken, BOTname, BOTusername);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;

// Initialize Wifi connection to the router
char ssid[] = "C3P";//"paralelo"; //"XXXXXX";     // your network SSID (name)
char password[] = "trespatios";//"37672027"; //"YYYYYY"; // your network key

int el_reset = D1;


/********************************************
 * EchoMessages - function to Echo messages *
 ********************************************/
void Bot_ExecMessages() {
  for (int i = 1; i < bot.message[0][0].toInt() + 1; i++)      {
    bot.message[i][5]=bot.message[i][5].substring(1,bot.message[i][5].length());
    Serial.println("GOT MESSAGE " + bot.message[i][5]);
    if (bot.message[i][5] == "ledon") {
      digitalWrite(el_reset, LOW);   // turn the LED on (HIGH is the voltage level)
      bot.sendMessage(bot.message[i][4], "Led is ON", "");
    }
    if (bot.message[i][5] == "ledoff") {
      digitalWrite(el_reset, HIGH);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(bot.message[i][4], "Led is OFF", "");
    }
    if (bot.message[i][5] == "reset") {
      digitalWrite(el_reset, HIGH);    // turn the LED off (LOW is the voltage level)
      delay(1000);
      digitalWrite(el_reset, LOW);    // turn the LED off (LOW is the voltage level)

      bot.sendMessage(bot.message[i][4], "Done reseting....gonohp", "");
    }

    if (bot.message[i][5] == "start") {
      String wellcome = "Wellcome from FlashLedBot, your personal Bot on ESP8266 board";
      String wellcome1 = "/ledon : to switch the Led ON";
      String wellcome2 = "/ledoff : to switch the Led OFF";
      String wellcome3 = "/reset : to make a resetting routine....lol";
      bot.sendMessage(bot.message[i][4], wellcome, "");
      bot.sendMessage(bot.message[i][4], wellcome1, "");
      bot.sendMessage(bot.message[i][4], wellcome2, "");
      bot.sendMessage(bot.message[i][4], wellcome3, "");
      Start = true;
    }
  }
  bot.message[0][0] = "";   // All messages have been replied - reset new messages
}

void setup(){

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //
  bot.begin();      // launch Bot functionalities
  pinMode(el_reset,OUTPUT);
  delay(2);
  digitalWrite(el_reset, LOW);

}

void loop(){


  if (millis() > Bot_lasttime + Bot_mtbs)  {
    bot.getUpdates(bot.message[0][1]);   // launch API GetUpdates up to xxx message
    Bot_ExecMessages();   // reply to message with Echo
    Bot_lasttime = millis();
  }

  /*
  digitalWrite(el_reset,LOW);
  delay(1000);
  digitalWrite(el_reset, HIGH);
  delay(1000);
*/
}
