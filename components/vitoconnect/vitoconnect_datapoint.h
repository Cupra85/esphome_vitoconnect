/* VitoWiFi

Copyright 2019 Bert Melis

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#pragma once

#include <stdint.h>
#include <assert.h>
#include <functional>
#include <string.h>   // for memcpy
#include "esphome/core/helpers.h"  // millis()

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
