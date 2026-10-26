import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ADDRESS, CONF_LENGTH, CONF_ON_VALUE

from .. import CONF_VITOCONNECT_ID, VitoConnect, vitoconnect_ns

DEPENDENCIES = ["vitoconnect"]
CONF_OFF_VALUE = "off_value"
CONF_STATE_ADDRESS = "state_address"

OPTOLINKSwitch = vitoconnect_ns.class_("OPTOLINKSwitch", switch.Switch)


def _validate(config):
    on_value = config[CONF_ON_VALUE]
    off_value = config[CONF_OFF_VALUE]
    maximum = (1 << (8 * config[CONF_LENGTH])) - 1
    if on_value == off_value:
        raise cv.Invalid("on_value and off_value must differ")
    if on_value > maximum or off_value > maximum:
        raise cv.Invalid("on_value or off_value does not fit length")
    return config


CONFIG_SCHEMA = cv.All(
    switch.switch_schema(
        OPTOLINKSwitch, block_inverted=True, default_restore_mode="DISABLED"
    ).extend({
        cv.GenerateID(CONF_VITOCONNECT_ID): cv.use_id(VitoConnect),
        cv.Required(CONF_ADDRESS): cv.hex_uint16_t,
        cv.Optional(CONF_STATE_ADDRESS): cv.hex_uint16_t,
        cv.Optional(CONF_LENGTH, default=1): cv.int_range(min=1, max=2),
        cv.Optional(CONF_ON_VALUE, default=1): cv.uint32_t,
        cv.Optional(CONF_OFF_VALUE, default=0): cv.uint32_t,
    }),
    _validate,
)


async def to_code(config):
    var = await switch.new_switch(config)
    hub = await cg.get_variable(config[CONF_VITOCONNECT_ID])
    cg.add(var.setAddress(config.get(CONF_STATE_ADDRESS, config[CONF_ADDRESS])))
    cg.add(var.setLength(config[CONF_LENGTH]))
    cg.add(var.set_write_address(config[CONF_ADDRESS]))
    cg.add(var.set_on_value(config[CONF_ON_VALUE]))
    cg.add(var.set_off_value(config[CONF_OFF_VALUE]))
    cg.add(var.set_parent(hub))
    cg.add(hub.register_datapoint(var))
