#include "vitoconnect_datapoint.h"

namespace esphome {
namespace vitoconnect {

std::function<void(uint8_t[], uint8_t, Datapoint* dp)> Datapoint::_stdOnData = nullptr;

void Datapoint::onData(std::function<void(uint8_t[], uint8_t, Datapoint* dp)> callback) {
  _stdOnData = callback;
}

void Datapoint::encode(uint8_t* raw, uint8_t length, void* data) {
  if (length != _length) {
    memset(raw, 0, _length);
  } else {
    memcpy(raw, data, length);
  }
}

void Datapoint::decode(uint8_t* data, uint8_t length, Datapoint* dp) {
  uint8_t* output = new uint8_t[_length];
  memset(output, 0, _length);
  if (length == _length) {
    memcpy(output, data, length);
    if (_stdOnData) _stdOnData(output, _length, dp);
    _last_update = esphome::millis();  // korrekt
  }
  delete[] output;
}

}  // namespace vitoconnect
}  // namespace esphome
