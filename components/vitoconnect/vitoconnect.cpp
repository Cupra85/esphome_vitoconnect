#include "vitoconnect.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace vitoconnect {

static const char *const TAG = "vitoconnect";

void VitoConnect::setup() {
  if (protocol == KW) {
    _optolink = new OptolinkKW(this);
  } else if (protocol == P300) {
    _optolink = new OptolinkP300(this);
  } else {
    ESP_LOGW(TAG, "Unknown protocol.");
  }

  _datapoints.shrink_to_fit();

  if (_optolink) {
    _optolink->onData(&VitoConnect::_onData);
    _optolink->onError(&VitoConnect::_onError);
    _optolink->begin();
  } else {
    ESP_LOGW(TAG, "Not able to initialize VitoConnect");
  }
}

void VitoConnect::register_datapoint(Datapoint* datapoint) {
  ESP_LOGD(TAG, "Adding datapoint with address 0x%04X and length %d", datapoint->getAddress(),
           datapoint->getLength());
  _datapoints.push_back(datapoint);
}

void VitoConnect::loop() {
  if (_optolink) {
    _optolink->loop();
  }
}

void VitoConnect::update() {
  uint32_t now = esphome::millis();

  for (Datapoint* dp : _datapoints) {
    if (now - dp->getLastUpdate() >= dp->getPollInterval()) {
      ESP_LOGD(TAG, "Requesting datapoint 0x%04X (len %d)", dp->getAddress(), dp->getLength());

      CbArg* arg = new CbArg(this, dp);
      if (_optolink->read(dp->getAddress(), dp->getLength(), reinterpret_cast<void*>(arg))) {
        dp->setLastUpdate(now);
      } else {
        delete arg;
      }
    }
  }
}

void VitoConnect::_onData(uint8_t* data, uint8_t len, void* arg) {
  CbArg* cbArg = reinterpret_cast<CbArg*>(arg);
  cbArg->dp->decode(data, len, cbArg->dp);
  delete cbArg;
}

void VitoConnect::_onError(uint8_t error, void* arg) {
  ESP_LOGD(TAG, "Error received: %d", error);
  CbArg* cbArg = reinterpret_cast<CbArg*>(arg);
  if (cbArg->v->_onErrorCb)
    cbArg->v->_onErrorCb(error, cbArg->dp);
  delete cbArg;
}

}  // namespace vitoconnect
}  // namespace esphome
