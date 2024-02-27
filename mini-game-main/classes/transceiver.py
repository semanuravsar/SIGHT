import math, pprint
import cv2
import numpy as np


class Receiver():
    def __init__(self, receiver_no:int, x:float, y:float, facing_direction_vector:np.ndarray, view_cone_vectors:list[np.ndarray]):
        self.RECEIVER_NO = receiver_no
        self.FACING_DIRECTION = facing_direction_vector
        self.VIEW_CONE_VECTORS = view_cone_vectors
        self.x = x
        self.y = y

        self.receiver_state = 1 # 1 if the receiver is receiving IR signal, 0 otherwise

    def get_position(self):
        return (self.x, self.y)
    
    def get_state(self):
        return self.receiver_state
    
    def get_view_cone_vectors(self):
        return self.VIEW_CONE_VECTORS
    
    def get_facing_direction(self):
        return self.FACING_DIRECTION

class Transmitter():
    def __init__(self, transmitter_no:int, x:float, y:float, facing_direction_vector:np.ndarray, view_cone_vectors:list[np.ndarray]):
        self.TRANSMITTER_NO = transmitter_no
        self.FACING_DIRECTION = facing_direction_vector
        self.VIEW_CONE_VECTORS = view_cone_vectors
        self.x = x
        self.y = y

        self.transmitter_state = 1 #if the transmitter is emitting IR signal, this is 1, otherwise 0

    def turn_on(self):
        self.transmitter_state = 1
    
    def turn_off(self):
        self.transmitter_state = 0

    def get_position(self):
        return (self.x, self.y)
    
    def get_state(self):
        return self.transmitter_state
    
    def get_view_cone_vectors(self):
        return self.VIEW_CONE_VECTORS
    
    def get_facing_direction(self):
        return self.FACING_DIRECTION

class TransceiverUnit():
    def __init__(self, id, x:float, y:float, number_of_sections:int, section_offset_angle:float = 0.0, transceiver_radius:float = 0.0, receiver_placement_radius:float = 0.0, receiver_view_cone_angle:float = 75.0, transmitter_placement_radius:float = 0.0, transmitter_view_cone_angle:float = 30.0):
        self.ID = id
        self.TRANSCEIVER_RADIUS = transceiver_radius
        self.NUMBER_OF_SECTIONS = number_of_sections
        self.RAD_PER_SECTION = 2 * math.pi / self.NUMBER_OF_SECTIONS
        self.OFFSET_RAD = math.radians(section_offset_angle)
        self.x = x
        self.y = y

        self.receivers:list[Receiver] = []
        self.RECEIVER_PLACEMENT_RADIUS = receiver_placement_radius
        self.RECEIVER_VIEW_CONE_RAD = math.radians(receiver_view_cone_angle)

        self.transmitters:list[Transmitter] = []
        self.TRANSMITTER_PLACEMENT_RADIUS = transmitter_placement_radius
        self.TRANSMITTER_VIEW_CONE_RAD = math.radians(transmitter_view_cone_angle)

        self.__initiliaze_receivers()
        self.__initiliaze_transmitters()
    
    def __initiliaze_receivers(self):
        self.receivers = []
        for i in range(self.NUMBER_OF_SECTIONS):            
            radian_wrt_center = self.RAD_PER_SECTION * i + self.OFFSET_RAD
            x_vector = math.cos(radian_wrt_center)
            y_vector = math.sin(radian_wrt_center)

            facing_direction = np.array([x_vector, y_vector])

            view_cone_vectors = []
            for multiplier in [-0.5, 0.5]:

                angle_rad = multiplier * self.RECEIVER_VIEW_CONE_RAD                
                rotation_matrix = np.array([
                    [np.cos(angle_rad), -np.sin(angle_rad)],
                    [np.sin(angle_rad), np.cos(angle_rad)]
                ])    
                rotated_vector = np.dot(rotation_matrix, facing_direction)
                view_cone_vectors.append(rotated_vector)
            
            x_rec = self.x + facing_direction[0] * self.RECEIVER_PLACEMENT_RADIUS
            y_rec = self.y + facing_direction[1] * self.RECEIVER_PLACEMENT_RADIUS   

            self.receivers.append(Receiver(receiver_no = i,x = x_rec, y = y_rec, facing_direction_vector= facing_direction, view_cone_vectors = view_cone_vectors))

    def __initiliaze_transmitters(self):
        self.transmitters = []
        for i in range(self.NUMBER_OF_SECTIONS):            
            radian_wrt_center = self.RAD_PER_SECTION * i + self.OFFSET_RAD
            x_vector = math.cos(radian_wrt_center)
            y_vector = math.sin(radian_wrt_center)

            facing_direction = np.array([x_vector, y_vector])

            view_cone_vectors = []
            # Rotation matrix
            for multiplier in [-0.5, 0.5]:

                angle_rad = multiplier * self.RECEIVER_VIEW_CONE_RAD                
                rotation_matrix = np.array([
                    [np.cos(angle_rad), -np.sin(angle_rad)],
                    [np.sin(angle_rad), np.cos(angle_rad)]
                ])    

                rotated_vector = np.dot(rotation_matrix, facing_direction)
                view_cone_vectors.append(rotated_vector)
            
            x_trans = self.x + facing_direction[0] * self.TRANSMITTER_PLACEMENT_RADIUS
            y_trans = self.y + facing_direction[1] * self.TRANSMITTER_PLACEMENT_RADIUS    

            self.transmitters.append(Transmitter(transmitter_no = i,x = x_trans, y = y_trans, facing_direction_vector= facing_direction, view_cone_vectors = view_cone_vectors))
        
    def turn_on_transmitter(self, transmitter_no:int):
        if transmitter_no >= len(self.transmitters) or transmitter_no < 0:
            raise ValueError("Transmitter number is out of range")        
        self.transmitters[transmitter_no].turn_on()
    
    def turn_off_all_transmitters(self):
        for transmitter in self.transmitters:
            transmitter.turn_off()

    def get_transceiver_info(self):
        return_dict = {
            "id": self.ID,
            "center_x": self.x,
            "center_y": self.y,

            "receivers": [],
            "transmitters": []
        }

        for receiver in self.receivers:
            return_dict["receivers"].append({
                "x": receiver.get_position()[0],
                "y": receiver.get_position()[1],
                "facing_direction": receiver.get_facing_direction(),
                "view_cone_vectors": receiver.get_view_cone_vectors(),
                "state": receiver.get_state()
            })
        
        for transmitter in self.transmitters:
            return_dict["transmitters"].append({
                "x": transmitter.get_position()[0],
                "y": transmitter.get_position()[1],
                "facing_direction": transmitter.get_facing_direction(),
                "view_cone_vectors": transmitter.get_view_cone_vectors(),
                "state": transmitter.get_state()
            })
        
        return return_dict

    def draw_transceiver(self, frame, img_size = None, edge_length_m = None, M_TO_PX = None):
        offset_m = edge_length_m / 2 #same for x and y

        x_px = int( (self.x + offset_m) * M_TO_PX)
        y_px = int( (offset_m-self.y) * M_TO_PX)

        cv2.circle(frame, (x_px, y_px), int(self.TRANSCEIVER_RADIUS * M_TO_PX), (125, 125, 125), -1)      
        half_px = int((self.TRANSCEIVER_RADIUS/5) * M_TO_PX)   
        cv2.putText(frame, str(self.ID), (x_px-half_px, y_px+half_px), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)
        
    def draw_transceiver_sensors(self, frame, img_size = None, edge_length_m = None, M_TO_PX = None):
        RECEIVER_CONE_SCALE = 1
        RECEIVER_DOT_RADIUS= 0.05
        TRANSMITTER_DOT_RADIUS = 0.03

        offset_m = edge_length_m / 2 #same for x and y

        #draw the receivers
        for receiver in self.receivers:
            x_px = int( (receiver.x + offset_m) * M_TO_PX)
            y_px = int( (offset_m-receiver.y) * M_TO_PX)

            receiver_color = (0, 255, 0) if receiver.get_state() == 1 else (75, 75, 75)            

            if receiver.get_state() == 1:
                cv2.circle(frame, (x_px, y_px), int((RECEIVER_DOT_RADIUS) * M_TO_PX), receiver_color, -1)
                triangle_coordinates = [ [x_px,y_px]]
                for vector in receiver.VIEW_CONE_VECTORS:
                    end_x = int( (receiver.x + offset_m + vector[0]*RECEIVER_CONE_SCALE) * M_TO_PX)
                    end_y = int( (offset_m -(receiver.y + vector[1]*RECEIVER_CONE_SCALE)) * M_TO_PX)
                    triangle_coordinates.append([end_x, end_y])

                #cv2.polylines(frame, [np.array(triangle_coordinates)], isClosed=True, color=receiver_color, thickness=1)
            else:
                cv2.circle(frame, (x_px, y_px), int((RECEIVER_DOT_RADIUS) * M_TO_PX), receiver_color, 2)


        #draw the transmitters
        for transmitter in self.transmitters:
            x_px = int( (transmitter.x + offset_m) * M_TO_PX)
            y_px = int( (offset_m-transmitter.y) * M_TO_PX)

            transmitter_color = (0, 0, 255) if transmitter.get_state() == 1 else (255, 0, 0)

            if transmitter.get_state() == 1:
                cv2.circle(frame, (x_px, y_px), int((TRANSMITTER_DOT_RADIUS) * M_TO_PX), transmitter_color, -1)
                triangle_coordinates = [ [x_px,y_px]]
                for vector in transmitter.VIEW_CONE_VECTORS:
                    end_x = int( (receiver.x + offset_m + vector[0]*RECEIVER_CONE_SCALE) * M_TO_PX)
                    end_y = int( (offset_m -(receiver.y + vector[1]*RECEIVER_CONE_SCALE)) * M_TO_PX)
                    triangle_coordinates.append([end_x, end_y])
                cv2.polylines(frame, [np.array(triangle_coordinates)], isClosed=True, color=transmitter_color, thickness=1)
            else:
                cv2.circle(frame, (x_px, y_px), int((TRANSMITTER_DOT_RADIUS) * M_TO_PX), transmitter_color, 1)


        

    



    