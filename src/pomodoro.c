#include "pomodoro.h"

#include <pthread.h>
#include <raylib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

// suppress raygui warnings
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#define W_WIDTH 1200
#define W_HEIGHT 800

pthread_mutex_t mtx;

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  bool startPomodoro = false;
  bool stopPomodoro = false;
  PomodoroState pState = {0, 0, 0,0,0, {0, 0, 0}};
  pthread_t timerThread;
  ma_engine soundEngine;

  pthread_mutex_init(&mtx, NULL);
  initSoundPlay(&soundEngine);

  InitWindow(W_WIDTH, W_HEIGHT, "Example");
  SetTargetFPS(60);

  // set font size for 4k display
  GuiSetStyle(DEFAULT, TEXT_SIZE, 48);

  while (!WindowShouldClose()) {
    BeginDrawing();
    {
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      pthread_mutex_lock(&mtx);

      if (pState.playEndSound) {
        playSound(GOSPEL, &soundEngine);
        pState.playEndSound = false;
      }

      int totalElapTime = pState.elapsedFromStart;

      if (!pState.isPomodoroRunning) {
        drawPomodoroBtn("#150#Start pomodoro", 2, 2, 150, &startPomodoro);
      } else {
        // DRAW TIMER
        drawPomodoroLbl(TextFormat("%02d:%02d", pState.t.m, pState.t.s), 96, 4,
                        4, 10);

        // DRAW STOP BUTTON
        drawPomodoroBtn("#211#Stop", 3, 3, 10, &stopPomodoro);
      }

      if (stopPomodoro) {
        pState.plsExit = true;
        stopPomodoro = false;
        pState.isPomodoroRunning = false;
      }
      pthread_mutex_unlock(&mtx);

      // display total elapsed time
      drawTotalElapsedTime(totalElapTime);

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

void playSound(PlayableSound sound, ma_engine *soundEngine) {
  const char *soundPath = getSoundPath(sound);
  ma_engine_play_sound(soundEngine, soundPath, NULL);
}

void drawTotalElapsedTime(int s) {
  Time t = {0, 0, 0};
  t.h = s / (60 * 60);
  t.m = (s % (60 * 60)) / 60;
  t.s = s % 60;

  drawPomodoroLbl(TextFormat("Total: %02d:%02d:%02d", t.h, t.m, t.s), 48, 2, 1,
                  20);
}

void *startPomodoroTimer(void *arg) {

  PomodoroState *pState = arg;

  pthread_mutex_lock(&mtx);
  pState->isPomodoroRunning = true;
  pthread_mutex_unlock(&mtx);

  int seconds = MINUTES * 60;

  while (seconds > 0) {
    pthread_mutex_lock(&mtx);

    if (pState->plsExit) {
      pState->plsExit = false;
      pState->playEndSound = true;
      pthread_mutex_unlock(&mtx);
      return NULL;
    }

    pState->t.m = (seconds % (60 * 60)) / 60;
    pState->t.s = seconds % 60;

    pthread_mutex_unlock(&mtx);

    clock_t stop = clock() + CLOCKS_PER_SEC;
    while (clock() < stop) {
    }

    seconds -= 1;
    pState->elapsedFromStart += 1;
  }

  pthread_mutex_lock(&mtx);
  pState->isPomodoroRunning = false;
  pState->plsExit = false;
  pState->playEndSound = true;
  pthread_mutex_unlock(&mtx);

  return NULL;
}

void drawPomodoroLbl(const char *text, int fontSize, int x_eights, int y_eights,
                     int padding) {
  int prevFontSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
  GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);
  Vector2 textboxSize = textboxSizeForText(text, padding);
  GuiLabel((Rectangle){floor((W_WIDTH / 8.0) * x_eights) - textboxSize.x / 2,
                       floor((W_HEIGHT / 8.0) * y_eights) - textboxSize.y / 2,
                       textboxSize.x, textboxSize.y},
           text);
  GuiSetStyle(DEFAULT, TEXT_SIZE, prevFontSize);
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
