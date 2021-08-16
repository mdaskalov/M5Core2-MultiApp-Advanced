#include <M5Core2.h>
#include "Wrappers.h"
#include "Resources.h"

const int version = 5;
unsigned long lastcheck = 0;
int SignalStrength = 0;
bool OtaRunning = false;

void setup()
{
	M5.begin();
	menu.setColorSchema(TFT_NAVY,TFT_NAVY,TFT_WHITE);

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

	//The main menu. Add main menu items here
	menu.addMenuItem(0, "APPLICATIONS", "<", "OK", ">", 1, Apps, appReturn);
	menu.addMenuItem(0, "SYSTEM", "<", "OK", ">", 2, System, appReturn);
	menu.addMenuItem(0, "ABOUT", "<", "OK", ">", -1, About, appAbout);
	menu.addMenuItem(0, "SLEEP/CHARGING", "<", "OK", ">", -1, Sleep, appSleep);

	menu.addMenuItem(1, "OSCILLOSCOPE", "<", "OK", ">", -1, Oscilloscope, appOscilloscope);
	menu.addMenuItem(1, "WEBRADIO", "<", "OK", ">", -1, WebRadio, appWebRadio);
	menu.addMenuItem(1, "WEATHER STATION", "<", "OK", ">", -1, WeatherStation, appWeatherStation);
	menu.addMenuItem(1, "WEBSERVER", "<", "OK", ">", -1, Webserver, appWebServer);
	menu.addMenuItem(1, "SD BROWSER", "<", "OK", ">", -1, Browser, appSdBrowser);
	menu.addMenuItem(1, "TOOLS", "<", "OK", ">", -1, Tools, appListTools);
	menu.addMenuItem(1, "GAMES", "<", "OK", ">", -1, Games, appGamesList);
	menu.addMenuItem(1, "RETURN", "<", "OK", ">", 0, Return, appReturn);

	menu.addMenuItem(2, "SYSTEM INFORMATIONS", "<", "OK", ">", -1, Sysinfo, appSysInfo);
	menu.addMenuItem(2, "WIFI CONNECTION", "<", "OK", ">", -1, WifiConn, appWiFiSetup);
	menu.addMenuItem(2, "DISPLAY BACKLIGHT", "<", "OK", ">", -1, Backlight, appCfgbrightness);
	menu.addMenuItem(2, "RETURN", "<", "OK", ">", 0, Return, appReturn);

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
