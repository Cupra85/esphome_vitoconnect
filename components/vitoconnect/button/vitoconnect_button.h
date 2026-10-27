#pragma once

#include <cstdint>

#include "esphome/components/button/button.h"
#include "../vitoconnect.h"

namespace esphome {
namespace vitoconnect {

class OPTOLINKButton : public button::Button {
 public:
  void set_parent(VitoConnect *parent) { parent_ = parent; }
  void set_address(uint16_t address) { address_ = address; }
  void set_length(uint8_t length) { length_ = length; }
  void set_value(uint32_t value) { value_ = value; }

 protected:
  void press_action() override;

 private:
  VitoConnect *parent_{nullptr};
  uint16_t address_{0};
  uint8_t length_{1};
  uint32_t value_{0};
};

}  // namespace vitoconnect
}  // namespace esphome
