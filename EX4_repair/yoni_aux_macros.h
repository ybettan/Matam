#ifndef AUX_MACROS_H_
#define AUX_MACROS_H_

#include <iostream>

using std::string;
using std::cout;
using std::endl;


#define EQUALS(a,b)                     if ((a) == (b)) { \
                                            cout << "SUCCESS" << endl; \
                                        }else{ \
                                            cout << "----FAILED----" << endl; \
                                        }

#define DIFFERENT(a,b)                  if ((a) != (b)) { \
											cout << "SUCCESS" << endl; \
										}else{ \
											cout << "----FAILED----" << endl; \
										}


#define RUN_TEST(name)  cout << "Running "; \
                        cout << #name;      \
                        cout << "... ";     \
                        if (!name()) { \
                            cout << "[FAILED]" << endl;       \
                            return false; \
                        }                               \
                        cout << "[SUCCESS]" << endl;





#endif /* AUX_MACROS_H_ */
