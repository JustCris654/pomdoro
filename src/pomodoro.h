#ifndef POMODORO_H_
#define POMODORO_H_

#include <raylib.h>

typedef struct {
  int h;
  int m;
  int s;
} Time;

typedef struct {
  int elapsedFromStart;
  bool isPomodoroRunning;
  bool plsExit;
  Time t;
} PomodoroState;

Vector2 textboxSizeForText(const char *text, int padding);
void *startPomodoroTimer(void *arg);
void drawTimer();
void drawPomodoroBtn(const char *text, int x_quarters, int y_quarters,
                     int padding, bool *clicked);
void drawPomodoroLbl(const char *text, int fontSize, int x_quarters,
                     int y_quarters, int padding);
void drawTotalElapsedTime(int totalElapsedTime);  

#endif
