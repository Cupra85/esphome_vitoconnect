#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/uart/uart_component.h"
#include "esphome/components/sensor/sensor.h"
#include "vitoconnect_optolink.h"
#include "vitoconnect_optolinkP300.h"
#include "vitoconnect_optolinkKW.h"
#include "vitoconnect_datapoint.h"

using namespace std;

namespace esphome {
namespace vitoconnect {

class VitoConnect : public uart::UARTDevice, public PollingComponent {
 public:
    VitoConnect() : PollingComponent(0) {}

    void setup() override;
    void loop() override;
    void update() override;

    void set_protocol(std::string protocol) { this->protocol = protocol; }
    void register_datapoint(Datapoint *datapoint);

    void onData(std::function<void(const uint8_t* data, uint8_t length, Datapoint* dp)> callback);
    void onError(std::function<void(uint8_t, Datapoint*)> callback);

    // ADDED: zentrale Schreibfunktion für switch, number, output
    bool write(Datapoint* datapoint, uint8_t* value, uint8_t len);

 protected:

 private:
    Optolink* _optolink{nullptr};
    std::vector<Datapoint*> _datapoints;
    std::string protocol;

    struct CbArg {
      CbArg(VitoConnect* vw, Datapoint* d) :
        v(vw),
        dp(d) {}
      VitoConnect* v;
      Datapoint* dp;
    };
    static void _onData(uint8_t* data, uint8_t len, void* arg);
    static void _onError(uint8_t error, void* arg);

    std::function<void(uint8_t, Datapoint*)> _onErrorCb;
};

}  // namespace vitoconnect
}  // namespace esphome
