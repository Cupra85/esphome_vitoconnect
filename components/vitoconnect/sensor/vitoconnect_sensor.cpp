#include "vitoconnect_sensor.h"

namespace esphome {
namespace vitoconnect {

OPTOLINKSensor::OPTOLINKSensor(){
  // empty
}

OPTOLINKSensor::~OPTOLINKSensor() {
  // empty
}

void OPTOLINKSensor::decode(uint8_t* data, uint8_t length, Datapoint* dp) {
  if (data == nullptr || length < _length) return;
  assert(length >= _length);

  if (!dp) dp = this;

  const uint8_t value_length = value_length_ == 0 ? _length : value_length_;
  if (static_cast<uint16_t>(byte_offset_) + value_length > length) return;
  const uint8_t* value_data = data + byte_offset_;

  if (value_length == 1){         // Commonly percentage with factor /2
    publish_state((float) value_data[0]);
  }
  else if (value_length == 2){   // Commonly temperature with factor /10 or /100
    int16_t tmp = 0;
    tmp = value_data[1] << 8 | value_data[0];
    float value = tmp / 1.0f;
    publish_state(value);
  }
  else if (value_length == 4){   // Commonly counter with different factors
    uint32_t tmp = 0;
    tmp = static_cast<uint32_t>(value_data[3]) << 24 | value_data[2] << 16 | value_data[1] << 8 | value_data[0];
    float value = tmp / 1.0f;
    publish_state(value);
  }
}

void OPTOLINKSensor::encode(uint8_t* raw, uint8_t length, void* data) {
  float value = *reinterpret_cast<float*>(data);
  encode(raw, length, value);
}

void OPTOLINKSensor::encode(uint8_t* raw, uint8_t length, float data) {
  assert(length >= _length);

  // Commonly temperature with factor /10 or /100
  if (_length == 2){
    int16_t tmp = floor((data) + 0.5);
    raw[1] = tmp >> 8;
    raw[0] = tmp & 0xFF;
  }

  // Commonly counter with different factors
  if (_length == 4){
    uint32_t tmp = floor((data) + 0.5f);
    raw[3] = tmp >> 24;
    raw[2] = tmp >> 16;
    raw[1] = tmp >> 8;
    raw[0] = tmp & 0xFF;
  }
}

}  // namespace vitoconnect
}  // namespace esphome
