#pragma once

#include <cstdint>

#include "esphome/components/switch/switch.h"
#include "../vitoconnect.h"

namespace esphome {
namespace vitoconnect {

class OPTOLINKSwitch : public switch_::Switch, public Datapoint {
 public:
  void set_parent(VitoConnect *parent) { parent_ = parent; }
  void set_write_address(uint16_t address) { write_address_ = address; }
  void set_on_value(uint32_t value) { on_value_ = value; }
  void set_off_value(uint32_t value) { off_value_ = value; }
  void decode(uint8_t *data, uint8_t length, Datapoint *dp = nullptr) override;

 protected:
  void write_state(bool state) override;

 private:
  VitoConnect *parent_{nullptr};
  uint16_t write_address_{0};
  uint32_t on_value_{1};
  uint32_t off_value_{0};
};

}  // namespace vitoconnect
}  // namespace esphome
