#include "vitoconnect_button.h"

#include "esphome/core/log.h"

namespace esphome {
namespace vitoconnect {

static const char *const TAG = "vitoconnect.button";

void OPTOLINKButton::press_action() {
  if (parent_ == nullptr || (length_ != 1 && length_ != 2 && length_ != 4)) {
    ESP_LOGW(TAG, "Rejected button write for 0x%04X", address_);
    return;
  }
  if (length_ < 4 && value_ > ((uint32_t(1) << (8 * length_)) - 1)) {
    ESP_LOGW(TAG, "Button value does not fit length at 0x%04X", address_);
    return;
  }
  uint8_t payload[4];
  for (uint8_t i = 0; i < length_; ++i) payload[i] = static_cast<uint8_t>(value_ >> (8 * i));
  if (!parent_->write_command(address_, payload, length_)) {
    ESP_LOGW(TAG, "Could not queue button write for 0x%04X", address_);
  }
}

}  // namespace vitoconnect
}  // namespace esphome
