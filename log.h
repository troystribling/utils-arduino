#ifndef LOG_H
#define LOG_H

#include "memory_free.h"

#ifdef DBUG
  #define DBUG_LOG(...) Serial.print(F("DBUG: ")); Serial.println(__VA_ARGS__)
  #define DBUG_FREE_MEMORY Serial.print(F("DBUG: Free Memory: "));Serial.print(freeMemory());Serial.println(F(" bytes"))
#else
  #define DBUG_LOG(...)
  #define DBUG_FREE_MEMORY
#endif

#ifdef INFO
  #define INFO_LOG(...) Serial.print(F("INFO: ")); Serial.println(__VA_ARGS__)
#else
  #define INFO_LOG(...)
#endif

#ifdef ERROR
  #define ERROR_LOG(...) Serial.print(F("ERROR: ")); Serial.println(__VA_ARGS__)
  #define HALT_ON_ERROR(X, Y) do {    \
    if (!X) {                         \
      ERROR_LOG(Y);                   \
      while(1);                       \
    }                                 \
  } while(0)
#else
  #define ERROR_LOG(...)
  #define HALT_ON_ERROR(X, Y) X;
#endif

#endif