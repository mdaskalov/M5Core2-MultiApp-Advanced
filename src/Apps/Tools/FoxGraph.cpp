#include "FoxGraph.h"
#include <HTTPClient.h>

std::vector<String> Name;
std::vector<String> Link;
int curURL = 0;
unsigned long oldtime=0;

bool FoxGraphClass::GetURLs(fs::FS &fs, const char *path)
{
    String URLList, name, link = "";

	File sdfile = fs.open(path);
	if (!sdfile)
	{
		return false;
	}

    if (sdfile.available())
	{
		URLList = sdfile.readStringUntil(EOF);
		while (URLList != "\0")
		{
			int t = URLList.indexOf("=");
			name = URLList.substring(0, t);
			URLList.replace(String(name + "="), "");
			int r = URLList.indexOf(";");
			link = URLList.substring(0, r);
			URLList.replace(String(link + ";"), "");
			if (URLList != "\0")
			{
				URLList = URLList.substring(2);
			}
			Name.push_back(name);
			Link.push_back(link);
		}
    }
    sdfile.close();
    return true;
}

void FoxGraphClass::ShowGraph()
{
    int MAX_X=315;
    WiFiClient *client;
    HTTPClient http;
    int httpCode = 0;
    int size=0;
    int valcount=0;
    float valmax=-1000;
    float valmin=1000;
    float valArray[300];
    String tmpst;
    bool debug=false;

    M5.Lcd.cursor_x=0;
    M5.Lcd.cursor_y=0;

    http.begin(Link[curURL]);
    httpCode = http.GET();
    if (httpCode > 0)
    {
        client = http.getStreamPtr();
        while (client->connected())
        {
            int n=0;
            while ((size = client->available()) > 0)
            {
                tmpst = client->readStringUntil(',');
                if (tmpst.substring(0,8)=="\"value\":") {
                    tmpst = tmpst.substring(8);
                    tmpst.replace("}","");
                    tmpst.replace("]","");
                    valArray[n]=tmpst.toFloat();
//                    if (debug) {
//                        M5.Lcd.println(valArray[n]);
//                    }
                    if (valArray[n]>valmax) valmax=valArray[n];
                    if (valArray[n]<valmin) valmin=valArray[n];
                    if (valcount<n) valcount=n;
                    n++;
                }
            }
            break;
        }
        client->stop();
    }
    http.end();

    float coef=180/abs(valmax-valmin);

    if (debug) {
        M5.Lcd.println(valmin);
        M5.Lcd.println(valmax);
        M5.Lcd.println(coef);
    }

    for (int i=0;i<316;i++){
        M5.Lcd.drawLine(i,30,i,210,BLACK);
    }

    for (int i=0;i<valcount;i++){
        M5.Lcd.drawLine(MAX_X-i, 210, MAX_X-i, 30-(valArray[valcount-i]-valmax)*coef, GREEN);
    }

    M5.Lcd.setTextColor(RED);

	M5.Lcd.drawString("Max", 0, 30, 2);
	M5.Lcd.drawString(String(valmax), 26, 30, 2);

    M5.Lcd.drawString("Cur", 0, 120, 2);
	M5.Lcd.drawString(String(valArray[valcount]), 26, 120, 2);

    M5.Lcd.drawString("Min", 0, 190, 2);
	M5.Lcd.drawString(String(valmin), 26, 190, 2);

    M5.Lcd.setTextColor(BLUE);
    M5.Lcd.drawString(Name[curURL],150,120,2);

}

void FoxGraphClass::Run()
{
    if (GetURLs(My_SD, "/FoxGraph.txt")){
        M5.update();
        menu.drawAppMenu(F("FOXGRAPH"), F("Prev"), F("ESC"), F("Next"));
        ShowGraph();
        oldtime=millis();
        while (!M5.BtnB.wasPressed())
        {
            M5.update();
            if (millis() - oldtime > 15*1000) {
				if (curURL < (unsigned int)(Link.size() - 1))
				{
					curURL++;
				}
				else
				{
					curURL = 0;
				}
//                M5.Lcd.clear();
//                menu.drawAppMenu(F("FOXGRAPH"), F("Prev"), F("ESC"), F("Next"));
                ShowGraph();
                oldtime=millis();
            }
            if (M5.BtnA.wasPressed())
            {
				if (curURL > 0 )
				{
					curURL--;
				}else{
                    curURL=(unsigned int)(Link.size() - 1);
                }
                M5.Lcd.clear();
                menu.drawAppMenu(F("FOXGRAPH"), F("Prev"), F("ESC"), F("Next"));
                ShowGraph();
            }
            if (M5.BtnC.wasPressed())
            {
				if (curURL < (unsigned int)(Link.size() - 1))
				{
					curURL++;
				}
				else
				{
					curURL = 0;
				}
                M5.Lcd.clear();
                menu.drawAppMenu(F("FOXGRAPH"), F("Prev"), F("ESC"), F("Next"));
                ShowGraph();
            }
        }
    }
}

FoxGraphClass::FoxGraphClass()
{
}

FoxGraphClass::~FoxGraphClass()
{
    M5.Lcd.fillScreen(0);
    menu.drawAppMenu(F("FOXGRAPH"), F("Esc"), F("Select"), F("List"));
    menu.showList();
}