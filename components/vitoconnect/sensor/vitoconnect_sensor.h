#pragma once

#include "esphome/components/sensor/sensor.h"
#include "../vitoconnect_datapoint.h"

namespace esphome {
namespace vitoconnect {

class OPTOLINKSensor : public sensor::Sensor, public Datapoint {

  public:
    OPTOLINKSensor();
    ~OPTOLINKSensor();

    void setByteOffset(uint8_t offset) { byte_offset_ = offset; }
    void setValueLength(uint8_t length) { value_length_ = length; }

    void decode(uint8_t* data, uint8_t length, Datapoint* dp = nullptr) override;
    void encode(uint8_t* raw, uint8_t length, void* data) override;
    void encode(uint8_t* raw, uint8_t length, float data);

  private:
    uint8_t byte_offset_{0};
    uint8_t value_length_{0};

};

}  // namespace vitoconnect
}  // namespace esphome
