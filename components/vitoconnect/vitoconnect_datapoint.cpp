/* VitoWiFi

Copyright 2019 Bert Melis

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/


#include "vitoconnect_datapoint.h"
#include "esphome/core/log.h"

namespace esphome {
namespace vitoconnect {

static const char *TAG = "vitoconnect.datapoint";

std::function<void(uint8_t[], uint8_t, Datapoint* dp)> Datapoint::_stdOnData = nullptr;

Datapoint::Datapoint() {
  _address = 0;
  _length = 0;
  _poll_interval = 60000;  // default 60s
  _last_update = 0;
}

Datapoint::~Datapoint() {
  // nothing to clean up
}

void Datapoint::onData(std::function<void(uint8_t[], uint8_t, Datapoint* dp)> callback) {
  _stdOnData = callback;
}

void Datapoint::onError(uint8_t error, Datapoint* dp) {
  ESP_LOGW(TAG, "Error reading datapoint 0x%X: %d", dp->getAddress(), error);
}

void Datapoint::encode(uint8_t* raw, uint8_t length, void* data) {
  if (length != _length) {
    ESP_LOGW(TAG, "Encode length mismatch for 0x%X: expected %d, got %d", _address, _length, length);
    memset(raw, 0, _length);
  } else {
    memcpy(raw, data, length);
  }
}

void Datapoint::decode(uint8_t* data, uint8_t length, Datapoint* dp) {
  if (length != _length) {
    ESP_LOGW(TAG, "Decode length mismatch for 0x%X: expected %d, got %d", _address, _length, length);
    return;
  }

  // Kopie der Daten
  uint8_t* output = new uint8_t[_length];
  memcpy(output, data, _length);

  // Callback aufrufen
  if (_stdOnData) {
    _stdOnData(output, _length, dp);
  }

  delete[] output;

  // Update last_update-Zeit automatisch beim Decodieren
  _last_update = millis();
}

}  // namespace vitoconnect
}  // namespace esphome

