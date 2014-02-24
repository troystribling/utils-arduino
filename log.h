#ifndef LOG_H
#define LOG_H

#ifdef DEBUG
  #define DLOG(...) Serial.print(F("DEBUG: ")); Serial.println(__VA_ARGS__)
#else
  #define DLOG(...) {}
#endif

#ifdef INFO
  #define INFO(...) Serial.print(F("INFO: ")); Serial.println(__VA_ARGS__)
#else
  #define INFO(...) {}
#endif

#ifdef ERROR
  #define ERROR(...) Serial.print(F("ERROR: ")); Serial.println(__VA_ARGS__)
#else
  #define ERROR(...) {}
#endif

#endif