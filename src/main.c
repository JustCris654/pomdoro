#include <raylib.h>
#include <stdio.h>

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

int main(int argc, char **argv) {

  InitWindow(800, 600, "Example");
  SetTargetFPS(60);

  // Font size customization
  GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

  bool showMessageBox = false;

  while (!WindowShouldClose()) {
    BeginDrawing();
    {
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      char *text = "#191#Show Message";
      Vector2 textSize = MeasureTextEx(
          GuiGetFont(), text, (float)GuiGetStyle(DEFAULT, TEXT_SIZE),
          (float)GuiGetStyle(DEFAULT, TEXT_SPACING));
      // add padding
      textSize.x += 10;
      textSize.y += 10;
      printf("Text sizes: %f %f", textSize.x, textSize.y);
      if (GuiButton((Rectangle){400 - textSize.x / 2, 300 - textSize.y / 2,
                                textSize.x, textSize.y},
                    text)) {
        showMessageBox = true;
      }

      if (showMessageBox) {
        int result =
            GuiMessageBox((Rectangle){85, 70, 250, 100}, "#191#Message Box",
                          "Hi! This is a message!", "Nice;Cool");
        if (result >= 0) {
          showMessageBox = false;
        }
      }
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
