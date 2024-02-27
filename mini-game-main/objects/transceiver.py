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
        self.receiver_state = 0 # 1 if the receiver is receiving IR signal, 0 otherwise

    def get_position(self):
        return (self.x, self.y)
    
    def get_state(self):
        return self.receiver_state
    
    def get_view_cone_vectors(self):
        return self.VIEW_CONE_VECTORS
    
    def get_facing_direction(self):
        return self.FACING_DIRECTION

    def set_off(self):
        self.receiver_state = 0
    
    def set_on(self):
        self.receiver_state = 1
    
class Transmitter():
    def __init__(self, transmitter_no:int, x:float, y:float, facing_direction_vector:np.ndarray, view_cone_vectors:list[np.ndarray]):
        self.TRANSMITTER_NO = transmitter_no
        self.FACING_DIRECTION = facing_direction_vector
        self.VIEW_CONE_VECTORS = view_cone_vectors
        self.x = x
        self.y = y

        self.transmitter_state = 0 #if the transmitter is emitting IR signal, this is 1, otherwise 0

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
    
    def move_x_y(self, del_x:float=0, del_y:float=0):
        self.x = self.x + del_x
        self.y = self.y + del_y

        self.__initiliaze_receivers()
        self.__initiliaze_transmitters()

    def rotate(self, angle:float):
        self.OFFSET_RAD = self.OFFSET_RAD + math.radians(angle)
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

                angle_rad = multiplier * self.TRANSMITTER_VIEW_CONE_RAD                
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
    
    def turn_off_transmitter(self, transmitter_no:int):
        if transmitter_no >= len(self.transmitters) or transmitter_no < 0:
            raise ValueError("Transmitter number is out of range")
        
        self.transmitters[transmitter_no].turn_off()

    def turn_off_all_transmitters(self):
        for transmitter in self.transmitters:
            transmitter.turn_off()

    def turn_on_all_transmitters(self):
        for transmitter in self.transmitters:
            transmitter.turn_on()
        
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

    def get_number_of_receivers(self):
        return len(self.receivers)
    
    def get_number_of_transmitters(self):
        return len(self.transmitters)
    
    def draw_transceiver(self, frame, img_size = None, edge_length_m = None, M_TO_PX = None):
        TRANSMITTER_CONE_SCALE = 1
        RECEIVER_CONE_SCALE = 0.75
        RECEIVER_DOT_RADIUS= 0.05
        TRANSMITTER_DOT_RADIUS = 0.03
        TRANSCEIVER_FRONT_ARROW_SCALE = 1.5
        OFFSET_M = edge_length_m / 2 #same for x and y

        func_x_to_px = lambda x: int((x+OFFSET_M)*M_TO_PX)
        func_y_to_px = lambda y: int((OFFSET_M-y)*M_TO_PX)

        x_px = func_x_to_px(self.x)
        y_px = func_y_to_px(self.y)           
        cv2.circle(frame, (x_px, y_px), int(self.TRANSCEIVER_RADIUS * M_TO_PX), (125, 125, 125), -1)      

        #draw the receivers
        for receiver in self.receivers:
            x_px = func_x_to_px(receiver.x)
            y_px = func_y_to_px(receiver.y)

            receiver_color = (0, 255, 0) if receiver.get_state() == 1 else (75, 75, 75)            

            if receiver.get_state() == 1:
                cv2.circle(frame, (x_px, y_px), int((RECEIVER_DOT_RADIUS) * M_TO_PX), receiver_color, -1)
                triangle_coordinates = [ [x_px,y_px]]
                for vector in receiver.VIEW_CONE_VECTORS:
                    end_x = func_x_to_px(receiver.x + vector[0]*RECEIVER_CONE_SCALE)
                    end_y = func_y_to_px(receiver.y + vector[1]*RECEIVER_CONE_SCALE)
                    triangle_coordinates.append([end_x, end_y])

                cv2.polylines(frame, [np.array(triangle_coordinates)], isClosed=True, color=receiver_color, thickness=1)
            else:
                cv2.circle(frame, (x_px, y_px), int((RECEIVER_DOT_RADIUS) * M_TO_PX), receiver_color, 2)

        #draw the transmitters
        for transmitter in self.transmitters:
            x_px = func_x_to_px(transmitter.x)
            y_px = func_y_to_px(transmitter.y)

            transmitter_color = (0, 0, 255) if transmitter.get_state() == 1 else (255, 0, 0)

            if transmitter.get_state() == 1:
                cv2.circle(frame, (x_px, y_px), int((TRANSMITTER_DOT_RADIUS) * M_TO_PX), transmitter_color, -1)
                triangle_coordinates = [ [x_px,y_px]]
                for vector in transmitter.VIEW_CONE_VECTORS:
                    end_x = func_x_to_px(transmitter.x + vector[0]*TRANSMITTER_CONE_SCALE)
                    end_y = func_y_to_px(transmitter.y + vector[1]*TRANSMITTER_CONE_SCALE)
                    triangle_coordinates.append([end_x, end_y])
   
                cv2.polylines(frame, [np.array(triangle_coordinates)], isClosed=True, color=transmitter_color, thickness=1)
            else:
                cv2.circle(frame, (x_px, y_px), int((TRANSMITTER_DOT_RADIUS) * M_TO_PX), transmitter_color, 1)

        #draw direction vector of the robot 
        robot_front_direction = np.array([math.cos(self.OFFSET_RAD ), math.sin(self.OFFSET_RAD)])
        x_px = func_x_to_px(self.x)
        y_px = func_y_to_px(self.y)
        end_x_px = func_x_to_px(self.x + robot_front_direction[0]*self.TRANSCEIVER_RADIUS*TRANSCEIVER_FRONT_ARROW_SCALE)
        end_y_px = func_y_to_px(self.y + robot_front_direction[1]*self.TRANSCEIVER_RADIUS*TRANSCEIVER_FRONT_ARROW_SCALE)
        cv2.line(frame, (x_px, y_px), (end_x_px, end_y_px), (0, 0, 0), 2)
        
        #draw the ID of the robot
        half_px = int((self.TRANSCEIVER_RADIUS/5) * M_TO_PX)   
        cv2.putText(frame, str(self.ID), (x_px-half_px, y_px+half_px), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)

    def update_receiver_states(self, units):
        for receiver in self.receivers:
            receiver.set_off()

            for unit in units:
                if unit == self:
                    continue

                for transmitter in unit.transmitters:
                    if transmitter.get_state() == 0:
                        continue                    

                    #=============================================================================================================
                    #check if the receiver is in the transmitter's view cone
                    is_receiver_in_transmitters_view_cone = False

                    receiver_pos = receiver.get_position()
                    transmitter_pos = transmitter.get_position()

                    v_t1, v_t2 = transmitter.get_view_cone_vectors()

                    #receiver_pos + a * v_t1 + b * v_t2 = transmitter_pos
                    # if A and B are both positive, the receiver is in the view cone

                    #solve the system of equations
                    A = np.array([
                        [v_t1[0], v_t2[0]],
                        [v_t1[1], v_t2[1]]
                    ])
                    B = np.array([
                        receiver_pos[0] - transmitter_pos[0],
                        receiver_pos[1] - transmitter_pos[1]
                    ])

                    a, b = np.linalg.solve(A, B)

                    if a > 0 and b > 0:
                        is_receiver_in_transmitters_view_cone = True

                    #=============================================================================================================
                    #check if the transmitter is in the receiver's view cone
                    is_transmitter_in_receivers_view_cone = False

                    v_r1, v_r2 = receiver.get_view_cone_vectors()

                    #transmitter_pos + a * v_r1 + b * v_r2 = receiver_pos
                    # if A and B are both positive, the transmitter is in the view cone

                    #solve the system of equations
                    A = np.array([
                        [v_r1[0], v_r2[0]],
                        [v_r1[1], v_r2[1]]
                    ])
                    B = np.array([
                        transmitter_pos[0] - receiver_pos[0],
                        transmitter_pos[1] - receiver_pos[1]
                    ])

                    a, b = np.linalg.solve(A, B)

                    if a > 0 and b > 0:
                        is_transmitter_in_receivers_view_cone = True

                    # #=============================================================================================================
                    # #check if there is an obstacle between the transmitter and the receiver
                    # is_obstacle_in_between = False
                    # #TODO: implement this part

                    #=============================================================================================================
                    if is_receiver_in_transmitters_view_cone and is_transmitter_in_receivers_view_cone:
                        receiver.set_on()
                        break

    def get_receiver_states(self):
        return [receiver.get_state() for receiver in self.receivers]
         




        

    



    