#include "telegramapi.h"

TelegramBot * telegramBot;


void setup ()
{
  Serial.begin(115200);
  while(!Serial) { ; }

  telegramBot = new TelegramBot();

  Serial.println("coing to loop....");


}

void loop ()
{
  Serial.print("memory before update");
  Serial.println(ESP.getFreeHeap());
  telegramBot->getTelegramUpdates(1000);
//  digitalWrite(EL_RESET, telegramBot->getResetPin());
  //Serial.println(telegramBot->getUpdateOffset());

}
