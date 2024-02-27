import pprint

from classes.transceiver import TransceiverUnit
base_station = TransceiverUnit(
    id = 0, 
    x=0, 
    y=0, 
    number_of_sections = 3, 
    section_offset_angle = 0, 
    transceiver_radius = 0.100,
    receiver_placement_radius = 0.075,
    receiver_view_cone_angle = 75.0, 
    transmitter_placement_radius = 0.075,
    transmitter_view_cone_angle = 30.0
)

pprint.pprint(base_station.get_transceiver_info())



                 
 
