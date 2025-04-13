#include <Arduino.h>
#include <ESP8266WiFi.h>
// #include "wifi.h"
#include "FlashFS.h"

#include <EEPROM.h>

#include "DataParser.h"

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "Website/Configuration.h"
#include "Website/Configuration.h"
#include "Website/jQuery.h"

#include <WiFiUdp.h>
#include <NTPClient.h>

#include "switchGPIO.h"
#include "NoDelayButton.h"
#include "TimeConverter.h"
#include "Server/Server.h"

#include <WiFiScanner.h>
#include <EEPROMManager.h>
#include <TimeManager.h>

#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

SoftwareSerial softSerial(-1, 15);

DFRobotDFPlayerMini myDFPlayer;
FlashFS flashESP;
bool checkDB = 0;

EEPROMManager eeprom(true);
int addressEEPROM = 100;
bool ESPRestart = 0;

WiFiScanner Scanner;

AsyncWebServer server(80);

WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP, "pool.ntp.org", 25200);
bool ForceUpdateTime = 0;

TimeConverter timeConv;

char chars[] = {'A', 'B', 'C', 'D'};
int arrPinOut[] = {2, 14, 12, 13};
NoDelayButton button(A0, chars, 3, 1023);

void CheckConnetion(String *DataNetwork);
void localHost();

uint64_t PM = 0, PMR = 0;
/*
  0 OUTPUT Tone
  1 TX
  2 TX     Musik
  3 RX
  4 SCl
  5 SDA
  12 OUTPUT
  13 OUTPUT
  14 OUTPUT
  15 OUTPUT Bell
  16 OUTPUT
  17 A0 Input
*/

void setup()
{
  Serial.begin(115200);
  pinMode(arrPinOut[0], OUTPUT);
  pinMode(arrPinOut[1], OUTPUT);
  pinMode(arrPinOut[2], OUTPUT);
  pinMode(arrPinOut[3], OUTPUT);
  Serial.println(flashESP.begin());
  Serial.println(flashESP.listDirectory("/"));
  if (flashESP.readFile("/scheduler/total.txt") == "")
  {
    flashESP.format();
    Serial.println(flashESP.writeFile("/scheduler/total.txt", "0"));
    Serial.println(flashESP.writeFile("/config.txt", "0"));
  }
  // Serial.println(flashESP.writeFile("/scheduler/total.txt", "0"));
  flashESP.end();

  eeprom.begin(512);

  WiFi.mode(WIFI_AP_STA); // Mengatur ESP dalam mode Access Point (AP) dan Station (STA)
  WiFi.disconnect();      // Memutuskan sambungan WiFi jika ada yang tersambung sebelumnya
  delay(100);

  int Count;
  String DataNetwork[2];
  DataParser::parse(eeprom.readString(0, 0, eeprom.readByte(addressEEPROM)), DataNetwork, ',', Count);

  CheckConnetion(DataNetwork);
  localHost();

  ntp.begin();
  ntp.update();

  myDFPlayer.begin(softSerial);
  myDFPlayer.volume(25);
  myDFPlayer.play(1);
}

void loop()
{
  if (ForceUpdateTime)
  {
    delay(1000);
    ntp.update();
    ForceUpdateTime = 0;
  }
  if (ESPRestart && !checkDB)
  {
    delay(5000);
    ESP.restart();
  }
  if (checkDB)
  {
    delay(1000);

    flashESP.replaceEmptyFiles("/scheduler/data/", "/scheduler/total.txt");
    checkDB = false;
  }

  String totalData;
  bool state;
  if (millis() - PM >= 1000)
  {
    PM = millis();

    flashESP.begin();
    totalData = flashESP.readFile("/scheduler/total.txt");
    flashESP.end();
    Serial.println(ESP.getFreeHeap());
    Serial.print("Jumlah Data : ");
    Serial.println(totalData);
    if (totalData == "" || totalData == "0")
    {
      return;
    }
    for (int i = 1; i <= totalData.toInt(); i++)
    {
      flashESP.begin();
      String dataEntry = flashESP.readFile("/scheduler/data/" + (String)i + ".txt");
      flashESP.end();
      if (dataEntry != "" && dataEntry != "0")
      {
        dataEntry.trim();
        dataEntry.replace(",", "~");
        dataEntry.replace("|", "~");
        dataEntry.replace(":", "~");
        // Serial.println(dataEntry);
        String OutputData[50];
        int count = 0;
        DataParser::parse(dataEntry, OutputData, '~', count);
        int64_t epochTimeNow = timeConv.convertToEpoch(OutputData[1].toInt(), OutputData[2].toInt(), OutputData[3].toInt(), OutputData[4].toInt(), OutputData[5].toInt(), 0);
        int newYear, newMonth, newDay, newDays, newHour, newMinute, newSecond;
        timeConv.convertFromEpoch(ntp.getEpochTime() + 3600 * 24 * 7, newYear, newMonth, newDay, newDays, newHour, newMinute, newSecond);
        // Serial.println(dataEntry);
        Serial.println(epochTimeNow - ntp.getEpochTime());
        if (OutputData[6].toInt() +
                OutputData[7].toInt() +
                OutputData[8].toInt() +
                OutputData[9].toInt() +
                OutputData[10].toInt() +
                OutputData[11].toInt() +
                OutputData[12].toInt() >
            0)
        {
          String rewrite = OutputData[0] + "~" +
                           (String)newYear + "," +
                           (String)newMonth + "," +
                           (String)newDay + "|" +
                           (String)newHour + ":" +
                           (String)newMinute + "~" +
                           OutputData[6] + "~" +
                           OutputData[7] + "~" +
                           OutputData[8] + "~" +
                           OutputData[9] + "~" +
                           OutputData[10] + "~" +
                           OutputData[11] + "~" +
                           OutputData[12] + "~" +
                           OutputData[13] + "~" +
                           OutputData[14] + "~" +
                           OutputData[15] + "~" +
                           OutputData[16] + "~" +
                           OutputData[17] + "~" +
                           OutputData[18] + "~" +
                           OutputData[19] + "~" +
                           OutputData[20];
          newDays == 0 ? newDays = 7 : newDays = newDays;
          Serial.println(OutputData[4].toInt());
          Serial.println(newHour);
          Serial.println(OutputData[5].toInt());
          Serial.println(newMinute);
          Serial.println(millis() - PMR);
          if (OutputData[5 + newDays] && OutputData[4].toInt() == newHour && OutputData[5].toInt() == newMinute && millis() - PMR >= 10000)
          {
            PMR = millis();
            state = 1;
            flashESP.begin();
            Serial.println(flashESP.writeFile("/scheduler/data/" + String(i) + ".txt", rewrite));
            flashESP.end();
          }
        }
        else if (epochTimeNow - ntp.getEpochTime() < -10)
        {
          flashESP.begin();
          Serial.println(flashESP.removeFile("/scheduler/data/" + String(i) + ".txt"));
          String data = flashESP.readFile("/scheduler/total.txt");
          int total = 0;
          if (data == "")
          {
            total = 0;
          }
          else
          {
            total = data.toInt();
          }
          flashESP.writeFile("/scheduler/total.txt", String(total - 1));
          checkDB = true;
          flashESP.end();
          state = 1;
        }
        if (state)
        {
          switch (OutputData[13].toInt())
          {
          case 1:
            for (int i = 0; i <= 10; i++)
            {
              int tone_A = OutputData[14].toInt();
              int tone_B = OutputData[15].toInt();
              tone(3, tone_A, tone_B);
              delay(1000);
              noTone(3);
              Serial.println("Tone");
            }
            break;
          case 2:
            for (int i = 0; i <= 10; i++)
            {
              digitalWrite(3, 1);
              delay(OutputData[16].toInt());
              digitalWrite(3, 0);
              delay(OutputData[16].toInt());
            }
            Serial.println("Bell");
            break;
          case 3:
            myDFPlayer.play(OutputData[17].toInt());
            Serial.println("MUSIK");
            break;
          default:
            digitalWrite(arrPinOut[OutputData[18].toInt()], OutputData[19].toInt());
            Serial.println("IO");
            break;
          }
          state = 0;
        }
      }
    }
  }
}

void localHost()
{
  server.onNotFound(handleNotFound);
  server.on("/Configuration", HTTP_GET, handleConfiguration);
  server.on("/ChangeNetworkSetting", HTTP_POST, handleChangeNetworkSetting);
  server.on("/", HTTP_GET, handleRoot);
  server.on("/jQuery.js", HTTP_GET, handleJQuery);
  server.on("/Date", HTTP_GET, handleDate);
  server.on("/Update", HTTP_GET, handleUpdate);
  server.on("/UpdateDB", HTTP_GET, handleUpdateDB);
  server.on("/ConfigAddScheduler", HTTP_GET, handleConfigAddScheduler);
  server.on("/ReadScheduler", HTTP_GET, handleReadScheduler);
  server.on("/Delete", HTTP_GET, handleDelete);
  server.on("/AddScheduler", HTTP_GET, handleAddScheduler);
  server.on("/OutputOne", HTTP_GET, handleOutputOne);
  server.on("/OutputTwo", HTTP_GET, handleOutputTwo);
  server.on("/OutputThree", HTTP_GET, handleOutputThree);
  server.on("/OutputFour", HTTP_GET, handleOutputFour);
  server.begin();
}
void CheckConnetion(String *DataNetwork)
{
  if (Scanner.isSSIDAvailable(DataNetwork[0]))
  {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.begin(DataNetwork[0], DataNetwork[1]);
    Serial.print("Connecting to WiFi...");
    int i = 0;
    while (WiFi.status() != WL_CONNECTED && i < 15)
    {
      i = i + 1;
      delay(500);
      Serial.println(i);
      if (i == 10)
      {
        WiFi.mode(WIFI_STA);
        WiFi.softAP("[CONFIG MODE]");
      }
    }

    Serial.println("\nConnected to WiFi.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    randomSeed(analogRead(A0));
    String randomChars;
    for (int i = 0; i < 10; i++)
    {
      randomChars += random(0, 2) == 0 ? (char)random('A', 'Z' + 1) : (char)random('a', 'z' + 1);
    }
    WiFi.softAP("[CONFIG MODE]");
    Serial.println("\nAccess Point started. " + randomChars);
    Serial.print("AP IP Address: ");
    Serial.println(WiFi.softAPIP());
  }
}