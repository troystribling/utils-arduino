#include <EEPROM.h>
#include <Arduino.h>

template <class T> class EEPROMObject {
public:
    EEPROMObject(uint16_t _offset, uint8_t _maxObjects) : offset(_offset), maxObjects(_maxObjects){};
    uint16_t create(const T& value);
    uint16_t update(uint16_t index, const T& value);
    uint16_t read(uint16_t index, T& value);
    uint16_t remove(uint16_t index);
    uint8_t count();
private:
    uint16_t location(int16_t index, uint16_t size){return (offset + index * size);};
    uint8_t nextLocation(uint16_t size);
    uint16_t offset;
    uint8_t  maxObjects;
};

template <class T> uint16_t EEPROMObject<T>::create(const T& value) {
    const byte* v = (const byte*)(const void*)&value;
    uint16_t loc = nextLocation(sizeof(value)), i;
    for (i = 0; i < sizeof(value); i++) {
        EEPROM.write(loc, *v);
        loc++;
        v++;
    }
    return i;
}

template <class T> uint16_t EEPROMObject<T>::update(uint16_t index, const T& value) {
    const byte* v = (const byte*)(const void*)&value;
    uint16_t loc = location(index, sizeof(value)), i;
    for (i = 0; i < sizeof(value); i++) {
        EEPROM.write(loc, *v);
        loc++;
        v++;
    }
    return i;
}

template <class T> uint16_t EEPROMObject<T>::read(uint16_t index, T& value) {
    byte* v = (byte*)(void*)&value;
    uint16_t loc = location(index, sizeof(value)), i;
    for (i = 0; i < sizeof(value); i++) {
        *v = EEPROM.read(loc);
        loc++;
        v++;
    }
    return i;
}

template <class T> uint16_t EEPROMObject<T>::remove(uint16_t index) {
    uint16_t loc = location(index, sizeof(T));
    EEPROM.write(loc, 0x0);
}

template <class T> uint8_t EEPROMObject<T>::count() {
    uint8_t cnt = 0;
    for (int i = 0; i < maxObjects; i++) {
        uint8_t loc = location(i, sizeof(T));
        if (EEPROM.read(loc) != 0x0) {
            cnt++;
        }
    }
    return cnt;
}

template <class T> uint8_t EEPROMObject<T>::nextLocation(uint16_t size) {
    uint16_t loc = offset;
    uint8_t i;
    for (i = 0; i < maxObjects; i++) {
        if (EEPROM.read(loc) == 0x0) {
            break;
        }
        loc += size;
    }
    return i;
}
