#pragma once

#include <M5Core2.h>

#define MENU_TITLE_MAX_SIZE 24
#define BTN_TITLE_MAX_SIZE 6
#define MAX_SUBMENUS 4

#define LIST_MAX_LABEL_SIZE 32
#define LIST_PAGE_LABELS 6

class Menu
{
  public:
    String lastBtnTittle[3];
    int WiFi_Mode;
    int vol, old_vol;

    void up();
    void down();
    void execute();
    void windowClr();
    void setColorSchema(unsigned int inmenucolor, unsigned int inwindowcolor, unsigned int intextcolor);
    void drawAppMenu(String inmenuttl, String inbtnAttl, String inbtnBttl, String inbtnCttl);
    void gotoLevel(uint32_t inlevel);
    unsigned int getrgb(uint8_t inred, uint8_t ingrn, uint8_t inblue);
    void addMenuItem(uint32_t levelID, const char *title, signed char goto_level, const char *img, void (*function)());
    void addMenuItem(uint32_t levelID, const char *title, const char *btnA, const char *btnB, const char *btnC,
                    signed char goto_level, const char *img, void (*function)());
    void show();
    void showList();
    void clearList();
    unsigned int getListID();
    String getListString();
    void nextList();
    void addList(String inLabel);
    void setListCaption(String inCaption);
    void btnRestore();

    void HprogressBar(int x, int y, int w, int h, uint32_t color, uint8_t val, bool redraw);
    void VprogressBar(int x, int y, int w, int h, uint32_t color, uint8_t val, bool redraw);

    Menu();

  private:
    String listCaption;
    void drawListItem(uint32_t inIDX, uint32_t postIDX);
    void drawMenu(String inmenuttl, String inbtnAttl, String inbtnBttl, String inbtnCttl, unsigned int inmenucolor,
                  unsigned int inwindowcolor, const char *iMenuImg, unsigned int intxtcolor);
    struct MenuCommandCallback
    {
      char title[MENU_TITLE_MAX_SIZE + 1];
      char btnAtitle[BTN_TITLE_MAX_SIZE + 1];
      char btnBtitle[BTN_TITLE_MAX_SIZE + 1];
      char btnCtitle[BTN_TITLE_MAX_SIZE + 1];
      signed char gotoLevel;
      const char *MenuImg;
      void (*function)();
    };
    std::vector<String> list_labels;
    uint32_t list_lastpagelines;
    uint32_t list_count;
    uint32_t list_pages;
    uint32_t list_page;
    unsigned int list_idx;
    uint32_t list_lines;

    MenuCommandCallback *menuList[MAX_SUBMENUS];
    uint32_t menuIDX;
    uint32_t levelIDX;
    uint32_t menuCount[MAX_SUBMENUS];
    unsigned int menucolor;
    unsigned int windowcolor;
    unsigned int menutextcolor;
};