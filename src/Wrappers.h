#pragma once
#include "Apps/About.h"
#include "Apps/CfgBrightness.h"
#include "Apps/Oscilloscope.h"
#include "Apps/SdBrowser.h"
#include "Apps/Sysinfo.h"
#include "Apps/WiFiSettings.h"
#include "Apps/MyWebServer.h"
#include "Apps/ToolsList.h"
#include "Apps/Tools/appOta.h"
#include "Apps/WebRadio.h"
#include "Apps/WeatherStation.h"
#include "Apps/Games/FlappyBird.h"
#include "Apps/Games/SpaceShooter.h"
#include "Apps/Games/Tetris.h"

void appSleep();
void appAbout();
void appCfgbrightness();
void appOscilloscope();
void appSdBrowser();
void appSysInfo();
void appWiFiSetup();
void appWebServer();
void appGamesList();
void appWebRadio();
void appWeatherStation();

void gameTetris();
void gameFlappyBird();
void gameSpaceShooter();

void appOta();