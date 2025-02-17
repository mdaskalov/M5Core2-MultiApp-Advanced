#include "StopWatch.h"

void StopWatchClass::Run()
{
    M5.update();
    menu.drawAppMenu(F("STOPWATCH"), F("S/S"), F("ESC"), F("RES"));

    M5.Lcd.drawString(F("MIN"), 40, 120, 2);
    M5.Lcd.drawString(F("SEC"), 170, 120, 2);
    M5.Lcd.drawFloat(tmp_sec, 1, 210, 100, 6);
    M5.Lcd.drawNumber(tmp_min, 80, 100, 6);

    while (!M5.BtnB.wasPressed())
    {
        M5.update();
        if (M5.BtnC.wasPressed())
        {
            tmp_sec = 0;
            tmp_min = 0;
            menu.windowClr();
            M5.Lcd.drawString(F("MIN"), 40, 120, 2);
            M5.Lcd.drawString(F("SEC"), 170, 120, 2);
            M5.Lcd.drawFloat(tmp_sec, 1, 210, 100, 6);
            M5.Lcd.drawNumber(tmp_min, 80, 100, 6);
        }
        if (tmp_run)
        {
            if (M5.BtnA.wasPressed())
            {
                tmp_run = LOW;
            }
            if (millis() - tmp_tmr > 100)
            {
                tmp_tmr = millis();
                tmp_sec = tmp_sec + 0.1;
                if (tmp_sec > 59.9)
                {
                    tmp_sec = 0;
                    tmp_min++;
                    menu.windowClr();
                    M5.Lcd.drawString(F("MIN"), 40, 120, 2);
                    M5.Lcd.drawString(F("SEC"), 170, 120, 2);
                    M5.Lcd.drawFloat(tmp_sec, 1, 210, 100, 6);
                    M5.Lcd.drawNumber(tmp_min, 80, 100, 6);
                }
                M5.Lcd.drawFloat(tmp_sec, 1, 210, 100, 6);
                M5.Lcd.drawNumber(tmp_min, 80, 100, 6);
            }
        }
        else
        {
            if (M5.BtnA.wasPressed())
            {
                tmp_run = HIGH;
            }
        }
    }
}

StopWatchClass::StopWatchClass()
{
}

StopWatchClass::~StopWatchClass()
{
    M5.Lcd.fillScreen(0);
    menu.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));
    menu.showList();
}