#include "vitoconnect.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"  // für millis()

namespace esphome {
namespace vitoconnect {

static const char *const TAG = "vitoconnect";

void VitoConnect::setup() {
  ESP_LOGD(TAG, "VitoConnect setup complete. %d datapoints registered.", _datapoints.size());
}

void VitoConnect::loop() {
  // Optional: hier UART Loop einfügen
}

void VitoConnect::register_datapoint(Datapoint* dp) {
  ESP_LOGD(TAG, "Register datapoint 0x%04X len %d", dp->getAddress(), dp->getLength());
  _datapoints.push_back(dp);
}

void VitoConnect::update() {
  uint32_t now = esphome::millis();

  for (Datapoint* dp : _datapoints) {
    if (now - dp->getLastUpdate() >= dp->getPollInterval()) {
      ESP_LOGD(TAG, "Requesting datapoint 0x%04X", dp->getAddress());
      dp->setLastUpdate(now);

      // TODO: Hier Optolink read aufrufen, z.B. _optolink->read(...)
    }
  }
}

void VitoConnect::_onData(uint8_t* data, uint8_t len, void* arg) {
  Datapoint* dp = reinterpret_cast<Datapoint*>(arg);
  if (dp) dp->decode(data, len, dp);
}

void VitoConnect::_onError(uint8_t error, void* arg) {
  Datapoint* dp = reinterpret_cast<Datapoint*>(arg);
  ESP_LOGW(TAG, "Error %d on datapoint 0x%04X", error, dp ? dp->getAddress() : 0);
}

}  // namespace vitoconnect
}  // namespace esphome
