import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import CONF_ADDRESS, CONF_LENGTH, CONF_OPTIONS

from .. import CONF_VITOCONNECT_ID, VitoConnect, vitoconnect_ns

DEPENDENCIES = ["vitoconnect"]
CONF_STATE_ADDRESS = "state_address"

OPTOLINKSelect = vitoconnect_ns.class_("OPTOLINKSelect", select.Select)


def _validate(config):
    options = config[CONF_OPTIONS]
    if not options:
        raise cv.Invalid("at least one option is required")
    if len(set(options.values())) != len(options):
        raise cv.Invalid("option labels must be unique")
    maximum = (1 << (8 * config[CONF_LENGTH])) - 1
    if any(raw < 0 or raw > maximum for raw in options):
        raise cv.Invalid("option value does not fit length")
    return config


CONFIG_SCHEMA = cv.All(
    select.select_schema(OPTOLINKSelect).extend({
        cv.GenerateID(CONF_VITOCONNECT_ID): cv.use_id(VitoConnect),
        cv.Required(CONF_ADDRESS): cv.hex_uint16_t,
        cv.Optional(CONF_STATE_ADDRESS): cv.hex_uint16_t,
        cv.Optional(CONF_LENGTH, default=1): cv.int_range(min=1, max=2),
        cv.Required(CONF_OPTIONS): cv.Schema({cv.int_: cv.string}),
    }),
    _validate,
)


async def to_code(config):
    options = config[CONF_OPTIONS]
    var = await select.new_select(config, options=list(options.values()))
    hub = await cg.get_variable(config[CONF_VITOCONNECT_ID])
    cg.add(var.setAddress(config.get(CONF_STATE_ADDRESS, config[CONF_ADDRESS])))
    cg.add(var.setLength(config[CONF_LENGTH]))
    cg.add(var.set_write_address(config[CONF_ADDRESS]))
    for raw in options:
        cg.add(var.add_raw_value(raw))
    cg.add(var.set_parent(hub))
    cg.add(hub.register_datapoint(var))
