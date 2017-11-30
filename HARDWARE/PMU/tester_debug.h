#ifndef __TESTER_DEBUG_H
#define	__TESTER_DEBUG_H


#include "usart.h"

#define TESTER_DBG_ON            1
#define TESTER_NET_DBG_ON        1
#define TESTER_MAIN_DBG_ON       1
#define TESTER_TP_DBG_ON         1
#define TESTER_UI_DBG_ON         1






#define TESTER_NET_DEBUG(format,...) do { \
																				 if ((TESTER_NET_DBG_ON) && TESTER_DBG_ON){ \
																					 printf (format, ##__VA_ARGS__); \
																					} \
																			 } while(0);
#define TESTER_MAIN_DEBUG(format,...) do { \
																				 if ((TESTER_MAIN_DBG_ON) && TESTER_DBG_ON){ \
																					 printf (format, ##__VA_ARGS__); \
																					} \
																			 } while(0);
#define TESTER_TP_DEBUG(format,...) do { \
																				 if ((TESTER_TP_DBG_ON) && TESTER_DBG_ON){ \
																					 printf (format, ##__VA_ARGS__); \
																					} \
																			 } while(0);
#define TESTER_UI_DEBUG(format,...) do { \
																				 if ((TESTER_UI_DBG_ON) && TESTER_DBG_ON){ \
																					 printf (format, ##__VA_ARGS__); \
																					} \
																			 } while(0);																			 

#endif /* __TESTER_DEBUG_H */

