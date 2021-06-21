#ifndef CGRAPH_ERROR_H_
#define CGRAPH_ERROR_H_

#include <stdbool.h>

typedef enum {
  CGRAPH_SUCCESS           = 0,
  CGRAPH_FAILURE           = 1,
  CGRAPH_ERROR_NO_COUNT
} cgraph_error_t;

extern cgraph_error_t cgraph_last_errno;

void cgraph_reset_last_errno();

bool cgraph_last_op_success();

typedef void cgraph_error_handler_t(
                  const char * reason,
                  const char * file,
                  int line,
                  cgraph_error_t errno);

typedef void cgraph_warning_handler_t(
                  const char * reason,
                  const char * file,
                  int line);

const char* cgraph_strerror(const cgraph_error_t cgraph_errno);

#define CGRAPH_ERROR(error_message, cgraph_errno) \
    do { \
        cgraph_error(error_message, __FILE__, __LINE__, cgraph_errno) ; \
    } while (0)

#define CGRAPH_CHECK(a) do { \
        int cgraph_i_ret=(a); \
        if (cgraph_i_ret != 0) {\
            CGRAPH_ERROR("Api failed", CGRAPH_FAILURE); \
        } } while (0)

#define CGRAPH_WARNING(reason) \
    do { \
        cgraph_warning(reason, __FILE__, __LINE__); \
    } while (0)

#ifdef __cplusplus
extern "C"{
#endif

void cgraph_error_handler_ignore(const char *, const char *, int, cgraph_error_t);

int cgraph_error(const char *reason,
                 const char *file,
                 int line,
                 cgraph_error_t errno);

int cgraph_warning(const char *reason,
                 const char *file,
                 int line);
void cgraph_warning_print(const char * reason,
                  const char * file,
                  int line);

void cgraph_error_print(const char * reason,
                  const char * file,
                  int line,
                  cgraph_error_t errno);

cgraph_error_handler_t* cgraph_set_error_handler(
    cgraph_error_handler_t* new_handler);

cgraph_warning_handler_t* cgraph_set_warning_handler(
    cgraph_warning_handler_t* new_handler);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_ERROR_H_
