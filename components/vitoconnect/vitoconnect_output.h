#pragma once

#include "esphome/components/output/float_output.h"
#include "vitoconnect.h"

namespace esphome {
namespace vitoconnect {

class VitoconnectOutput : public output::FloatOutput {
 public:
  void set_parent(VitoConnect *parent, Datapoint* dp) { this->parent_ = parent; this->dp_ = dp; }

 protected:
  void write_state(float state) override {
    if (this->parent_ && this->dp_) {
      uint8_t raw = (uint8_t)(state * 255.0f);
      this->parent_->write(this->dp_, &raw, 1);
    }
  }

 private:
  VitoConnect *parent_{nullptr};
  Datapoint* dp_{nullptr};
};

}  // namespace vitoconnect
}  // namespace esphome
