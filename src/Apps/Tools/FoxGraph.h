#pragma once
#include "globals.h"

class FoxGraphClass
{
  public:
    FoxGraphClass();
    ~FoxGraphClass();
    void Run();

  private:
    void ShowGraph();
    bool GetURLs(fs::FS &fs, const char *path);

};
