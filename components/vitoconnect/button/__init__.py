import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import CONF_ADDRESS, CONF_LENGTH, CONF_VALUE

from .. import CONF_VITOCONNECT_ID, VitoConnect, vitoconnect_ns

DEPENDENCIES = ["vitoconnect"]

OPTOLINKButton = vitoconnect_ns.class_("OPTOLINKButton", button.Button)


def _validate(config):
    length = config[CONF_LENGTH]
    if length not in (1, 2, 4):
        raise cv.Invalid("length must be 1, 2 or 4")
    if config[CONF_VALUE] > (1 << (8 * length)) - 1:
        raise cv.Invalid("value does not fit length")
    return config


CONFIG_SCHEMA = cv.All(
    button.button_schema(OPTOLINKButton).extend({
        cv.GenerateID(CONF_VITOCONNECT_ID): cv.use_id(VitoConnect),
        cv.Required(CONF_ADDRESS): cv.hex_uint16_t,
        cv.Optional(CONF_LENGTH, default=1): cv.int_range(min=1, max=4),
        cv.Required(CONF_VALUE): cv.uint32_t,
    }),
    _validate,
)


async def to_code(config):
    var = await button.new_button(config)
    hub = await cg.get_variable(config[CONF_VITOCONNECT_ID])
    cg.add(var.set_address(config[CONF_ADDRESS]))
    cg.add(var.set_length(config[CONF_LENGTH]))
    cg.add(var.set_value(config[CONF_VALUE]))
    cg.add(var.set_parent(hub))
