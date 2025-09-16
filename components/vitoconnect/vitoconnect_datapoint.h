#pragma once

#include <stdint.h>
#include <functional>
#include <string.h>
#include "esphome/core/helpers.h"

namespace esphome {
namespace vitoconnect {

class Datapoint {
 public:
  Datapoint() = default;
  virtual ~Datapoint() = default;

  void setAddress(uint16_t address) { this->_address = address; }
  uint16_t getAddress() const { return this->_address; }

  void setLength(uint8_t length) { this->_length = length; }
  uint8_t getLength() const { return this->_length; }

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

  uint32_t _poll_interval{60000};
  uint32_t _last_update{0};

  static std::function<void(uint8_t[], uint8_t, Datapoint* dp)> _stdOnData;
};

}  // namespace vitoconnect
}  // namespace esphome
