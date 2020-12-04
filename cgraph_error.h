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

#define CGRAPH_CHECK(a) do { \
        int cgraph_i_ret=(a); \
        if (cgraph_i_ret != 0) {\
            CGRAPH_ERROR("API failed"); \
        } } while (0)

#ifdef __cplusplus
extern "C"{
#endif 

void cgraph_error_handler_ignore(const char *, const char *, int);

int cgraph_error(const char *reason, 
                 const char *file, 
                 int line);

cgraph_error_handler_t* cgraph_set_error_handler(
    cgraph_error_handler_t* new_handler);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_ERROR_H_