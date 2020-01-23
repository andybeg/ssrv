#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#include <stdint.h>

#define PACKED __attribute__((packed))

typedef int8_t      int8;
typedef int16_t     int16;
typedef int32_t     int32;
typedef int64_t     int64;
typedef int64_t     __int64;

typedef uint8_t     uint8;
typedef uint16_t    uint16;
typedef uint32_t    uint32;
typedef uint64_t    uint64;
#define FALSE 0
#define S_OK 0
#define S_FALSE 1

// Структура описывающая идентификатор
typedef struct {
    uint8 ID[16];
} SId;

typedef struct SYSTEMTIME {
  int16 wYear;
  int16 wMonth;
  int16 wDayOfWeek;
  int16 wDay;
  int16 wHour;
  int16 wMinute;
  int16 wSecond;
  int16 wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME;

#endif
