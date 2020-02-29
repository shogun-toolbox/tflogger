from tflogger import EventLogger
from tflogger.pb import Event, Summary

logger = EventLogger("tlpy")

event = Event()
event.wall_time = 1111;
event.step = 1;
summary_val = Summary.Value()
summary_val.tag = "tag0"
summary_val.simple_value = 11
event.summary.value.append(summary_val)

logger.write_event(event)
logger.close()
