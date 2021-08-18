#include "Wrappers.h"

// Wrapper funtctions for Apps classes instances calls

void appSleep()
{
    preferences.begin("WiFi", false);
    preferences.putInt("mode", (int)WiFi.getMode());
    preferences.end();
    //M5m.setWakeupButton(BUTTON_B_PIN);
    //M5m.powerOFF();
}

void appAbout()
{
    AboutClass AboutObj;
    AboutObj.Run();
}

void appCfgbrightness()
{
    CfgBrightnessClass BrightnessObj;
    BrightnessObj.Run();
}

void appOscilloscope()
{
    OscilloscopeClass OscilloscopeObj;
    OscilloscopeObj.Run();
}

void appSdBrowser()
{
    SdBrowserClass SdBrowserObj;
    SdBrowserObj.Run();
}

void appSysInfo()
{
    SysinfoClass SysinfoObj;
    SysinfoObj.Run();
}

void appWiFiSetup()
{
    WifiSettingsClass WifiSettingsObj;
    WifiSettingsObj.Run();
}

void appWebServer()
{
    WebServerClass WebServerObj;
    WebServerObj.Run();
}

void appWebRadio()
{
    WebRadioClass WebRadioObj;
    WebRadioObj.Run();
}

void appWeatherStation()
{
    WeatherStationClass WeatherStationObj;
    WeatherStationObj.Run();
}

void gameTetris()
{
    TetrisClass *TetrisObj = new TetrisClass();
    TetrisObj->Run();
    TetrisClass::DestroyInstance(TetrisObj);
}

void gameFlappyBird()
{
    FlappyBirdClass FlappyBirdObj;
    FlappyBirdObj.Run();
}

void gameSpaceShooter()
{
    SpaceShooterClass SpaceShooterObj;
    SpaceShooterObj.Run();
}