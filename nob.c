#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER "src/"

int main(int argc, char **argv) {
  NOB_GO_REBUILD_URSELF(argc, argv);

  if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

  // BUILD PROJECT
  Nob_Cmd cmd = {0};

  nob_cc(&cmd);
  nob_cc_flags(&cmd);
  nob_cc_output(&cmd, BUILD_FOLDER"main");
  nob_cc_inputs(&cmd, SRC_FOLDER"main.c");
  nob_cmd_append(&cmd, "-lraylib", "-lGL", "-lm", "-lpthread", "-ldl", "-lrt", "-lX11");

  if (!nob_cmd_run(&cmd)) return 1;

  printf("\n");
  
  // RUN
  Nob_Cmd run_cmd = {0};
  nob_cmd_append(&run_cmd, BUILD_FOLDER"main");

  if (!nob_cmd_run(&run_cmd)) return 1;

}

