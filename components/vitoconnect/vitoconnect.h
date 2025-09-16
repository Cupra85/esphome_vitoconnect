#pragma once

#include "vitoconnect_datapoint.h"
#include "esphome/core/component.h"
#include <vector>

namespace esphome {
namespace vitoconnect {

class OptolinkBase;

enum ProtocolType { KW, P300 };

class VitoConnect : public PollingComponent {
 public:
  VitoConnect() : PollingComponent(0), _optolink(nullptr), protocol(KW) {}

  void setup() override;
  void loop() override;
  void update() override;

  void register_datapoint(Datapoint *datapoint);

  ProtocolType protocol;

 protected:
  OptolinkBase* _optolink;
  std::vector<Datapoint*> _datapoints;

  struct CbArg {
    VitoConnect* v;
    Datapoint* dp;
    CbArg(VitoConnect* vv, Datapoint* dd) : v(vv), dp(dd) {}
  };

  static void _onData(uint8_t* data, uint8_t len, void* arg);
  static void _onError(uint8_t error, void* arg);
};

}  // namespace vitoconnect
}  // namespace esphome
