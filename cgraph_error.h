#ifndef CGRAPH_ERROR_H_
#define CGRAPH_ERROR_H_

typedef void cgraph_error_handler_t(
                  const char * reason,
                  const char * file,
                  int line);

#define CGRAPH_ERROR(reason) \
    do { \
        cgraph_error (reason, __FILE__, __LINE__) ; \
    } while (0)

int cgraph_error(const char *reason, 
                 const char *file, 
                 int line);

cgraph_error_handler_t* cgraph_set_error_handler(
    cgraph_error_handler_t* new_handler);

#endif  // CGRAPH_ERROR_H_