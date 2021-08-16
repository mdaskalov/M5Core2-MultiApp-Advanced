#include "CfgBrightness.h"

void CfgBrightnessClass::Run()
{
    preferences.begin("Brightness", false);
    tmp_bright = preferences.getUShort("light", 95);
    tmp_lbright = 0;

    while (!M5.BtnB.wasPressed())
    {
        if (M5.BtnA.wasPressed() && tmp_bright >= 16)
        {
            tmp_bright = tmp_bright - 16;
        }
        if (M5.BtnC.wasPressed() && tmp_bright <= 239)
        {
            tmp_bright = tmp_bright + 16;
        }
        if (tmp_lbright != tmp_bright)
        {
            tmp_lbright = tmp_bright;
            preferences.putUShort("light", tmp_lbright);
            M5.Lcd.setBrightness(tmp_lbright);
            menu.windowClr();
            val = map(tmp_lbright, 16, 255, 0, 100);
            menu.HprogressBar(40, 110, 240, 30, YELLOW, val, true);
        }
        M5.update();
    }
    preferences.end();
}

CfgBrightnessClass::CfgBrightnessClass()
{
    menu.drawAppMenu(F("DISPLAY BRIGHTNESS"), F("-"), F("OK"), F("+"));
    M5.update();
}

CfgBrightnessClass::~CfgBrightnessClass()
{
    menu.show();
}
