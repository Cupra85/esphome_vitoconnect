#pragma once

#include <cstdint>
#include <vector>

#include "esphome/components/select/select.h"
#include "../vitoconnect.h"

namespace esphome {
namespace vitoconnect {

class OPTOLINKSelect : public select::Select, public Datapoint {
 public:
  void set_parent(VitoConnect *parent) { parent_ = parent; }
  void set_write_address(uint16_t address) { write_address_ = address; }
  void add_raw_value(uint32_t value) { raw_values_.push_back(value); }
  void decode(uint8_t *data, uint8_t length, Datapoint *dp = nullptr) override;

 protected:
  void control(size_t index) override;

 private:
  VitoConnect *parent_{nullptr};
  uint16_t write_address_{0};
  std::vector<uint32_t> raw_values_;
};

}  // namespace vitoconnect
}  // namespace esphome
