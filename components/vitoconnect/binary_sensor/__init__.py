import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID, CONF_ADDRESS
from .. import vitoconnect_ns, VitoConnect, CONF_VITOCONNECT_ID

DEPENDENCIES = ["vitoconnect"]

OPTOLINKBinarySensor = vitoconnect_ns.class_("OPTOLINKBinarySensor", binary_sensor.BinarySensor)

CONF_POLL_INTERVAL = "poll_interval"

CONFIG_SCHEMA = binary_sensor.BINARY_SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(OPTOLINKBinarySensor),
    cv.GenerateID(CONF_VITOCONNECT_ID): cv.use_id(VitoConnect),
    cv.Required(CONF_ADDRESS): cv.uint16_t,
    cv.Optional(CONF_POLL_INTERVAL, default="60s"): cv.positive_time_period_milliseconds,
})

async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)

    # Add configuration to datapoint
    cg.add(var.setAddress(config[CONF_ADDRESS]))
    cg.add(var.setLength(1))
    cg.add(var.setPollInterval(config[CONF_POLL_INTERVAL]))

    # Add sensor to component hub (VitoConnect)
    hub = await cg.get_variable(config[CONF_VITOCONNECT_ID])
    cg.add(hub.register_datapoint(var))

