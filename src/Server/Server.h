#include "FlashFS.h"

#include <eeprom.h>

#include "DataParser.h"

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "Website/MainWebPage.h"
#include "Website/Configuration.h"
#include "Website/jQuery.h"

#include "TimeConverter.h"

#include <WiFiScanner.h>
#include <eepromManager.h>

extern EEPROMManager eeprom;
extern AsyncWebServer server;
extern NTPClient ntp;
extern TimeConverter timeConv;
extern FlashFS flashESP;
extern int addressEEPROM;
extern bool ESPRestart;
extern bool ForceUpdateTime;
extern bool checkDB;
extern int arrPinOut[];

void handleNotFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not Found");
}

void handleConfiguration(AsyncWebServerRequest *request)
{
    request->send_P(200, "text/html", Configuration_html);
}

void handleChangeNetworkSetting(AsyncWebServerRequest *request)
{
    String message = "";
    String ssid_val = "N/A";
    String pass_val = "N/A";

    if (request->hasParam("ssid", true))
    {
        ssid_val = request->getParam("ssid", true)->value();
        message = ssid_val;
    }

    if (request->hasParam("pass", true))
    {
        message += "," + request->getParam("pass", true)->value();
        pass_val = "****";
    }

    if (message.length() > 0 && message.indexOf(',') != -1)
    {
        eeprom.writeString(message, 0, addressEEPROM);
        eeprom.writeByte(addressEEPROM, message.length());
        ESPRestart = 1;
        String responseMsg = "SSID dan Password telah diganti dengan SSID: " + ssid_val + ", Pass: (disembunyikan)";
        request->send(200, "text/plain", responseMsg.c_str());
    }
    else
    {
        request->send(400, "text/plain", "Parameter SSID atau Password tidak lengkap.");
    }
}

void handleRoot(AsyncWebServerRequest *request)
{
    request->send_P(200, "text/html", MainWebPage);
}

void handleJQuery(AsyncWebServerRequest *request)
{
    request->send_P(200, "text/plain", jquery);
}

void handleDate(AsyncWebServerRequest *request)
{
    int newYear, newMonth, newDay, newDays, newHour, newMinute, newSecond;
    unsigned long epochTime = ntp.getEpochTime();
    timeConv.convertFromEpoch(epochTime, newYear, newMonth, newDay, newDays, newHour, newMinute, newSecond);
    int pin0 = digitalRead(arrPinOut[0]);
    int pin1 = digitalRead(arrPinOut[1]);
    int pin2 = digitalRead(arrPinOut[2]);
    int pin3 = digitalRead(arrPinOut[3]);
    long freeSketch = ESP.getFreeSketchSpace();
    long freeHeap = ESP.getFreeHeap();

    String response = (String)newYear + "-" +
                      (String)newMonth + "-" +
                      (String)newDay + " " +
                      (String)newDays + " " +
                      (String)newHour + ":" +
                      (String)newMinute + ":" +
                      (String)newSecond + " " +
                      (String)pin0 + " " +
                      (String)pin1 + " " +
                      (String)pin2 + " " +
                      (String)pin3 + " " +
                      (String)freeSketch + " " +
                      (String)freeHeap;

    request->send(200, "text/plain", response.c_str());
}

void handleUpdate(AsyncWebServerRequest *request)
{
    ForceUpdateTime = 1;
    request->send(200, "text/plain", "OK");
}

void handleUpdateDB(AsyncWebServerRequest *request)
{
    checkDB = true;
    request->send(200, "text/plain", "OK");
}
void handleConfigAddScheduler(AsyncWebServerRequest *request)
{
    request->send_P(200, "text/plain", AddScheduler);
}

void handleReadScheduler(AsyncWebServerRequest *request)
{
    if (!flashESP.begin())
    {
        request->send(500, "text/plain", "Filesystem Error");
        return;
    }

    String data = flashESP.readFile("/scheduler/total.txt");

    if (data == "" || data == "0")
    {
        flashESP.end();
        request->send(200, "text/plain", "no data");
        return;
    }

    String alldata = data + "^";
    int count = data.toInt();

    for (int i = 1; i <= count; i++)
    {
        String filename = "/scheduler/data/" + String(i) + ".txt";
        String _data = flashESP.readFile(filename);
        if (_data != "")
        {
            alldata += _data + "$";
        }
    }
    flashESP.end();
    request->send(200, "text/plain", alldata.c_str());
}

void handleDelete(AsyncWebServerRequest *request)
{
    if (!flashESP.begin())
    {
        request->send(500, "text/plain", "Filesystem Error");
        return;
    }

    if (request->hasParam("delete"))
    {
        String deleteValue = request->getParam("delete")->value();
        if (deleteValue == "0")
        {
            flashESP.writeFile("/scheduler/total.txt", "0");
        }
        else
        {
            String filename = "/scheduler/data/" + deleteValue + ".txt";
            if (flashESP.removeFile(filename))
            {
                checkDB = true;
                String data = flashESP.readFile("/scheduler/total.txt");
                if (data == "")
                {
                    flashESP.writeFile("/scheduler/total.txt", "0");
                }
                else
                {
                    flashESP.writeFile("/scheduler/total.txt", String(data.toInt() - 1));
                }
            }
        }
    }

    flashESP.end();
    request->send(200, "text/plain", "sucses");
}

void handleAddScheduler(AsyncWebServerRequest *request)
{
    String data = "", text = "";
    int jumlah = 0;

    if (!flashESP.begin())
    {
        request->send(500, "text/plain", "Filesystem Error");
        return;
    }

    data = flashESP.readFile("/scheduler/total.txt");
    if (data == "")
    {
        jumlah = 0;
    }
    else
    {
        jumlah = data.toInt();
    }
    if (jumlah > 20)
    {
        request->send(500, "text/plain", "RAM FULL");
        return;
    }
    String newDataString = "";

    if (request->hasParam("Date"))
    {
        text = request->getParam("Date")->value();
        text.replace("-", ",");
        newDataString = String(jumlah + 1) + "~" + text;
        if (text == "")
        {
            flashESP.end();
            request->send_P(400, "text/plain", "Bermasalah: Parameter 'Date' hilang");
        }
    }
    else
    {
        flashESP.end();
        request->send_P(400, "text/plain", "Bermasalah: Parameter 'Date' hilang");
        return;
    }

    if (request->hasParam("Clock"))
    {
        text = request->getParam("Clock")->value();
        newDataString += "|" + text;
        if (text == "")
        {
            flashESP.end();
            request->send_P(400, "text/plain", "Bermasalah: Parameter 'Date' hilang");
        }
    }
    else
    {
        flashESP.end();
        request->send_P(400, "text/plain", "Bermasalah: Parameter 'Clock' hilang");
        return;
    }

    if (request->hasParam("Tipe"))
    {
        int tipe = request->getParam("Tipe")->value().toInt();
        String sen, sel, rab, kam, jum, sab, ming;
        switch (tipe)
        {
        case 1:
            newDataString += "~1~1~1~1~1~1~1";
            break;
        case 2:
        case 4:
        case 5:
            newDataString += "~0~0~0~0~0~0~0";
            break;
        case 3:
            sen = String(request->hasParam("senin"));
            sel = String(request->hasParam("selasa"));
            rab = String(request->hasParam("rabu"));
            kam = String(request->hasParam("kamis"));
            jum = String(request->hasParam("jumat"));
            sab = String(request->hasParam("sabtu"));
            ming = String(request->hasParam("minggu"));
            newDataString += "~" + sen + "~" + sel + "~" + rab + "~" + kam + "~" + jum + "~" + sab + "~" + ming;
            break;
        default:
            flashESP.end();
            request->send_P(400, "text/plain", "Bermasalah: Nilai 'Tipe' tidak valid");
            return;
        }
    }
    else
    {
        flashESP.end();
        request->send_P(400, "text/plain", "Bermasalah: Parameter 'Tipe' hilang");
        return;
    }

    const char *params[] = {"Mode", "frekuensi", "waktu", "delay", "number", "jalur", "value", "pesan"};
    const int numParams = sizeof(params) / sizeof(params[0]);
    bool allParamsOk = true;

    for (int i = 0; i < numParams; i++)
    {
        if (request->hasParam(params[i]))
        {
            text = request->getParam(params[i])->value();
            newDataString += "~" + text;
        }
        else
        {
            allParamsOk = false;
            break;
        }
    }

    if (!allParamsOk)
    {
        flashESP.end();
        request->send_P(400, "text/plain", "Bermasalah: Parameter tambahan tidak lengkap");
        return;
    }

    String newFilename = "/scheduler/data/" + String(jumlah + 1) + ".txt";
    flashESP.writeFile(newFilename, newDataString);

    if (!flashESP.writeFile("/scheduler/total.txt", String(jumlah + 1)))
    {
        flashESP.end();
        request->send_P(500, "text/plain", "Bermasalah: Gagal menyimpan schedule baru");
        return;
    }

    flashESP.end();
    request->send(200, "text/plain", "OK");
}
void handleOutputOne(AsyncWebServerRequest *request)
{
    String data;
    if (request->hasParam("Write"))
    {
        data = request->getParam("Write")->value();
        digitalWrite(arrPinOut[0], !digitalRead(arrPinOut[0]));
        request->send(200, "text/plain", String(digitalRead(arrPinOut[0])).c_str());
        return;
    }
    else if (request->hasParam("Read"))
    {
        data = request->getParam("Read")->value();
        request->send(200, "text/plain", String(digitalRead(arrPinOut[0])).c_str());
        return;
    }
    request->send(400, "text/plain", "Salah");
}
void handleOutputTwo(AsyncWebServerRequest *request)
{
    String data;
    if (request->hasParam("Write"))
    {
        data = request->getParam("Write")->value();
        digitalWrite(arrPinOut[1], !digitalRead(arrPinOut[1])); 
        request->send(200, "text/plain", String(digitalRead(arrPinOut[1])).c_str());
        return;
    }
    else if (request->hasParam("Read"))
    {
        data = request->getParam("Read")->value();
        request->send(200, "text/plain", String(digitalRead(arrPinOut[1])).c_str());
        return;
    }
    
}
void handleOutputThree(AsyncWebServerRequest *request)
{
    String data;
    if (request->hasParam("Write"))
    {
        data = request->getParam("Write")->value();
        digitalWrite(arrPinOut[2], !digitalRead(arrPinOut[2]));
    }
    else if (request->hasParam("Read"))
    {
        data = request->getParam("Read")->value();
    }
    request->send(200, "text/plain", String(digitalRead(arrPinOut[2])).c_str());
}
void handleOutputFour(AsyncWebServerRequest *request)
{
    String data;
    if (request->hasParam("Write"))
    {
        data = request->getParam("Write")->value();
        digitalWrite(arrPinOut[3], !digitalRead(arrPinOut[3]));
    }
    else if (request->hasParam("Read"))
    {
        data = request->getParam("Read")->value();
    }
    request->send(200, "text/plain", String(digitalRead(arrPinOut[3])).c_str());
}