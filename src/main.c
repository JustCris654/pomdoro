#include <pthread.h>
#include <raylib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

// suppress raygui warnings
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#define W_WIDTH 1200
#define W_HEIGHT 800

Vector2 textboxSizeForText(const char *text, int padding);
void *startPomodoroTimer(void *arg);
void drawTimer();
void drawPomodoroBtn(const char *text, int x_quarters, int y_quarters,
                     int padding, bool *clicked);

typedef struct {
  int m;
  int s;
} Timer;

typedef struct {
  int elapsedFromStart;
  bool isPomodoroRunning;
  bool plsExit;
  Timer t;
} PomodoroState;

pthread_mutex_t mtx;

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  bool startPomodoro = false;
  bool stopPomodoro = false;
  PomodoroState pState = {0, 0};
  pthread_t timerThread;

  pthread_mutex_init(&mtx, NULL);

  InitWindow(W_WIDTH, W_HEIGHT, "Example");
  SetTargetFPS(60);

  // set font size for 4k display
  GuiSetStyle(DEFAULT, TEXT_SIZE, 48);

  while (!WindowShouldClose()) {
    BeginDrawing();
    {
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      pthread_mutex_lock(&mtx);

      if (!pState.isPomodoroRunning) {
        drawPomodoroBtn("#150#Start pomodoro", 2, 2, 150, &startPomodoro);
      } else {
        // DRAW TIMER
        GuiSetStyle(DEFAULT, TEXT_SIZE, 96);
        Vector2 textboxSize =
            textboxSizeForText("000000", 10); // it is always 5 characters
        GuiLabel((Rectangle){floor(W_WIDTH / 2.0) - textboxSize.x / 2,
                             floor(W_HEIGHT / 2.0) - textboxSize.y / 2,
                             textboxSize.x, textboxSize.y},
                 TextFormat("#219#%02d:%02d", pState.t.m, pState.t.s));
        GuiSetStyle(DEFAULT, TEXT_SIZE, 48);

        // DRAW STOP BUTTON
        drawPomodoroBtn("#211#Stop", 3, 3, 10, &stopPomodoro);
      }

      if (stopPomodoro) {
        pState.plsExit = true;
        stopPomodoro = false;
      }
      pthread_mutex_unlock(&mtx);

      if (startPomodoro) {
        if (pthread_create(&timerThread, NULL, startPomodoroTimer, &pState) !=
            0) {
          perror("pthread_create");
          exit(1);
        }
        startPomodoro = false;
      }
    }
    EndDrawing();
  }

  pthread_mutex_destroy(&mtx);
  CloseWindow();

  return 0;
}

void *startPomodoroTimer(void *arg) {

  PomodoroState *pState = arg;

  pthread_mutex_lock(&mtx);
  pState->isPomodoroRunning = true;
  pthread_mutex_unlock(&mtx);

  int seconds = 25 * 60;

  while (seconds > 0) {
    pthread_mutex_lock(&mtx);

    if (pState->plsExit) {
      pState->isPomodoroRunning = false;
      pState->plsExit = false;
      pState->elapsedFromStart += 25 * 60 - seconds;
      pthread_mutex_unlock(&mtx);
      return NULL;
    }

    pState->t.m = (seconds % (60 * 60)) / 60;
    pState->t.s = seconds % 60;

    /* printf("\r%02d:%02d", pState->t.m, pState->t.s); */

    /* fflush(stdout); */
    pthread_mutex_unlock(&mtx);

    clock_t stop = clock() + CLOCKS_PER_SEC;
    while (clock() < stop) {
    }

    seconds -= 1;
  }

  pthread_mutex_lock(&mtx);
  pState->isPomodoroRunning = false;
  pState->plsExit = false;
  pState->elapsedFromStart += 25 * 60;
  pthread_mutex_unlock(&mtx);

  return NULL;
}

void drawPomodoroBtn(const char *text, int x_quarters, int y_quarters,
                     int padding, bool *clicked) {
  Vector2 textboxSize = textboxSizeForText(text, padding);
  if (GuiButton(
          (Rectangle){floor((W_WIDTH / 4.0) * x_quarters) - textboxSize.x / 2,
                      floor((W_HEIGHT / 4.0) * y_quarters) - textboxSize.y / 2,
                      textboxSize.x, textboxSize.y},
          text)) {
    *clicked = true;
  }
}

Vector2 textboxSizeForText(const char *text, int padding) {
  Vector2 textSize =
      MeasureTextEx(GuiGetFont(), text, (float)GuiGetStyle(DEFAULT, TEXT_SIZE),
                    (float)GuiGetStyle(DEFAULT, TEXT_SPACING));
  // add padding
  textSize.x += padding;
  textSize.y += padding;

  return textSize;
}
