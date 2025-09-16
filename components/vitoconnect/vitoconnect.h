#pragma once

#include "esphome/core/component.h"
#include "vitoconnect_datapoint.h"
#include <vector>

namespace esphome {
namespace vitoconnect {

class VitoConnect : public PollingComponent {
 public:
  VitoConnect() : PollingComponent(1000) {}  // Default Polling 1s

  void setup() override;
  void loop() override;
  void update() override;

  void register_datapoint(Datapoint* dp);

  // Callbacks
  static void _onData(uint8_t* data, uint8_t len, void* arg);
  static void _onError(uint8_t error, void* arg);

 protected:
  std::vector<Datapoint*> _datapoints;
};

}  // namespace vitoconnect
}  // namespace esphome
