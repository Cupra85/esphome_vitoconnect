#pragma once

#include "esphome/components/number/number.h"
#include "../vitoconnect.h"

namespace esphome {
namespace vitoconnect {

class OPTOLINKNumber : public number::Number, public Datapoint {
 public:
  void set_parent(VitoConnect *parent) { parent_ = parent; }
  void set_write_address(uint16_t address) { write_address_ = address; }
  void set_scale(double scale) { scale_ = scale; }
  void set_signed(bool is_signed) { signed_ = is_signed; }
  void set_bounds(float minimum, float maximum) { minimum_ = minimum; maximum_ = maximum; }
  void decode(uint8_t *data, uint8_t length, Datapoint *dp = nullptr) override;

 protected:
  void control(float value) override;

 private:
  VitoConnect *parent_{nullptr};
  uint16_t write_address_{0};
  double scale_{1.0};
  bool signed_{false};
  float minimum_{0.0f};
  float maximum_{0.0f};
};

}  // namespace vitoconnect
}  // namespace esphome
