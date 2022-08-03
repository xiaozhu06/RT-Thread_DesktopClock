#ifndef __HTTP_CLIENT_H_
#define __HTTP_CLIENT_H_

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct http_weather_info_s {
    char weather[8];
    char temperature[8];
}http_weather_info_t;

int http_weather_collect(void);
http_weather_info_t *http_weather_info(void);

#ifdef  __cplusplus
}
#endif
#endif

