# Vitoconnect for ESPHome

Integration of Viessmann Optolink protocol into [ESPHome]. I named this component after the cloud reading unit of Viessmann: Vitoconnect.

## Usage

For usage, simply add the following to your config file. Example: V200WO1.
Address, length and post processing can be retrieved from <https://github.com/openv/openv/wiki/Adressen>. Length of 2 bytes is by default interpreted as int16, 4 bytes as uint32. Both values are then converted to float.

```yaml
external_components:
  - source: github://Cupra85/esphome_vitoconnect

esphome:
  name: viessmann-reader
  friendly_name: vitoconnect

esp32:
  board: esp32doit-devkit-v1

# esp8266:
  # board: nodemcuv2

uart:
  - id: uart_vitoconnect
    rx_pin: GPIO16              # ESP32 RX2
    tx_pin: GPIO17              # ESP32 TX2
    #rx_pin: GPIO03             # ESP8266 RX
    #tx_pin: GPIO01             # ESP8266 TX
    baud_rate: 4800
    data_bits: 8
    parity: EVEN
    stop_bits: 2
    # debug:
    #   direction: BOTH
    #   dummy_receiver: false
    #   after:
    #     delimiter: [0x06]
    #   sequence:
    #     - lambda: UARTDebug::log_hex(direction, bytes, ':');

vitoconnect:
  uart_id: uart_vitoconnect
  protocol: P300                # set protocol to KW or P300
  update_interval: 30s

sensor:
  - platform: vitoconnect
    name: "Außentemperatur"
    address: 0x01C1             # vitoconnect: address of the value
    length: 2                   # vitoconnect: length of the value
    unit_of_measurement: "°C"
    accuracy_decimals: 1
    filters:
      - multiply: 0.1
  - platform: vitoconnect
    name: "Betriebsstunden Verdichter"
    address: 0x0580
    length: 4
    unit_of_measurement: "h"
    accuracy_decimals: 1
    filters:
      # - multiply: 0.000277777777777778 # use multiply filter for ESP8266
      - lambda: return x / 3600.0;
  - platform: vitoconnect
    name: "Brennerleistung"
    address: 0xA38F
    length: 1
    unit_of_measurement: "%"
    accuracy_decimals: 1
    filters:
      - multiply: 0.5
binary_sensor:
  - platform: vitoconnect
    name: "Status Verdichter"
    address: 0x0400
```

Tested with OptoLink ESP32 adapter from here:
<https://github.com/openv/openv/wiki/Bauanleitung-ESP32-Adafruit-Feather-Huzzah32-and-Proto-Wing>

## Per-sensor update intervals

The default polling interval is 30 seconds. Set `update_interval` on an individual `sensor` or `binary_sensor` to read a slowly changing value less often:

```yaml
sensor:
  - platform: vitoconnect
    name: "Betriebszeit Leiterplatte"
    address: 0x08FA
    length: 4
    update_interval: 120s
```

The `vitoconnect` hub's `update_interval` still controls entities without their own interval. Custom intervals begin after the first hub poll. Reads are scheduled at these intervals; the shared Optolink connection may deliver them later when other requests are queued. Write readbacks keep their priority.

## Writable values (ESP-IDF)

This fork provides native ESPHome `number`, `select`, `switch`, and `button` platforms under the existing `vitoconnect` name. The ESP32-C6 build uses `framework: {type: esp-idf}`. Existing sensors and binary sensors keep their behavior.

A control command queues an Optolink write. For `number`, `select`, and `switch`, the entity state is updated only from a subsequent read of the device; a failed write does not publish the requested value. A `button` sends its configured raw value once when pressed and has no state or readback. No value is written during startup. Use only addresses and byte lengths that are confirmed writable for your exact controller. The `tests/esp32c6_write.yaml` addresses are compile-only placeholders and must not be flashed to a heater.

`number`, `select`, and `switch` require `address` (write address) and accept `state_address` (read address, defaults to `address`). Both use the same `length`. Supported widths are 1 or 2 bytes for `select` and `switch`, and 1, 2 or 4 bytes for `number`.

- `number`: set `min_value`, `max_value`, and `step`; optional `scale` converts a raw integer to the displayed value (default 1), and `signed` enables signed two's-complement values (default false).
- `select`: configure `options` as a mapping from raw integer to unique display label.
- `switch`: optional `on_value` and `off_value` default to 1 and 0. Startup restoration is disabled, so reconnecting cannot replay an old switch value.
- `button`: set `address` and `value` (unsigned raw integer). Optional `length` defaults to 1 byte; 1, 2 and 4 bytes are supported. The value must fit the selected length. The button is never polled and sends exactly one write per press.

Example button configuration; replace the placeholder address and value with a confirmed write command before using it on a device:

```yaml
button:
  - platform: vitoconnect
    name: "Example write command"
    address: 0x1006  # Placeholder, not a heater address
    length: 1
    value: 1
```

The ESP32-C6 build test validates the existing sensor and all four writable platforms with ESP-IDF. It compiles firmware only; it does not connect to or write to a heating device.

## Credits

Built based on [VitoWifi] by [Bert Melis] and inspired by [vitowifi_esphome] by [Philipp Hack].

## License

MIT License
Copyright (c) 2023 Philipp Danner

## Support Development

### Paypal

[![](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=848P2G8EA68PJ)

[ESPHome]: https://esphome.io/
[VitoWifi]: https://github.com/bertmelis/VitoWifi
[vitowifi_esphome]: https://github.com/phha/vitowifi_esphome
[Bert Melis]: https://github.com/bertmelis
[Philipp Hack]: https://github.com/phha
