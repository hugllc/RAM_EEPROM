/* 
  EEPROM.cpp - esp8266 EEPROM emulation

  Copyright (c) 2014 Ivan Grokhotkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef RAM_EEPROM_h
#define RAM_EEPROM_h

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <cstdio>

class RAMEEPROMClass {
private:
    void _init(void);
    bool _free = false;
public:
    RAMEEPROMClass(void *nothing, size_t size, uint8_t blockSize = 0);
    RAMEEPROMClass(unsigned int address, size_t size, uint8_t blockSize = 0);
    ~RAMEEPROMClass();

    void begin(void);
    uint8_t read(int address);
    void write(int address, uint8_t val);
    bool commit(void);
    bool flush(void);
    void end(void);

    bool readBlock(int block, uint8_t *buffer);
    bool writeBlock(int block, uint8_t *data);
    bool copyBlock(int dest, int src);

    size_t size() {
        return _size;
    }
    size_t blockSize() {
        return _blockSize;
    }
    uint16_t pages() {
        return _size;
    }
    template<typename T> 
    T &get(int address, T &t) {
        if (!_goodAddress(address, sizeof(T))) {
            return t;
        }

        memcpy((uint8_t*) &t, _data + address, sizeof(T));
        return t;
    }

    template<typename T> 
    const T &put(int address, const T &t) {
        if (!_goodAddress(address, sizeof(T))) {
            return t;
        }
        memcpy(_data + address, (const uint8_t*) &t, sizeof(T));
        return t;
    }

protected:
    uint8_t *_data = NULL;
    size_t _size = 0;
    size_t _blockSize = 0;


    bool _goodAddress(int address, size_t size = 0)
    {
        if (_data == NULL) {
            return false;
        }
        int addr = address + size;
        return !((address < 0) || ((size_t)addr >= _size));
    }

    int _blockAddress(int block)
    {
        return block * _blockSize;
    }

    /**
     * Copying not allowed
     */
    RAMEEPROMClass(const RAMEEPROMClass &other)
    {
    }
    /**
     * Copying not allowed
     */
    RAMEEPROMClass &operator=(const RAMEEPROMClass &other)
    {
        return *this;
    }

};

#endif // RAM_EEPROM_H

