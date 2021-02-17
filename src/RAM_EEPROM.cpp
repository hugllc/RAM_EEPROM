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

#include "Arduino.h"
#include "RAM_EEPROM.h"

RAMEEPROMClass::RAMEEPROMClass(void *nothing, size_t size, uint8_t blockSize)
: _size(size), _blockSize(blockSize)
{
    _init();
}

RAMEEPROMClass::RAMEEPROMClass(unsigned int address, size_t size, uint8_t blockSize)
 : _size(size), _blockSize(blockSize)
{
    _init();
}
void RAMEEPROMClass::_init(void) 
{
    _data = new uint8_t[_size];
    if (_blockSize > _size) {
        _blockSize = _size;
    }
    memset(_data, 0xFF, _size);
}

RAMEEPROMClass::~RAMEEPROMClass()
{
    end();
    delete [] _data;
    _data = NULL;
}

void RAMEEPROMClass::begin(void) {
}

void RAMEEPROMClass::end(void) {
}


uint8_t RAMEEPROMClass::read(int address) {
    if (!_goodAddress(address)) {
        return 0;
    }
    return _data[address];
}

void RAMEEPROMClass::write(int address, uint8_t value) {
    if (!_goodAddress(address)) {
        return;
    }
    _data[address] = value;
}

bool RAMEEPROMClass::readBlock(int block, uint8_t *buffer) {
    int address = _blockAddress(block);
    if (!_goodAddress(address, _blockSize) || !buffer || (_blockSize == 0)) {
        return false;
    }
    memcpy(buffer, &_data[address], _blockSize);
    return true;
}

bool RAMEEPROMClass::writeBlock(int block, uint8_t *buffer) {
    int address = _blockAddress(block);
    uint8_t index;
    if (!_goodAddress(address, _blockSize) || !buffer || (_blockSize == 0)) {
        return false;
    }
    for (index = 0; index < _blockSize; index++) {
        _data[address + index] = buffer[index];
    }
    return true;
}

bool RAMEEPROMClass::copyBlock(int dest, int src) {
    int address = _blockAddress(src);
    if (!_goodAddress(address, _blockSize)) {
        return false;
    }
    return writeBlock(dest, &_data[address]);
}

bool RAMEEPROMClass::commit(void) {
    return true;
}

bool RAMEEPROMClass::flush(void) {
    return true;
}