#include <M5Core2.h>
#include "Wrappers.h"
#include "Resources.h"

const int version = 5;
unsigned long lastcheck = 0;
int SignalStrength = 0;
bool OtaRunning = false;

TFT_eSprite display = TFT_eSprite(&M5.Lcd);

Menu menu;
Preferences preferences;

void setup()
{
	M5.begin();

	M5.Lcd.setRotation(1);
  display.createSprite(M5.Lcd.width(), M5.Lcd.height());
	menu.initialize();

	dacWrite(25, 0); // Speaker OFF
	preferences.begin("WiFi", false);
	menu.WiFi_Mode = preferences.getInt("mode", 0);
	preferences.end();
	WiFi.mode(wifi_mode_t(menu.WiFi_Mode));
	if (menu.WiFi_Mode > 0)
	{
		WiFi.begin();
	}

	preferences.begin("Brightness", false);
	M5.Lcd.setBrightness(preferences.getUShort("light", 95));
	preferences.end();

	// MAIN
	menu.addMenuItem(0, "APPLICATIONS", 1, Apps, NULL);
	menu.addMenuItem(0, "GAMES", 2, Games, NULL);
	menu.addMenuItem(0, "SYSTEM", 3, System, NULL);
	menu.addMenuItem(0, "ABOUT", -1, About, appAbout);
	menu.addMenuItem(0, "SLEEP/CHARGING", -1, Sleep, appSleep);

  // APPLICATIONS
	menu.addMenuItem(1, "OSCILLOSCOPE", -1, Oscilloscope, appOscilloscope);
	menu.addMenuItem(1, "WEBRADIO", -1, WebRadio, appWebRadio);
	menu.addMenuItem(1, "WEATHER STATION", -1, WeatherStation, appWeatherStation);
	menu.addMenuItem(1, "WEBSERVER", -1, Webserver, appWebServer);
	menu.addMenuItem(1, "SD BROWSER", -1, Browser, appSdBrowser);
	menu.addMenuItem(1, "TOOLS", -1, Tools, appListTools);
	menu.addMenuItem(1, "RETURN", 0, Return, NULL);

  // GAMES
	menu.addMenuItem(2, "TETRIS", -1, NULL, gameTetris);
	menu.addMenuItem(2, "FLAPPY BIRD", -1, NULL, gameFlappyBird);
	menu.addMenuItem(2, "SPACE SHOOTER", -1, NULL, gameSpaceShooter);
	menu.addMenuItem(2, "RETURN", 0, Return, NULL);

  // SYSTEM
	menu.addMenuItem(3, "SYSTEM INFORMATIONS", -1, Sysinfo, appSysInfo);
	menu.addMenuItem(3, "WIFI CONNECTION", -1, WifiConn, appWiFiSetup);
	menu.addMenuItem(3, "DISPLAY BACKLIGHT", -1, Backlight, appCfgbrightness);
	menu.addMenuItem(3, "RETURN", 0, Return, NULL);

	menu.show();
}

void loop()
{
	unsigned long now = millis();
	if (now - lastcheck >= 1000)
	{
		menu.WiFi_Mode = WiFi.getMode();
		if (menu.WiFi_Mode == WIFI_MODE_STA && WiFi.isConnected())
		{
			M5.Lcd.setTextColor(WHITE, 15);
			SignalStrength = map(100 + WiFi.RSSI(), 5, 90, 0, 100);
			M5.Lcd.drawRightString("WiFi: " + String(SignalStrength) + " %", 310, 5, 2);
			if (!OtaRunning)
			{
				appOta();
				OtaRunning = true;
			}
		}
		else if (menu.WiFi_Mode == WIFI_MODE_APSTA)
		{
			M5.Lcd.setTextColor(WHITE, 15);
			M5.Lcd.drawRightString("Clients: " + String(WiFi.softAPgetStationNum()), 300, 5, 2);
		}
		else if (menu.WiFi_Mode == WIFI_MODE_NULL)
		{
			M5.Lcd.setTextColor(WHITE, 15);
			M5.Lcd.drawRightString("Wifi OFF", 310, 5, 2);
		}
		lastcheck = now;
	}

	M5.update();
	if (OtaRunning)
	{
		ArduinoOTA.handle();
	}
	if (M5.BtnA.wasPressed())
	{
		menu.down();
	}
	if (M5.BtnB.wasPressed())
	{
		menu.execute();
	}
	if (M5.BtnC.wasPressed())
	{
		menu.up();
	}
}
