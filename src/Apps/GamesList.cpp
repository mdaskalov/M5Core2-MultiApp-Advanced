#include "GamesList.h"

// the game list menu
void GamesListClass::Run()
{
    M5.update();
    menu.drawAppMenu(F("GAMES"), F("ESC"), F("SELECT"), F("LIST"));
    menu.clearList();
    menu.setListCaption("GAMES");

    // The list items Add new items to list to be displayed
    menu.addList("TETRIS");
    menu.addList("FLAPPY BIRD");
    menu.addList("ALIEN SHOOTER");
    menu.showList();

    while (!M5.BtnA.wasPressed())
    {
        if (M5.BtnC.wasPressed())
        {
            menu.nextList();
        }
        if (M5.BtnB.wasPressed())
        {
            if (menu.getListString() == "TETRIS")
            {
                M5.update();
                TetrisClass *TetrisObj = new TetrisClass();
                TetrisObj->Run();
                TetrisClass::DestroyInstance(TetrisObj);
            }
            if (menu.getListString() == "FLAPPY BIRD")
            {
                M5.update();
                FlappyBirdClass FlappyBirdObj;
                FlappyBirdObj.Run();
            }
            if (menu.getListString() == "ALIEN SHOOTER")
            {
                M5.update();
                // The game object with member call
                SpaceShooterClass SpaceShooterObj;
                SpaceShooterObj.Run();
            }
        }
        M5.update();
    }
}

GamesListClass::GamesListClass()
{
}

GamesListClass::~GamesListClass()
{
    menu.show();
}