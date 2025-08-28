#define NOB_IMPLEMENTATION
#include "./libs/nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER "src/"

int main(int argc, char **argv) {
  NOB_GO_REBUILD_URSELF(argc, argv);

  if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

  // BUILD PROJECT
  Nob_Cmd cmd = {0};

  nob_cc(&cmd);
  nob_cc_flags(&cmd);
  nob_cc_output(&cmd, BUILD_FOLDER"pomodoro");
  nob_cc_inputs(&cmd, SRC_FOLDER"pomodoro.c");
  nob_cmd_append(&cmd, "-Ilibs", "-Ilibs/raylib-5.5/include");
  nob_cmd_append(&cmd, "-L./libs/raylib-5.5/lib");
  nob_cmd_append(&cmd, "-Wl,-rpath,$ORIGIN/../libs/raylib-5.5/lib");
  nob_cmd_append(&cmd, "-lraylib", "-lGL", "-lm", "-lpthread", "-ldl", "-lrt", "-lX11");

  if (!nob_cmd_run(&cmd)) return 1;

  printf("\n");
  
  // RUN
  Nob_Cmd run_cmd = {0};
  nob_cmd_append(&run_cmd, BUILD_FOLDER"pomodoro");

  if (!nob_cmd_run(&run_cmd)) return 1;

}

