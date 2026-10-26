import math

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import CONF_ADDRESS, CONF_LENGTH, CONF_MAX_VALUE, CONF_MIN_VALUE, CONF_STEP

from .. import CONF_VITOCONNECT_ID, VitoConnect, vitoconnect_ns

DEPENDENCIES = ["vitoconnect"]
CONF_SCALE = "scale"
CONF_SIGNED = "signed"
CONF_STATE_ADDRESS = "state_address"

OPTOLINKNumber = vitoconnect_ns.class_("OPTOLINKNumber", number.Number)


def _validate(config):
    length = config[CONF_LENGTH]
    if length not in (1, 2, 4):
        raise cv.Invalid("length must be 1, 2 or 4")
    if config[CONF_MIN_VALUE] > config[CONF_MAX_VALUE]:
        raise cv.Invalid("min_value must not exceed max_value")
    bits = 8 * length
    signed = config[CONF_SIGNED]
    low = -(1 << (bits - 1)) if signed else 0
    high = (1 << (bits - 1)) - 1 if signed else (1 << bits) - 1
    scale = config[CONF_SCALE]
    for key in (CONF_MIN_VALUE, CONF_MAX_VALUE):
        raw = config[key] / scale
        if not math.isfinite(raw) or raw < low - 0.5 or raw > high + 0.5:
            raise cv.Invalid(f"{key} cannot be encoded in {length} byte(s)")
    return config


CONFIG_SCHEMA = cv.All(
    number.number_schema(OPTOLINKNumber).extend({
        cv.GenerateID(CONF_VITOCONNECT_ID): cv.use_id(VitoConnect),
        cv.Required(CONF_ADDRESS): cv.hex_uint16_t,
        cv.Optional(CONF_STATE_ADDRESS): cv.hex_uint16_t,
        cv.Required(CONF_LENGTH): cv.int_range(min=1, max=4),
        cv.Required(CONF_MIN_VALUE): cv.float_,
        cv.Required(CONF_MAX_VALUE): cv.float_,
        cv.Required(CONF_STEP): cv.positive_float,
        cv.Optional(CONF_SCALE, default=1.0): cv.positive_float,
        cv.Optional(CONF_SIGNED, default=False): cv.boolean,
    }),
    _validate,
)


async def to_code(config):
    var = await number.new_number(
        config,
        min_value=config[CONF_MIN_VALUE],
        max_value=config[CONF_MAX_VALUE],
        step=config[CONF_STEP],
    )
    hub = await cg.get_variable(config[CONF_VITOCONNECT_ID])
    cg.add(var.setAddress(config.get(CONF_STATE_ADDRESS, config[CONF_ADDRESS])))
    cg.add(var.setLength(config[CONF_LENGTH]))
    cg.add(var.set_write_address(config[CONF_ADDRESS]))
    cg.add(var.set_scale(config[CONF_SCALE]))
    cg.add(var.set_signed(config[CONF_SIGNED]))
    cg.add(var.set_bounds(config[CONF_MIN_VALUE], config[CONF_MAX_VALUE]))
    cg.add(var.set_parent(hub))
    cg.add(hub.register_datapoint(var))
