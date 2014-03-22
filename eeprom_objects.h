#include <EEPROM.h>
#include <Arduino.h>
#include "log.h"

template <class T> class EEPROMObject {
public:
    EEPROMObject(uint16_t _offset, uint8_t _maxObjects) : offset(_offset), maxObjects(_maxObjects){};
    uint16_t create(uint8_t& index, const T& value);
    uint16_t update(uint8_t index, const T& value);
    uint16_t read(uint8_t index, T& value);
    uint16_t remove(uint8_t index);
    uint16_t next(uint8_t& index, T& value);
    uint8_t count();
private:
    uint16_t location(int8_t index, uint16_t size){return (offset + index * size);};
    uint16_t nextFreeLocation(uint16_t size);
    uint8_t nextUsedIndex(uint8_t currentIndex, uint16_t size);
    uint16_t offset;
    uint8_t  maxObjects;
};

template <class T> uint16_t EEPROMObject<T>::create(uint8_t& index, const T& value) {
    const byte* v = (const byte*)(const void*)&value;
    uint16_t loc = nextFreeLocation(sizeof(value)), i;
    if (loc != 0xffff) {
        index = (loc - offset) / sizeof(value);
        for (i = 0; i < sizeof(value); i++) {
            EEPROM.write(loc, *v);
            loc++;
            v++;
        }
    } else {
        i = 0xffff;
    }
    return i;
}

template <class T> uint16_t EEPROMObject<T>::update(uint8_t index, const T& value) {
    const byte* v = (const byte*)(const void*)&value;
    uint16_t loc = location(index, sizeof(value)), i;
    for (i = 0; i < sizeof(value); i++) {
        EEPROM.write(loc, *v);
        loc++;
        v++;
    }
    return i;
}

template <class T> uint16_t EEPROMObject<T>::read(uint8_t index, T& value) {
    byte* v = (byte*)(void*)&value;
    uint16_t loc = location(index, sizeof(value)), i;
    for (i = 0; i < sizeof(value); i++) {
        *v = EEPROM.read(loc);
        loc++;
        v++;
    }
    return i;
}

template <class T> uint16_t EEPROMObject<T>::next(uint8_t& nextIndex, T& value) {
    nextIndex = nextUsedIndex(nextIndex, sizeof(value));
    return read(nextIndex, value);
}

template <class T> uint16_t EEPROMObject<T>::remove(uint8_t index) {
    uint16_t loc = location(index, sizeof(T));
    EEPROM.write(loc, 0x0);
}

template <class T> uint8_t EEPROMObject<T>::count() {
    uint8_t cnt = 0;
    uint16_t loc = location(0, sizeof(T));
    for (uint8_t i = 0; i < maxObjects; i++) {
        if (EEPROM.read(loc) != 0x0) {
            cnt++;
        }
        loc += sizeof(T);
    }
    return cnt;
}

// private
template <class T> uint16_t EEPROMObject<T>::nextFreeLocation(uint16_t size) {
    uint16_t loc = offset;
    for (uint8_t i = 0; i < maxObjects; i++) {
        if (EEPROM.read(loc) == 0x0) {
            break;
        } else if (i == maxObjects - 1) {
            loc = 0xffff;
            break;
        }
        loc += size;
    }
    return loc;
}

template <class T> uint8_t EEPROMObject<T>::nextUsedIndex(uint8_t currentIndex, uint16_t size) {
    uint16_t loc = location(currentIndex + 1, size);
    uint8_t i;
    for (i = currentIndex + 1; i < maxObjects; i++) {
        if (EEPROM.read(loc) != 0x0) {
            break;
        } else if (i == maxObjects - 1) {
            i = 0xff;
            break;
        }
        loc += size;
    }
    if (i == 0xff) {
        loc = location(0, size);
        for (i = 0; i < currentIndex; i++) {
            if (EEPROM.read(loc) != 0x0) {
                break;
            } else if (i == currentIndex - 1) {
                i = currentIndex;
                break;
            }
            loc += size;
        }
    }
    return i;
}
