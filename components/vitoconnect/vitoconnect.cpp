/*
  optolink.cpp - Connect Viessmann heating devices via Optolink to ESPhome

  Copyright (C) 2023  Philipp Danner

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <stdint.h>
#include <assert.h>
#include <functional>
#include <string.h>  // for memcpy
#include "esphome/core/helpers.h"  // für millis()

namespace esphome {
namespace vitoconnect {

class Datapoint {
 public:
  Datapoint();
  virtual ~Datapoint();

  void setAddress(uint16_t address) { this->_address = address; }
  uint16_t getAddress() const { return this->_address; }

  void setLength(uint8_t length) { this->_length = length; }
  uint8_t getLength() const { return this->_length; }

  // 🆕 Polling-Intervall Setter/Getter
  void setPollInterval(uint32_t interval) { this->_poll_interval = interval; }
  uint32_t getPollInterval() const { return this->_poll_interval; }

  void setLastUpdate(uint32_t t) { this->_last_update = t; }
  uint32_t getLastUpdate() const { return this->_last_update; }

  static void onData(std::function<void(uint8_t[], uint8_t, Datapoint* dp)> callback);
  void onError(uint8_t, Datapoint* dp);

  virtual void encode(uint8_t* raw, uint8_t length, void* data);
  virtual void decode(uint8_t* data, uint8_t length, Datapoint* dp = nullptr);

 protected:
  uint16_t _address{0};
  uint8_t _length{0};

  // 🆕 Polling-Daten
  uint32_t _poll_interval{60000};  // Standard: 60s
  uint32_t _last_update{0};

  static std::function<void(uint8_t[], uint8_t, Datapoint* dp)> _stdOnData;
};

}  // namespace vitoconnect
}  // namespace esphome
