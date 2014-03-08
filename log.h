#ifndef LOG_H
#define LOG_H

#ifdef DBUG
  #define DBUG(...) Serial.print(F("DBUG: ")); Serial.println(__VA_ARGS__)
#else
  #define DBUG(...) {}
#endif

#ifdef INFO
  #define INFO(...) Serial.print(F("INFO: ")); Serial.println(__VA_ARGS__)
#else
  #define INFO(...) {}
#endif

#ifdef ERROR
  #define ERROR(...) Serial.print(F("ERROR: ")); Serial.println(__VA_ARGS__)
  #define HALT_ON_ERROR(X, Y) if (!X) {     \
    ERROR(Y);                               \
    while(1);                               \
  }                                         \
#else
  #define ERROR(...) {}
  #define HALT_ON_ERROR(X, Y) X
#endif

#endif