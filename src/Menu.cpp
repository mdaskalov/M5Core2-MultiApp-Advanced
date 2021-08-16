#include "Menu.h"
#include "Preferences.h"

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
    GoToLevel(menuList[levelIDX][menuIDX].gotoLevel);
  }
}

void Menu::windowClr()
{
  M5.Lcd.fillRoundRect(0, 29, M5.Lcd.width(), M5.Lcd.height() - 28 - 28, 5, windowcolor);
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

void Menu::GoToLevel(uint32_t inlevel)
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

void Menu::addMenuItem(uint32_t levelID, const char *menu_title, const char *btnA_title, const char *btnB_title,
                             const char *btnC_title, signed char goto_level, const char *Menu_Img, void (*function)())
{
  uint32_t mCnt = menuCount[levelID];
  menuList[levelID] = (MenuCommandCallback *)realloc(menuList[levelID], (mCnt + 1) * sizeof(MenuCommandCallback));
  strncpy(menuList[levelID][mCnt].title, menu_title, MENU_TITLE_MAX_SIZE);
  strncpy(menuList[levelID][mCnt].btnAtitle, btnA_title, BTN_TITLE_MAX_SIZE);
  strncpy(menuList[levelID][mCnt].btnBtitle, btnB_title, BTN_TITLE_MAX_SIZE);
  strncpy(menuList[levelID][mCnt].btnCtitle, btnC_title, BTN_TITLE_MAX_SIZE);
  menuList[levelID][mCnt].gotoLevel = goto_level;
  menuList[levelID][mCnt].MenuImg = Menu_Img;
  menuList[levelID][mCnt].function = function;
  menuCount[levelID]++;
}

void Menu::show()
{
  drawMenu(menuList[levelIDX][menuIDX].title, menuList[levelIDX][menuIDX].btnAtitle, menuList[levelIDX][menuIDX].btnBtitle,
           menuList[levelIDX][menuIDX].btnCtitle, menucolor, windowcolor, menuList[levelIDX][menuIDX].MenuImg, menutextcolor);
}

void Menu::showList()
{
  windowClr();
  unsigned int labelid = 0;
  M5.Lcd.drawCentreString(listCaption, M5.Lcd.width() / 2, 45, 2);
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
  M5.Lcd.drawCentreString(lastBtnTittle[0], 31 + 30, M5.Lcd.height() - 28 + 6, 2);
  M5.Lcd.drawCentreString(lastBtnTittle[1], 126 + 30, M5.Lcd.height() - 28 + 6, 2);
  M5.Lcd.drawCentreString(lastBtnTittle[2], 221 + 30, M5.Lcd.height() - 28 + 6, 2);
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

void Menu::drawMenu(String inmenuttl, String inbtnAttl, String inbtnBttl, String inbtnCttl, unsigned int inmenucolor,
                          unsigned int inwindowcolor, const char *iMenuImg, unsigned int intxtcolor)
{
  lastBtnTittle[0] = inbtnAttl;
  lastBtnTittle[1] = inbtnBttl;
  lastBtnTittle[2] = inbtnCttl;
  M5.Lcd.fillRoundRect(31, M5.Lcd.height() - 28, 60, 28, 5, inmenucolor);
  M5.Lcd.fillRoundRect(126, M5.Lcd.height() - 28, 60, 28, 5, inmenucolor);
  M5.Lcd.fillRoundRect(221, M5.Lcd.height() - 28, 60, 28, 5, inmenucolor);
  M5.Lcd.fillRoundRect(0, 0, M5.Lcd.width(), 28, 5, inmenucolor);
  M5.Lcd.fillRoundRect(0, 29, M5.Lcd.width(), M5.Lcd.height() - 28 - 28, 5, inwindowcolor);
  if (iMenuImg != NULL)
  {
    M5.Lcd.drawJpg((uint8_t *)iMenuImg, (sizeof(iMenuImg) / sizeof(iMenuImg[0])), 0, 30);
  }

  M5.Lcd.setTextColor(intxtcolor);
  M5.Lcd.drawCentreString(inmenuttl, M5.Lcd.width() / 2, 6, 2);

  M5.Lcd.drawCentreString(inbtnAttl, 31 + 30, M5.Lcd.height() - 28 + 6, 2);
  M5.Lcd.drawCentreString(inbtnBttl, 126 + 30, M5.Lcd.height() - 28 + 6, 2);
  M5.Lcd.drawCentreString(inbtnCttl, 221 + 30, M5.Lcd.height() - 28 + 6, 2);
}

void Menu::drawListItem(uint32_t inIDX, uint32_t postIDX)
{
  if (inIDX == list_idx)
  {
    M5.Lcd.drawString(list_labels[inIDX], 15, 80 + (postIDX * 20), 2);
    M5.Lcd.drawString(">", 3, 80 + (postIDX * 20), 2);
  }
  else
  {
    M5.Lcd.drawString(list_labels[inIDX], 15, 80 + (postIDX * 20), 2);
  }
}

Menu menu;
Preferences preferences;