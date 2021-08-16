#pragma once
#include "globals.h"

class VideoPlayerClass
{
private:
  uint8_t *videoBuffer = NULL;
  File file;

public:
  void Play(const char *fileName);
  VideoPlayerClass();
  ~VideoPlayerClass();
};
