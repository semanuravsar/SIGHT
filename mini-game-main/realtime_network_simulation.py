import pprint
from classes.transceiver import TransceiverUnit

import numpy as np
import cv2


def display_current_frame(img_size = 750, edge_length_m = 2.75, units = [], obstacles = [], frame_time_ms = 250):    
    GRID_SIZE = 9
    M_TO_PX = img_size/edge_length_m

    frame = np.full((img_size, img_size, 3), 255, dtype=np.uint8)  # Light gray background
    spacing_px = int(img_size / 11)
    cv2.rectangle(frame, (spacing_px, spacing_px), (img_size-spacing_px, img_size-spacing_px), (0, 0, 0), 2)
    # Draw the grid lines
    for i in range(1, 9):
        # Vertical lines
        cv2.line(frame, (spacing_px + i * spacing_px, spacing_px), (spacing_px + i * spacing_px, img_size - spacing_px), (0, 0, 0), 1)
        # Horizontal lines
        cv2.line(frame, (spacing_px, spacing_px + i * spacing_px), (img_size - spacing_px, spacing_px + i * spacing_px), (0, 0, 0), 1)

    for transceiver in units:
        transceiver.draw_transceiver(frame, img_size = img_size, edge_length_m = edge_length_m, M_TO_PX = M_TO_PX)   

    cv2.imshow('simulation', frame)
    cv2.waitKey(frame_time_ms)
    

units = []

base_station = TransceiverUnit(
    id = 0, 
    x=0, 
    y=0, 
    number_of_sections = 18, 
    section_offset_angle = 0, 
    transceiver_radius = 0.17,
    receiver_placement_radius = 0.10,
    receiver_view_cone_angle = 75.0, 
    transmitter_placement_radius = 0.15,
    transmitter_view_cone_angle = 30
)
units.append(base_station)

slave_1 = TransceiverUnit(
    id = 1, 
    x=0.25*3, 
    y=0.25*3, 
    number_of_sections = 6, 
    section_offset_angle = 0, 
    transceiver_radius = 0.17,
    receiver_placement_radius = 0.10,
    receiver_view_cone_angle = 75.0, 
    transmitter_placement_radius = 0.15,
    transmitter_view_cone_angle = 30
)


units.append(slave_1)

counter = 0
while True:
    base_station.turn_off_all_transmitters()

    base_station.turn_on_transmitter(counter%base_station.get_number_of_transmitters())  

    display_current_frame(units = units, frame_time_ms = 250)

    counter += 1


cv2.destroyAllWindows()
                 
 
