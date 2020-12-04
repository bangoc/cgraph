#include "cgraph_error.h"

static cgraph_error_handler_t *cgraph_i_error_handler = 0;

void cgraph_error_handler_ignore(
      const char *reason,
      const char *file,
      int line) {
}

int cgraph_error(const char *reason, const char *file, int line) {
    if (cgraph_i_error_handler) {
        cgraph_i_error_handler(reason, file, line);
    }
  return 0;
}

cgraph_error_handler_t * cgraph_set_error_handler(
          cgraph_error_handler_t * new_handler) {
    cgraph_error_handler_t * previous_handler = cgraph_i_error_handler;
    cgraph_i_error_handler = new_handler;
  return previous_handler;
}