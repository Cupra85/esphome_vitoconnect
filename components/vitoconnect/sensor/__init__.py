import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_NAME, CONF_ADDRESS, CONF_LENGTH, CONF_UPDATE_INTERVAL #, CONF_TYPE
from .. import vitoconnect_ns, VitoConnect, CONF_VITOCONNECT_ID

DEPENDENCIES = ["vitoconnect"]
OPTOLINKSensor = vitoconnect_ns.class_("OPTOLINKSensor", sensor.Sensor)

CONF_BYTE_OFFSET = "byte_offset"
CONF_VALUE_LENGTH = "value_length"


def validate_value_slice(config):
    read_length = config[CONF_LENGTH]
    byte_offset = config.get(CONF_BYTE_OFFSET, 0)
    value_length = config.get(CONF_VALUE_LENGTH, read_length)
    if read_length < 1 or read_length > 10:
        raise cv.Invalid("length must be between 1 and 10 bytes")
    if value_length not in (1, 2, 4):
        raise cv.Invalid("value_length must be 1, 2 or 4 bytes")
    if byte_offset + value_length > read_length:
        raise cv.Invalid("byte_offset + value_length must not exceed length")
    return config


CONFIG_SCHEMA = cv.All(sensor.sensor_schema(OPTOLINKSensor).extend({
    cv.GenerateID(): cv.declare_id(OPTOLINKSensor),
    cv.GenerateID(CONF_VITOCONNECT_ID): cv.use_id(VitoConnect),
    cv.Required(CONF_ADDRESS): cv.uint16_t,
    cv.Required(CONF_LENGTH): cv.uint8_t,
    cv.Optional(CONF_BYTE_OFFSET, default=0): cv.uint8_t,
    cv.Optional(CONF_VALUE_LENGTH): cv.uint8_t,
    cv.Optional(CONF_UPDATE_INTERVAL): cv.positive_time_period_milliseconds,
}), validate_value_slice)

async def to_code(config):
    var = await sensor.new_sensor(config)

    # Add configuration to datapoint
    cg.add(var.setAddress(config[CONF_ADDRESS]))
    cg.add(var.setLength(config[CONF_LENGTH]))
    cg.add(var.setByteOffset(config[CONF_BYTE_OFFSET]))
    if CONF_VALUE_LENGTH in config:
        cg.add(var.setValueLength(config[CONF_VALUE_LENGTH]))
    if CONF_UPDATE_INTERVAL in config:
        cg.add(var.set_poll_interval(config[CONF_UPDATE_INTERVAL]))

    # Add sensor to component hub (VitoConnect)
    hub = await cg.get_variable(config[CONF_VITOCONNECT_ID])
    cg.add(hub.register_datapoint(var))
