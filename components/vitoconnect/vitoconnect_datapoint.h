#pragma once

#include <cstdint>
#include <functional>
#include <cstring>

namespace esphome {
namespace vitoconnect {

class Datapoint {
 public:
  Datapoint() = default;
  virtual ~Datapoint() = default;

  void setAddress(uint16_t address) { _address = address; }
  uint16_t getAddress() const { return _address; }

  void setLength(uint8_t length) { _length = length; }
  uint8_t getLength() const { return _length; }

  void setPollInterval(uint32_t interval) { _poll_interval = interval; }
  uint32_t getPollInterval() const { return _poll_interval; }

  void setLastUpdate(uint32_t t) { _last_update = t; }
  uint32_t getLastUpdate() const { return _last_update; }

  virtual void encode(uint8_t* raw, uint8_t length, void* data);
  virtual void decode(uint8_t* data, uint8_t length, Datapoint* dp = nullptr);

 protected:
  uint16_t _address{0};
  uint8_t _length{0};
  uint32_t _poll_interval{60000};
  uint32_t _last_update{0};

  static std::function<void(uint8_t[], uint8_t, Datapoint*)> _stdOnData;
};

}  // namespace vitoconnect
}  // namespace esphome
