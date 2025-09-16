import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_NAME, CONF_ADDRESS, CONF_LENGTH, CONF_UPDATE_INTERVAL
from .. import vitoconnect_ns, VitoConnect, CONF_VITOCONNECT_ID

DEPENDENCIES = ["vitoconnect"]

OPTOLINKSensor = vitoconnect_ns.class_("OPTOLINKSensor", sensor.Sensor)

CONFIG_SCHEMA = sensor.sensor_schema(OPTOLINKSensor).extend({
    cv.GenerateID(): cv.declare_id(OPTOLINKSensor),
    cv.GenerateID(CONF_VITOCONNECT_ID): cv.use_id(VitoConnect),
    cv.Required(CONF_ADDRESS): cv.uint16_t,
    cv.Required(CONF_LENGTH): cv.uint8_t,
    cv.Optional(CONF_UPDATE_INTERVAL, default="60s"): cv.positive_time_period_milliseconds,
})

async def to_code(config):
    var = await sensor.new_sensor(config)

    # Add configuration to datapoint
    cg.add(var.setAddress(config[CONF_ADDRESS]))
    cg.add(var.setLength(config[CONF_LENGTH]))
    cg.add(var.setPollInterval(config[CONF_UPDATE_INTERVAL]))

    # Add sensor to component hub (VitoConnect)
    hub = await cg.get_variable(config[CONF_VITOCONNECT_ID])
    cg.add(hub.register_datapoint(var))
