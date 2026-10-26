#include "vitoconnect_select.h"

#include "esphome/core/log.h"

namespace esphome {
namespace vitoconnect {

static const char *const TAG = "vitoconnect.select";

void OPTOLINKSelect::decode(uint8_t *data, uint8_t length, Datapoint * /*dp*/) {
  if (data == nullptr || length != _length || (length != 1 && length != 2)) {
    ESP_LOGW(TAG, "Invalid read response for 0x%04X", _address);
    return;
  }
  uint32_t raw = 0;
  for (uint8_t i = 0; i < length; ++i) raw |= static_cast<uint32_t>(data[i]) << (8 * i);
  for (size_t index = 0; index < raw_values_.size(); ++index) {
    if (raw_values_[index] == raw) {
      publish_state(index);
      return;
    }
  }
  ESP_LOGW(TAG, "Unknown option %u at 0x%04X", raw, _address);
}

void OPTOLINKSelect::control(size_t index) {
  if (parent_ == nullptr || index >= raw_values_.size() || (_length != 1 && _length != 2)) {
    ESP_LOGW(TAG, "Rejected select write for 0x%04X", write_address_);
    return;
  }
  const uint32_t raw = raw_values_[index];
  if (raw > ((uint32_t(1) << (8 * _length)) - 1)) return;
  uint8_t payload[2];
  for (uint8_t i = 0; i < _length; ++i) payload[i] = static_cast<uint8_t>(raw >> (8 * i));
  if (!parent_->write_datapoint(this, write_address_, payload, _length)) {
    ESP_LOGW(TAG, "Could not queue select write for 0x%04X", write_address_);
  }
}

}  // namespace vitoconnect
}  // namespace esphome
