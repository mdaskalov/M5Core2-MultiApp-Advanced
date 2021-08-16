#pragma once

#include "Menu.h"

#include "ArduinoOTA.h"
#include <Update.h>
#include <HTTPClient.h>
#include "Preferences.h"
#include "AudioFileSourceSD.h"
#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

#define My_SD SD

#define FSS9 &FreeSans9pt7b
#define FSS12 &FreeSans12pt7b
#define FSS18 &FreeSans18pt7b
#define FSS24 &FreeSans24pt7b
#define FFS9B &FreeSansBold9pt7b

extern Menu menu;
extern Preferences preferences;
