#pragma once

#include "esphome/components/switch/switch.h"
#include "vitoconnect.h"

namespace esphome {
namespace vitoconnect {

class VitoconnectSwitch : public switch_::Switch {
 public:
  void set_parent(VitoConnect *parent, Datapoint* dp) { this->parent_ = parent; this->dp_ = dp; }

 protected:
  void write_state(bool state) override {
    uint8_t val = state ? 1 : 0;
    if (this->parent_ && this->dp_) {
      this->parent_->write(this->dp_, &val, 1);
    }
    publish_state(state);
  }

 private:
  VitoConnect *parent_{nullptr};
  Datapoint* dp_{nullptr};
};

}  // namespace vitoconnect
}  // namespace esphome
