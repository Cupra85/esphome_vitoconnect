/*
  optolink.cpp - Connect Viessmann heating devices via Optolink to ESPhome

  Copyright (C) 2023  Philipp Danner

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "vitoconnect.h"
#include "esphome/core/log.h"

namespace esphome {
namespace vitoconnect {

static const char *const TAG = "vitoconnect";

VitoConnect::VitoConnect() : _optolink(nullptr) {}

void VitoConnect::setup() {
  if (this->protocol_ == KW) {
    _optolink = new OptolinkKW(this);
  } else if (this->protocol_ == P300) {
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

void VitoConnect::register_datapoint(Datapoint *datapoint) {
  ESP_LOGD(TAG, "Adding datapoint with address 0x%04X and length %d", datapoint->getAddress(),
           datapoint->getLength());
  this->_datapoints.push_back(datapoint);
}

void VitoConnect::loop() {
  if (_optolink) {
    _optolink->loop();
  }
}

void VitoConnect::update() {
  // Zyklische Abfrage mit individuellem Intervall
  uint32_t now = millis();

  for (Datapoint *dp : this->_datapoints) {
    if (now - dp->getLastUpdate() >= dp->getPollInterval()) {
      ESP_LOGD(TAG, "Requesting datapoint 0x%04X (len %d)", dp->getAddress(), dp->getLength());

      CbArg *arg = new CbArg(this, dp);
      if (_optolink->read(dp->getAddress(), dp->getLength(), reinterpret_cast<void *>(arg))) {
        dp->setLastUpdate(now);
      } else {
        delete arg;
      }
    }
  }
}

void VitoConnect::_onData(uint8_t *data, uint8_t len, void *arg) {
  CbArg *cbArg = reinterpret_cast<CbArg *>(arg);
  cbArg->dp->decode(data, len, cbArg->dp);
  delete cbArg;
}

void VitoConnect::_onError(uint8_t error, void *arg) {
  ESP_LOGD(TAG, "Error received: %d", error);
  CbArg *cbArg = reinterpret_cast<CbArg *>(arg);
  if (cbArg->v->_onErrorCb)
    cbArg->v->_onErrorCb(error, cbArg->dp);
  delete cbArg;
}

}  // namespace vitoconnect
}  // namespace esphome
