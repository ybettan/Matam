#ifndef AUX_MACROS_H_
#define AUX_MACROS_H_

#include <stdio.h>

#define TEST_EQUALS(res_arg, a,b)   if (res_arg && (a != b)) { \
                                        res_arg = false; \
                                    }

#define EQUALS(a,b)                     if (a == b) { \
                                            printf("SUCCESS\n"); \
                                        }else{ \
                                            printf("----FAILED----\n"); \
                                        }

#define DIFFERENT(a,b)                  if (a != b) { \
                                            printf("SUCCESS\n"); \
                                        }else{ \
                                            printf("FAILED\n"); \
                                        }

#define EQUALS_STRINGS(a,b)             if (strcmp(a,b) == 0) { \
                                            printf("SUCCESS\n"); \
                                        }else{ \
                                            printf("----FAILED----\n"); \
                                        }

#define TEST_DIFFERENT(res_arg, a,b)    if (res_arg && (a == b)) { \
                                            res_arg = false; \
                                        }


#define RUN_TEST(name)  printf("Running "); \
                        printf(#name);      \
                        printf("... ");     \
                        if (!name()) { \
                            printf("[FAILED]\n");       \
                            return false; \
                        }                               \
                        printf("[SUCCESS]\n");





#endif /* AUX_MACROS_H_ */
