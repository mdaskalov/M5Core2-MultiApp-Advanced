#include "ToolsList.h"

void appListTools()
{
    menu.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));

    while (M5.BtnB.wasPressed())
    {
        M5.update();
    }
    menu.clearList();
    menu.setListCaption("TOOLS");
    menu.addList("FOXGRAPH");
    menu.addList("WIFI PACKET MONITOR");
    menu.addList("WIFI SCANNER");
    menu.addList("I2C SCANNER");
    menu.addList("DHT TEMP & HUMIDITY");
    menu.addList("STOPWATCH");
    menu.showList();

    while (!M5.BtnA.wasPressed())
    {
        if (M5.BtnC.wasPressed())
        {
            menu.nextList();
        }
        if (M5.BtnB.wasPressed())
        {
            if (menu.getListString() == "DHT TEMP & HUMIDITY")
            {
                DhtReaderClass DhtReaderObj;
                DhtReaderObj.Run();
            }
            if (menu.getListString() == "WIFI SCANNER")
            {
                WifiScannerClass WifiScannerObj;
                WifiScannerObj.Run();
            }
            if (menu.getListString() == "I2C SCANNER")
            {
               I2CScannerClass I2CScannerObj;
               I2CScannerObj.Run();
            }
            if (menu.getListString() == "WIFI PACKET MONITOR")
            {
               Monitor_run();
            }
            if (menu.getListString() == "STOPWATCH")
            {
                StopWatchClass StopWatchObj;
                StopWatchObj.Run();
            }
            if (menu.getListString() == "FOXGRAPH")
            {
                FoxGraphClass FoxGraphObj;
                FoxGraphObj.Run();
            }
        }
        M5.update();
    }
    menu.show();
}