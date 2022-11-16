/*
 * 参考にした： https://kerikeri.top/posts/2017-04-08-esp32-mail/
 * https://github.com/LittleWat/m5stack-notify-sensors/blob/master/include/Mailer.h
 * 
 * 上だけじゃGmailのSMTPサーバに接続できない
 * SSL通信できないため
 * 
 * 証明書の登録が必要
 * WifiClientSecureのバージョンによって違う
 * 最新のものは必要
 * 参考：https://garretlab.web.fc2.com/arduino/esp32/examples/WiFiClientSecure/WiFiClientSecure.html
 
 証明書は面倒なので、認証しないことにした。
 
*/
#pragma once

#include <WiFiClientSecure.h>
#include <M5Stack.h>
#include <base64.h>

// 証明書

/*const char *gmailCA = \
  "-----BEGIN CERTIFICATE-----\n"  \
  "MIIEhjCCA26gAwIBAgIQOnkmp9hJ81oKC/XHCs9uxjANBgkqhkiG9w0BAQsFADBG\n " \
  "MQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExM\n" \
  "QzETMBEGA1UEAxMKR1RTIENBIDFDMzAeFw0yMjEwMjUxODE1MTRaFw0yMzAxMTcx\n" \
  "ODE1MTNaMBkxFzAVBgNVBAMTDnNtdHAuZ21haWwuY29tMFkwEwYHKoZIzj0CAQYI\n" \
  "KoZIzj0DAQcDQgAE+bQp2HpCVrHX86kNFumdDmpXDBrdxiZObcaTCWigZClkOMwU\n" \
  "GKhyNSuGojOVHlUg8mHcvgr/UNyUb98H/TOjraOCAmYwggJiMA4GA1UdDwEB/wQE\n" \
  "AwIHgDATBgNVHSUEDDAKBggrBgEFBQcDATAMBgNVHRMBAf8EAjAAMB0GA1UdDgQW\n" \
  "BBReYRNAo2imJu2o8zlXw47XXD1hjDAfBgNVHSMEGDAWgBSKdH+vhc3ulc09nNDi\n" \
  "RhTzcTUdJzBqBggrBgEFBQcBAQReMFwwJwYIKwYBBQUHMAGGG2h0dHA6Ly9vY3Nw\n" \
  "LnBraS5nb29nL2d0czFjMzAxBggrBgEFBQcwAoYlaHR0cDovL3BraS5nb29nL3Jl\n" \
  "cG8vY2VydHMvZ3RzMWMzLmRlcjAZBgNVHREEEjAQgg5zbXRwLmdtYWlsLmNvbTAh\n" \
  "BgNVHSAEGjAYMAgGBmeBDAECATAMBgorBgEEAdZ5AgUDMDwGA1UdHwQ1MDMwMaAv\n" \
  "oC2GK2h0dHA6Ly9jcmxzLnBraS5nb29nL2d0czFjMy9mVkp4YlYtS3Rtay5jcmww\n" \
  "ggEDBgorBgEEAdZ5AgQCBIH0BIHxAO8AdgCzc3cH4YRQ+GOG1gWp3BEJSnktsWcM\n" \
  "C4fc8AMOeTalmgAAAYQQkgIdAAAEAwBHMEUCIQD9hbPgT3PJFSYAcayEswNVtWQs\n" \
  "QJR2yxoQGTlZ9quEAQIgbMo4IzEwkDPi3xO657RTz5Shyi/32ygnseh+53wvH0AA\n" \
  "dQDoPtDaPvUGNTLnVyi8iWvJA9PL0RFr7Otp4Xd9bQa9bgAAAYQQkgIqAAAEAwBG\n" \
  "MEQCIFHPaRi3+PuHRC5kdrGplMf6ou7cHVJSy3Nlnb/1BbXYAiAtYXPy3ULoLoKq\n" \
  "1Fs5G8sIjdDy/hFOJSraWpnblrisTzANBgkqhkiG9w0BAQsFAAOCAQEAvVbcHKET\n" \
  "xNHJaqaSkzcxJLzpZWxBCIede/ECZQ+zyuQo1Dm7kfkUwhc3t/BTsK/QuD0Ei+1J\n" \
  "etGrBP9Sv6R9G3oyq5hZMVlySsYEBmd4BfYMi2d74KKcCDmxTmFpBc94do50pyCy\n" \
  "EROXJ05lh3BUYKbeWcGAveUa4LEiykJo3YiufaWHcOM1XHZfDwfzi3o37IwQCzUq\n" \
  "KRagOdCzBHkTcLAfufzMrxJbyflS0wDxDgSqLe/eDrFZqomlB7oFEEUsOJwvLQTM\n" \
  "aJzA/K4J0vjEgws9d2ngv5LkADbCN7IYettqnxMklDmOx+GffSh5JB2Z2oxl38qc\n" \
  "WRDQHSHA2NHHkQ==\n" \
  "-----END CERTIFICATE-----\n";
*/

class Mailer {
  public:
    Mailer (const char *userName, const char* password, const char *fromAddress,
      const int smtpPort = 465,
      const char *smtpHost = "smtp.gmail.com")
      : userName(userName),
        password(password),
        fromAddress(fromAddress),
        smtpPort(smtpPort),
        smtpHost(smtpHost) {}

    bool send (const String &toAddress, const String &subject, const String &content) {
      WiFiClientSecure client;

      //client.setCACert(gmailCA);
      client.setInsecure();
      Serial.printf("connecting to %s\n", smtpHost);
      if (!client.connect(smtpHost, smtpPort)) {
        Serial.printf("failed to connect\n");
        return false;
      }

      readResponse(client, "220");

      client.println("HELO example.com");
      if (!readResponse(client, "250")) {
        Serial.printf("identification failed\n");
        return false;
      }

      client.println("AUTH LOGIN");
      if (!readResponse(client, "334")) {
        Serial.printf("AUTH LOGIN failed\n");
      }

      client.println(base64::encode(userName));
      if (!readResponse(client, "334")) {
        Serial.printf("AUTH LOGIN failed\n");
        return false;
      }

      client.println(base64::encode(password));
      if (!readResponse(client, "235")) {
        Serial.printf("SMTP AUTH failed\n");
        return false;
      }

      client.println("MAIL FROM: <" + String(fromAddress) + '>');
      if (!readResponse(client, "250")) {
        Serial.printf("MAIL FROM falied");
        return false;
      }

      client.println("RCPT TO: <" + toAddress + '>');
      if (!readResponse(client, "250")) {
        Serial.printf("RCPT TO failed");
        return false;
      }

      client.println("DATA");
      if (!readResponse(client, "354")) {
        Serial.printf("SMTP DATA failed");
        return false;
      }

      client.println("From: <" + String(fromAddress) + ">");
      delay(100);
      client.println("To: <" + toAddress + ">");
      delay(100);
      client.println("Subject: " + subject);
      delay(100);
      client.println("Mime-Version: 1.0");
      delay(100);
      client.println("Content-Type: text/html");
      delay(100);
      client.println();
      delay(100);
      client.println(content);
      delay(100);
      client.println(".");

       if (!readResponse(client, "250")) {
        Serial.printf("falied to send mail\n");
        return false;
       }

       client.println("QUIT");
       if (!readResponse(client, "221")) {
        Serial.printf("QUIT falied\n");
        return false;
       }

       Serial.printf("successed\n");
       return true;
    }

  private:
    const char *userName;
    const char *password;
    const char *fromAddress;
    const int smtpPort;
    const char *smtpHost;

    bool readResponse(WiFiClientSecure& client, const String& target, uint32_t timeoutMs = 3000) {
      uint32_t startTime  = millis();

      while (1) {
        if (client.available()) break;
        if (millis() > startTime + timeoutMs) {
          Serial.printf("SMTP response TIMEOUT\n");
          return false;
        }
        delay (1);
      }

      String res = client.readStringUntil('\n');
      res.trim();
      Serial.printf("response: %s\n", res.c_str());

      if (target != "" && res.indexOf(target) == -1) return false;

      return true;
    }
};
