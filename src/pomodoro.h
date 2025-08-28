#ifndef POMODORO_H_
#define POMODORO_H_

#include <miniaudio.h>
#include <raylib.h>
#include <stdio.h>


const char *GOSPEL_PATH = "./assets/gospel.mp3";
const char *LETS_GET_IT_ON_PATH = "./assets/lets-get-it-on.mp3";

typedef struct {
  int h;
  int m;
  int s;
} Time;

typedef struct {
  int elapsedFromStart;
  bool isPomodoroRunning;
  bool plsExit;
  bool playEndSound;
  bool playStartSound;
  Time t;
} PomodoroState;

typedef enum { GOSPEL, LETS_GET_IT_ON } PlayableSound;

Vector2 textboxSizeForText(const char *text, int padding);
void *startPomodoroTimer(void *arg);
void drawTimer();
void drawPomodoroBtn(const char *text, int x_quarters, int y_quarters,
                     int padding, bool *clicked);
void drawPomodoroLbl(const char *text, int fontSize, int x_quarters,
                     int y_quarters, int padding);
void drawTotalElapsedTime(int totalElapsedTime);
int initSoundPlay(ma_engine *soundEngine);
const char *getSoundPath(PlayableSound sound);
void playSound(PlayableSound sound, ma_engine *soundEngine);

int initSoundPlay(ma_engine *soundEngine) {
  ma_result result;

  result = ma_engine_init(NULL, soundEngine);
  if (result != MA_SUCCESS) {
    perror("Cannot initialize sound engine");
    return -1;
  }

  return 0;
}

const char *getSoundPath(PlayableSound sound) {
  switch (sound) {
  case GOSPEL:
    return GOSPEL_PATH;
    break;
  case LETS_GET_IT_ON:
    return LETS_GET_IT_ON_PATH;
    break;
  default:
    return GOSPEL_PATH;
  }
}

#endif
