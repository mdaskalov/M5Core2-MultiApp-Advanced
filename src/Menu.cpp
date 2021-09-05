#include "Menu.h"
#include "Preferences.h"

extern TFT_eSprite display;

void Menu::up()
{
  if (menuIDX < menuCount[levelIDX] - 1)
  {
    menuIDX++;
  }
  else
  {
    menuIDX = 0;
  }
  show();
}

void Menu::down()
{
  if (menuIDX > 0)
  {
    menuIDX--;
  }
  else
  {
    menuIDX = menuCount[levelIDX] - 1;
  }
  show();
}

void Menu::execute()
{
  if (menuList[levelIDX][menuIDX].gotoLevel == -1)
  {
    (*menuList[levelIDX][menuIDX].function)();
  }
  else
  {
    gotoLevel(menuList[levelIDX][menuIDX].gotoLevel);
  }
}

void Menu::windowClr()
{
  M5.Lcd.fillRect(0, titleHeight, display.width(), display.height() - titleHeight - btnHeight, TFT_BLACK);
}

void Menu::setColorSchema(unsigned int inmenucolor, unsigned int inwindowcolor, unsigned int intextcolor)
{
  menucolor = inmenucolor;
  windowcolor = inwindowcolor;
  menutextcolor = intextcolor;
}

void Menu::drawAppMenu(String inmenuttl, String inbtnAttl, String inbtnBttl, String inbtnCttl)
{
  drawMenu(inmenuttl, inbtnAttl, inbtnBttl, inbtnCttl, menucolor, windowcolor, NULL, menutextcolor);
  M5.Lcd.setTextColor(menutextcolor, windowcolor);
}

void Menu::gotoLevel(uint32_t inlevel)
{
  levelIDX = inlevel;
  menuIDX = 0;
  show();
}

unsigned int Menu::getrgb(uint8_t inred, uint8_t ingrn, uint8_t inblue)
{
  inred = map(inred, 0, 255, 0, 31);
  ingrn = map(ingrn, 0, 255, 0, 63);
  inblue = map(inblue, 0, 255, 0, 31);
  return inred << 11 | ingrn << 5 | inblue;
}

void Menu::addMenuItem(uint32_t levelID, const char *title, signed char goto_level, const char *img, void (*function)())
{
  addMenuItem(levelID, title, "<", "OK", ">", goto_level, img, function);
}

void Menu::addMenuItem(uint32_t levelID, const char *title, const char *btnA, const char *btnB,
                             const char *btnC, signed char goto_level, const char *img, void (*function)())
{
  uint32_t mCnt = menuCount[levelID];
  menuList[levelID] = (MenuCommandCallback *)realloc(menuList[levelID], (mCnt + 1) * sizeof(MenuCommandCallback));
  strncpy(menuList[levelID][mCnt].title, title, MENU_TITLE_MAX_SIZE);
  strncpy(menuList[levelID][mCnt].btnAtitle, btnA, BTN_TITLE_MAX_SIZE);
  strncpy(menuList[levelID][mCnt].btnBtitle, btnB, BTN_TITLE_MAX_SIZE);
  strncpy(menuList[levelID][mCnt].btnCtitle, btnC, BTN_TITLE_MAX_SIZE);
  menuList[levelID][mCnt].gotoLevel = goto_level;
  menuList[levelID][mCnt].MenuImg = img;
  menuList[levelID][mCnt].function = function;
  menuCount[levelID]++;
}

void Menu::show()
{
  M5.update();
  drawMenu(menuList[levelIDX][menuIDX].title, menuList[levelIDX][menuIDX].btnAtitle, menuList[levelIDX][menuIDX].btnBtitle,
           menuList[levelIDX][menuIDX].btnCtitle, menucolor, windowcolor, menuList[levelIDX][menuIDX].MenuImg, menutextcolor);
}

void Menu::clearList()
{
  list_count = 0;
  list_pages = 0;
  list_page = 0;
  list_lastpagelines = 0;
  list_idx = 0;
  list_labels.clear();
  list_labels.shrink_to_fit();
  listCaption = "";
}

unsigned int Menu::getListID()
{
  return list_idx;
}

String Menu::getListString()
{
  return list_labels[list_idx];
}

void Menu::nextList()
{
  if (list_idx < list_page * LIST_PAGE_LABELS + list_lines - 1)
  {
    list_idx++;
  }
  else
  {
    if (list_page < list_pages - 1)
    {
      list_page++;
    }
    else
    {
      list_page = 0;
    }
    list_idx = list_page * LIST_PAGE_LABELS;
  }
  showList();
}

void Menu::addList(String inStr)
{
  if (inStr.length() <= LIST_MAX_LABEL_SIZE && inStr.length() > 0)
  {
    list_labels.push_back(inStr);
    list_count++;
  }
  if (list_count > 0)
  {
    if (list_count > LIST_PAGE_LABELS)
    {
      list_lastpagelines = list_count % LIST_PAGE_LABELS;
      if (list_lastpagelines > 0)
      {
        list_pages = (list_count - list_lastpagelines) / LIST_PAGE_LABELS;
        list_pages++;
      }
      else
      {
        list_pages = list_count / LIST_PAGE_LABELS;
      }
    }
    else
    {
      list_pages = 1;
    }
  }
}

void Menu::setListCaption(String inCaption)
{
  listCaption = inCaption;
}

void Menu::btnRestore()
{
  M5.Lcd.setTextColor(menutextcolor);
  M5.Lcd.fillRoundRect(0, M5.Lcd.height() - 28, M5.Lcd.width(), 28, 5, 0x00);
  M5.Lcd.fillRoundRect(31, M5.Lcd.height() - 28, 60, 28, 5, menucolor);
  M5.Lcd.fillRoundRect(126, M5.Lcd.height() - 28, 60, 28, 5, menucolor);
  M5.Lcd.fillRoundRect(221, M5.Lcd.height() - 28, 60, 28, 5, menucolor);
  M5.Lcd.drawCentreString(lastBtnTittle[0], 31 + 30, M5.Lcd.height() - 28 + 6, FONT2);
  M5.Lcd.drawCentreString(lastBtnTittle[1], 126 + 30, M5.Lcd.height() - 28 + 6, FONT2);
  M5.Lcd.drawCentreString(lastBtnTittle[2], 221 + 30, M5.Lcd.height() - 28 + 6, FONT2);
  M5.Lcd.setTextColor(menutextcolor, windowcolor);
}

void Menu::HprogressBar(int x, int y, int w, int h, uint32_t color, uint8_t val, bool redraw)
{
  if (redraw)
  {
    M5.lcd.fillRoundRect(x + 1, y + 1, w, h - 2, 7, 0);
  }
  M5.lcd.drawRoundRect(x, y, w, h, 8, color);
  M5.lcd.fillRoundRect(x + 1, y + 1, w * (((float)val) / 100.0), h - 2, 7, color);
}

void Menu::VprogressBar(int x, int y, int w, int h, uint32_t color, uint8_t val, bool redraw)
{
  if (redraw)
  {
    M5.lcd.fillRoundRect(x + 1, y + 1, w - 1, h - 1, 10, 0);
  }
  M5.lcd.drawRoundRect(x, y, w, h, 10, color);
  M5.lcd.fillRoundRect(x + 1, y + (h - (h * (((float)val) / 100.0))), w - 2, h * (((float)val) / 100.0), 10, color);
}

void Menu::displayClr()
{
  display.fillRoundRect(0, titleHeight + 3, display.width(), display.height() - titleHeight - btnHeight - 4, 5, windowcolor);
}

void Menu::drawLabel(int32_t x, int32_t y, int32_t w, int32_t h, const String &title, unsigned int background, unsigned int color, uint8_t font, int32_t radius)
{
  display.fillRoundRect(x, y, w, h, radius, background);
  display.setTextColor(color);
  display.setTextDatum(CC_DATUM);
  display.drawString(title, x + (w / 2), y + (h / 2), font);
  display.setTextDatum(TL_DATUM);
}

void Menu::drawMenu(const String& inmenuttl, const String& inbtnAttl, const String& inbtnBttl, const String& inbtnCttl, unsigned int inmenucolor,
                          unsigned int inwindowcolor, const char *iMenuImg, unsigned int intxtcolor)
{
  lastBtnTittle[0] = inbtnAttl;
  lastBtnTittle[1] = inbtnBttl;
  lastBtnTittle[2] = inbtnCttl;

  drawLabel(0, 0, titleWidth, titleHeight, inmenuttl, inmenucolor, intxtcolor, FONT2);
  drawLabel(xBtnA, yBtn, btnWidth, btnHeight, inbtnAttl, inmenucolor, intxtcolor, FONT2);
  drawLabel(xBtnB, yBtn, btnWidth, btnHeight, inbtnBttl, inmenucolor, intxtcolor, FONT2);
  drawLabel(xBtnC, yBtn, btnWidth, btnHeight, inbtnCttl, inmenucolor, intxtcolor, FONT2);

  displayClr();
  if (iMenuImg != NULL)
  {
    display.drawJpg((uint8_t *)iMenuImg, (sizeof(iMenuImg) / sizeof(iMenuImg[0])), 0, 30);
  }
  display.pushSprite(0,0);
}

void Menu::showList()
{
  displayClr();
  unsigned int labelid = 0;
  display.setTextDatum(CC_DATUM);
  display.drawString(listCaption, display.width() / 2, 45, FONT2);
  display.setTextDatum(TL_DATUM);
  if ((list_page + 1) == list_pages)
  {
    if (list_lastpagelines == 0 && list_count >= LIST_PAGE_LABELS)
    {
      list_lines = LIST_PAGE_LABELS;
      for (uint32_t i = 0; i < LIST_PAGE_LABELS; i++)
      {
        labelid = i + (list_page * LIST_PAGE_LABELS);
        drawListItem(labelid, i);
      }
    }
    else
    {
      if (list_pages > 1)
      {
        list_lines = list_lastpagelines;
        for (uint32_t i = 0; i < list_lastpagelines; i++)
        {
          labelid = i + (list_page * LIST_PAGE_LABELS);
          drawListItem(labelid, i);
        }
      }
      else
      {
        list_lines = list_count;
        for (uint32_t i = 0; i < list_count; i++)
        {
          labelid = i + (list_page * LIST_PAGE_LABELS);
          drawListItem(labelid, i);
        }
      }
    }
  }
  else
  {
    list_lines = LIST_PAGE_LABELS;
    for (uint32_t i = 0; i < LIST_PAGE_LABELS; i++)
    {
      labelid = i + (list_page * LIST_PAGE_LABELS);
      drawListItem(labelid, i);
    }
  }
  display.pushSprite(0,0);
}

void Menu::drawListItem(uint32_t inIDX, uint32_t postIDX)
{
  if (inIDX == list_idx)
  {
    display.drawString(">", 3, 80 + (postIDX * 20), FONT2);
  }
  display.drawString(list_labels[inIDX], 15, 80 + (postIDX * 20), FONT2);
}

void Menu::initialize()
{
  titleWidth = display.width();
  titleHeight = 28;
  btnGap = 13;
  btnWidth = (display.width() - (4 * btnGap)) / 3;
  btnHeight = 28;
  xBtnA = btnGap;
  xBtnB = display.width() / 2 - btnWidth / 2;
  xBtnC = display.width() - btnWidth - btnGap;
  yBtn = display.height() - btnHeight;

	setColorSchema(TFT_NAVY,TFT_BLACK,TFT_WHITE);
  display.setTextSize(1);
}

Menu::Menu()
{
  initialize();
}