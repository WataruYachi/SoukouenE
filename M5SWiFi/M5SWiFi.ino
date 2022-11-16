#include <WiFi.h>
#include <M5Stack.h>
#include "Mailer.h"
#include "Password.h"
//#include <ezTime.h>

const char *ssid = "Denki4F-Zikken-2.4G";
// wifiPassword -> Pass.h
const char *smtpUserName = "s213112@gm.ishikawa-nct.ac.jp";
// smtpPassword -> Pass.h
const char *smtpFromAddress = "s213112@gm.ishikawa-nct.ac.jp";
const int smtpPort = 465;
const char *smtpHostName = "smtp.gmail.com";

const char *toAddress = "s213112@gm.ishikawa-nct.ac.jp";
const char * subject = "test";

const char *message = "これはM5Stackから送信されています。";

Mailer mail(smtpUserName, smtpPassword, smtpFromAddress, smtpPort, smtpHostName);

//Timezone Tokyo;

//WiFiServer server(80);

void setup() {
  M5.begin();
  Serial.begin(115200);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("connecting");

  WiFi.begin(ssid, wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }

  M5.Lcd.println("Successed");
  M5.Lcd.println("IP :");
  M5.Lcd.println(WiFi.localIP());

  //Tokyo.setLocation("Asia/Tokyo");
  //server.begin();

}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()) {
    M5.Lcd.println("send EMAIL");
    mail.send(toAddress, subject, message);
  }
}
