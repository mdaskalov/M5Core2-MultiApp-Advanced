#include "WiFiSettings.h"
#include <M5Core2.h>

void WifiSettingsClass::AP_Mode()
{
    WiFi.disconnect();
    vTaskDelay(200 / portTICK_PERIOD_MS);
    WiFi.mode(WIFI_MODE_AP);
    menu.WiFi_Mode = WIFI_MODE_AP;
    WiFi.begin("M5Stack");
    WiFi.softAPsetHostname("M5Stack");
    M5.Lcd.drawString("AP Mode Started", 5, 50, 2);
    M5.Lcd.drawString("Host Name: M5Stack", 5, 70, 2);
    M5.Lcd.drawString("IP Address: " + WiFi.softAPIP().toString(), 5, 90, 2);
}

void WifiSettingsClass::STA_Mode()
{
    WiFi.disconnect();
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin();
    menu.WiFi_Mode = WIFI_MODE_STA;
    M5.Lcd.drawString("STA Mode Started", 5, 50, 2);
    M5.Lcd.drawString("Will Connect to stored SSID", 5, 70, 2);
}

void WifiSettingsClass::APSTA_Mode()
{
    WiFi.disconnect();
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.begin();
    menu.WiFi_Mode = WIFI_MODE_APSTA;
    M5.Lcd.drawString("AP + STA Mode Started", 5, 50, 2);
    M5.Lcd.drawString("Will use the stored SSID", 5, 70, 2);
}

void WifiSettingsClass::SmartConfig()
{
    int i = 0;
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();
    M5.Lcd.drawString("Waiting for SmartConfig", 5, 30, 2);
    while (!WiFi.smartConfigDone())
    {
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.drawNumber(i, 5, 50, 2);
        delay(500);
        M5.Lcd.setTextColor(BLACK);
        M5.Lcd.drawNumber(i, 5, 50, 2);
        if (i == 119)
        {
            M5.Lcd.setTextColor(WHITE);
            M5.Lcd.drawString("SmartConfig NOT received!", 5, 70, 2);
            STA_Mode();
            return;
        }
        i++;
    }
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.drawString("SmartConfig received", 5, 70, 2);
    M5.Lcd.drawString("Waiting for WiFi", 5, 90, 2);
    i = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.drawNumber(i, 5, 110, 2);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        M5.Lcd.setTextColor(BLACK);
        M5.Lcd.drawNumber(i, 5, 110, 2);
        if (i == 59)
        {
            STA_Mode();
            M5.Lcd.setTextColor(WHITE);
            M5.Lcd.drawString("Wifi Not Found!", 5, 130, 2);
            return;
        }
        i++;
    }
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.drawString("WiFi Connected", 5, 130, 2);
    M5.Lcd.drawString("IP: " + WiFi.localIP().toString(), 5, 150, 2);
    menu.WiFi_Mode = WIFI_MODE_STA;
}

void WifiSettingsClass::Run()
{
    menu.clearList();
    menu.setListCaption("WiFi");
    menu.addList("WiFi SmartConfig");
    menu.addList("Connect by WPS Button");
    menu.addList("Connect by WPS Pin Code");
    menu.addList("WiFi STA");
    menu.addList("WiFi AP");
    menu.addList("WiFi OFF");
    menu.showList();

    while (!M5.BtnA.wasPressed())
    {
        if (M5.BtnC.wasPressed())
        {
            menu.nextList();
        }
        if (M5.BtnB.wasPressed())
        {
            if (menu.getListString() == "WiFi STA")
            {
                menu.windowClr();
                STA_Mode();
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                menu.windowClr();
                menu.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));
                menu.showList();
            }
            if (menu.getListString() == "WiFi SmartConfig")
            {
                menu.windowClr();
                SmartConfig();
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                menu.windowClr();
                menu.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));
                menu.showList();
            }
            if (menu.getListString() == "WiFi AP")
            {
                menu.windowClr();
                AP_Mode();
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                menu.windowClr();
                menu.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));
                menu.showList();
            }
            if (menu.getListString() == "Connect by WPS Button")
            {
                menu.windowClr();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                Wps_run(true);
                menu.WiFi_Mode = WIFI_MODE_STA;
            }
            if (menu.getListString() == "Connect by WPS Pin Code")
            {
                menu.windowClr();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                Wps_run(false);
                menu.WiFi_Mode = WIFI_MODE_STA;
            }
            if (menu.getListString() == "WiFi OFF")
            {
                menu.windowClr();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                WiFi.disconnect();
                WiFi.mode(WIFI_MODE_NULL);
                menu.WiFi_Mode = WIFI_MODE_NULL;
                M5.Lcd.drawString("WiFi Turned OFF", 5, 50, 2);
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                menu.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));
                menu.showList();
            }
        }
        M5.update();
    }
    preferences.begin("WiFi", false);
    preferences.putInt("mode", (int)WiFi.getMode());
    preferences.end();
}

WifiSettingsClass::WifiSettingsClass()
{
    menu.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));
    M5.update();
}

WifiSettingsClass::~WifiSettingsClass()
{
    menu.show();
}