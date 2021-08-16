#include "VideoPlayer.h"

void VideoPlayerClass::Play(const char *fileName)
{
    M5.update();
    M5.Lcd.fillScreen(BLACK);
    file = My_SD.open(fileName);
    M5.Lcd.setSwapBytes(true);
    while(!M5.BtnB.wasPressed() && file.read(videoBuffer, 93960))
    {
        M5.Lcd.pushImage(15,36,290,162,(uint16_t*)videoBuffer);
        M5.update();
    }
    file.close();
    return;
}

VideoPlayerClass::VideoPlayerClass()
{
     videoBuffer = (uint8_t*)heap_caps_malloc(93960,MALLOC_CAP_DEFAULT);
}

VideoPlayerClass::~VideoPlayerClass()
{
    heap_caps_free(videoBuffer);
    menu.drawAppMenu(F("SD BROWSER"), F("EXIT"), F("OPEN"), F(">"));
    menu.showList();
}