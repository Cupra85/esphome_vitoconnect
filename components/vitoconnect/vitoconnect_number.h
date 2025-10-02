#pragma once

#include "esphome/components/number/number_.h"
#include "vitoconnect.h"

namespace esphome {
namespace vitoconnect {

class VitoconnectNumber : public number_::Number_ {
 public:
  void set_parent(VitoConnect *parent, Datapoint* dp) { this->parent_ = parent; this->dp_ = dp; }

 protected:
  void control(float value) override {
    if (this->parent_ && this->dp_) {
      uint16_t raw = (uint16_t)(value * 10);  // Beispiel: 22.5°C -> 225
      this->parent_->write(this->dp_, (uint8_t*)&raw, 2);
    }
    publish_state(value);
  }

 private:
  VitoConnect *parent_{nullptr};
  Datapoint* dp_{nullptr};
};

}  // namespace vitoconnect
}  // namespace esphome
