#include "vitoconnect_number.h"

#include <cmath>
#include <cstdint>

#include "esphome/core/log.h"

namespace esphome {
namespace vitoconnect {

static const char *const TAG = "vitoconnect.number";

void OPTOLINKNumber::decode(uint8_t *data, uint8_t length, Datapoint * /*dp*/) {
  if (data == nullptr || length != _length || (length != 1 && length != 2 && length != 4)) {
    ESP_LOGW(TAG, "Invalid read response for 0x%04X", _address);
    return;
  }
  uint32_t raw = 0;
  for (uint8_t i = 0; i < length; ++i) raw |= static_cast<uint32_t>(data[i]) << (8 * i);
  int64_t decoded = raw;
  if (signed_) {
    const uint8_t bits = 8 * length;
    if ((raw & (uint32_t(1) << (bits - 1))) != 0) decoded -= int64_t(1) << bits;
  }
  publish_state(static_cast<float>(decoded * scale_));
}

void OPTOLINKNumber::control(float value) {
  if (parent_ == nullptr || !std::isfinite(value) || !std::isfinite(scale_) || scale_ <= 0 ||
      value < minimum_ || value > maximum_) {
    ESP_LOGW(TAG, "Rejected out-of-range write for 0x%04X", write_address_);
    return;
  }
  const double scaled = static_cast<double>(value) / scale_;
  const double rounded = std::round(scaled);
  const uint8_t bits = 8 * _length;
  if ((_length != 1 && _length != 2 && _length != 4) || !std::isfinite(rounded)) return;
  const int64_t low = signed_ ? -(int64_t(1) << (bits - 1)) : 0;
  const int64_t high = signed_ ? (int64_t(1) << (bits - 1)) - 1 : (int64_t(1) << bits) - 1;
  if (rounded < low || rounded > high) {
    ESP_LOGW(TAG, "Encoded value does not fit 0x%04X", write_address_);
    return;
  }
  const uint64_t integer = static_cast<uint64_t>(static_cast<int64_t>(rounded));
  uint8_t payload[4];
  for (uint8_t i = 0; i < _length; ++i) payload[i] = static_cast<uint8_t>(integer >> (8 * i));
  if (!parent_->write_datapoint(this, write_address_, payload, _length)) {
    ESP_LOGW(TAG, "Could not queue write for 0x%04X", write_address_);
  }
}

}  // namespace vitoconnect
}  // namespace esphome
