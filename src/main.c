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
void drawStartPomodoro(bool *start);

typedef struct {
  int elapsed;
} Timer;

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  bool startPomodoro = false;
  pthread_t timerThread;

  InitWindow(W_WIDTH, W_HEIGHT, "Example");
  SetTargetFPS(60);

  // set font size for 4k display
  GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

  while (!WindowShouldClose()) {
    BeginDrawing();
    {
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      drawStartPomodoro(&startPomodoro);

      if (startPomodoro) {

	if (pthread_create(&timerThread, NULL, startPomodoroTimer, NULL) != 0) {
	  perror("pthread_create");
	  return 1;
	}
        startPomodoro = false;
      }
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}

void *startPomodoroTimer(void *arg) {
  (void)arg;
  printf("Starting pomodoro\n");

  while (true) {
    printf("    Pomodoro ticking\n");
    sleep(1);
  }
}

void drawStartPomodoro(bool *start) {
  const char *text = "#150#Start pomodoro";
  Vector2 textboxSize = textboxSizeForText(text, 150);
  if (GuiButton((Rectangle){floor(W_WIDTH / 2.0) - textboxSize.x / 2,
                            floor(W_HEIGHT / 2.0) - textboxSize.y / 2,
                            textboxSize.x, textboxSize.y},
                text)) {
    *start = true;
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
