#include "../src/JOT.h"

int main (void) {
  JOT::base_hook &hook = JOT::base_hook::getInstance();
  hook.installHook(WH_MOUSE_LL, JOT::printMouseEvent_high_res_stream);
  hook.messageLoop();
  return 0;
}