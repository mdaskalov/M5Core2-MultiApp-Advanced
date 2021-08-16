#include "SdBrowser.h"

void SdBrowserClass::listDir(fs::FS &fs, String dirName, int levels)
{
    File root = fs.open(dirName);
    if (!root)
    {
        return;
    }
    if (!root.isDirectory())
    {
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (!strstr((file.name()), ignored))
        {
            if (file.isDirectory())
            {
                if (levels)
                {
                    listDir(fs, file.name(), levels - 1);
                }
            }
            else
            {
                fileVector.push_back(fileinfo);
                String fileName = file.name();
                fileVector[appsCount].fileName = fileName;
                fileVector[appsCount].fileSize = file.size();
                appsCount++;
            }
        }
        file = root.openNextFile();
    }
    file.close();
}

void SdBrowserClass::aSortFiles()
{
    bool swapped;
    String name1, name2;
    do
    {
        swapped = false;
        for (int i = 0; i < appsCount - 1; i++)
        {
            name1 = String(fileVector[i].fileName[0]);
            name2 = String(fileVector[i + 1].fileName[0]);
            if (name1 == name2)
            {
                name1 = String(fileVector[i].fileName[1]);
                name2 = String(fileVector[i + 1].fileName[1]);
                if (name1 == name2)
                {
                    name1 = String(fileVector[i].fileName[2]);
                    name2 = String(fileVector[i + 1].fileName[2]);
                }
            }

            if (name1 > name2)
            {
                fileinfo = fileVector[i];
                fileVector[i] = fileVector[i + 1];
                fileVector[i + 1] = fileinfo;
                swapped = true;
            }
        }
    } while (swapped);
}

void SdBrowserClass::buildMyMenu()
{
    menu.clearList();
    menu.setListCaption("Files");
    for (int i = 0; i < appsCount; i++)
    {
        String shortName = fileVector[i].fileName.substring(1);
        menu.addList(shortName);
    }
}

void SdBrowserClass::Run()
{
    appsCount = 0;
    listDir(My_SD, "/", levels);
    aSortFiles();
    buildMyMenu();
    menu.showList();
    while (!M5.BtnA.wasPressed())
    {
        if (M5.BtnC.wasPressed())
        {
            if (repaint)
            {
                M5.Lcd.clear();
                menu.drawAppMenu(F("SD BROWSER"), F("EXIT"), F("OPEN"), F(">"));
                menu.btnRestore();
            }
            repaint = false;
            menu.nextList();
        }
        int MenuID = menu.getListID();

        if (M5.BtnB.wasPressed())
        {
            String FileName = fileVector[MenuID].fileName;
            if (FileName.endsWith(".jpg"))
            {
                M5.Lcd.clear();
                if (fileVector[MenuID].fileSize > 100000UL)
                {
                    M5.Lcd.drawJpgFile(My_SD, FileName.c_str(), 0, 0, 0, 0, 0, 0, JPEG_DIV_8);
                }
                else if (fileVector[MenuID].fileSize > 70000UL)
                {
                    M5.Lcd.drawJpgFile(My_SD, FileName.c_str(), 0, 0, 0, 0, 0, 0, JPEG_DIV_4);
                }
                else if (fileVector[MenuID].fileSize > 50000)
                {
                    M5.Lcd.drawJpgFile(My_SD, FileName.c_str(), 0, 0, 0, 0, 0, 0, JPEG_DIV_2);
                }
                else
                {
                    M5.Lcd.drawJpgFile(My_SD, FileName.c_str());
                }
                repaint = true;
                menu.btnRestore();
            }
            else if (FileName.endsWith(".mp3"))
            {
                Mp3PlayerClass Mp3PlayerObj;
                Mp3PlayerObj.Play(&FileName);
            }
            else if (FileName.endsWith(".mov"))
            {
                VideoPlayerClass VideoPlayerObj;
                VideoPlayerObj.Play(FileName.c_str());
            }
            else if (!inmenu)
            {
                inmenu = true;
                menu.windowClr();
                M5.Lcd.drawCentreString("File Name: " + fileVector[MenuID].fileName, M5.Lcd.width() / 2, (M5.Lcd.height() / 2) - 10, 2);
                M5.Lcd.drawCentreString("File Size: " + String(fileVector[MenuID].fileSize), M5.Lcd.width() / 2, (M5.Lcd.height() / 2) + 10, 2);
            }
            else
            {
                inmenu = false;
                menu.showList();
            }
        }
        M5.update();
    }
}

SdBrowserClass::SdBrowserClass()
{
    M5.update();
    menu.drawAppMenu(F("SD BROWSER"), F("EXIT"), F("OPEN"), F(">"));
}

SdBrowserClass::~SdBrowserClass()
{
    fileVector.clear();
    fileVector.shrink_to_fit();
    menu.clearList();
    menu.show();
}