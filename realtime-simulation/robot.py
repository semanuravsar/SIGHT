import numpy as np

def ____rotate_vector(vector: np.ndarray, angle_degrees: float) -> np.ndarray:
    """
    Rotates a vector by a given angle in degrees in the clockwise direction
    """
    angle_radians = np.radians(angle_degrees)
    rotation_matrix = np.array([[np.cos(angle_radians), -np.sin(angle_radians)],
                                    [np.sin(angle_radians), np.cos(angle_radians)]])
    return rotation_matrix @ vector    
np.ext_rotate_vector = ____rotate_vector # Add the function to the numpy module

#======================================================================================================================

class Robot():
    def __init__(self, id:int = None, robot_radius:float = None, position: np.ndarray = None, sensor_dict: dict = None):
                self.ID = id
                self.SENSOR_DICT = sensor_dict   
                self.ROBOT_RADIUS = robot_radius # The radius of the robot in meters 
                self.position = position # x,y position of the robot w.r.t the world frame in meters
                self.current_rotation = 0 # The angle of rotation of the robot w.r.t the world frame in degrees in clockwise direction
                self.facing_direction = np.array([1.0,0.0]) # The direction in which the robot is facing as a unit vector

                self.initiliaze_sensors()

    def initiliaze_sensors(self):
        self.receivers_list = []
        self.transmitters_list = []
        self.ultrasonic_sensors_list = []

        if "receiver" in self.SENSOR_DICT:
                #the dictionary contains: number_of_receivers, receiver_placement_radius, receiver_placement_offset_angle, view_angle              

                NUMBER_OF_RECEIVERS = self.SENSOR_DICT["receiver"]["number_of_receivers"]                    
                PLACEMENT_RADIUS = self.SENSOR_DICT["receiver"]["receiver_placement_radius"]
                OFFSET_ANGLE= self.SENSOR_DICT["receiver"]["receiver_placement_offset_angle"]
                VIEW_ANGLE = self.SENSOR_DICT["receiver"]["view_angle"]
                _ANGLE_PER_RECEIVER = 360.0 / NUMBER_OF_RECEIVERS

                for i in range(NUMBER_OF_RECEIVERS):                        
                    rotate_angle = i*_ANGLE_PER_RECEIVER + OFFSET_ANGLE
                    rotated_relative_position_vector = np.ext_rotate_vector(  np.array([PLACEMENT_RADIUS, 0.0]), rotate_angle)      
                    receiver_i = Receiver(robot_object=self, sensor_id = i, relative_position_vector=rotated_relative_position_vector, view_angle= VIEW_ANGLE)
                    print(receiver_i)
                    self.receivers_list.append(receiver_i)
         
        if "transmitter" in self.SENSOR_DICT:
                #the dictionary contains: number_of_transmitters, transmitter_placement_radius, transmitter_placement_offset_angle
                NUMBER_OF_TRANSMITTERS = self.SENSOR_DICT["transmitter"]["number_of_transmitters"]                    
                PLACEMENT_RADIUS = self.SENSOR_DICT["transmitter"]["transmitter_placement_radius"]
                OFFSET_ANGLE= self.SENSOR_DICT["transmitter"]["transmitter_placement_offset_angle"]
                BEAM_ANGLE = self.SENSOR_DICT["transmitter"]["beam_angle"]
                _ANGLE_PER_TRANSMITTER = 360.0 / NUMBER_OF_TRANSMITTERS

                for i in range(NUMBER_OF_TRANSMITTERS):
                    rotate_angle = i*_ANGLE_PER_TRANSMITTER + OFFSET_ANGLE
                    rotated_relative_position_vector = np.ext_rotate_vector(  np.array([PLACEMENT_RADIUS, 0.0]), rotate_angle)
                    transmitter_i = Transmitter(robot_object=self, sensor_id = i, relative_position_vector=rotated_relative_position_vector, beam_angle= BEAM_ANGLE)
                    print(transmitter_i)
                    self.transmitters_list.append(transmitter_i)

        if "ultrasonic_sensor" in self.SENSOR_DICT:
                #the dictionary contains: placement_angles, ultrasonic_sensor_placement_radius
                PLACEMENT_ANGLES = self.SENSOR_DICT["ultrasonic_sensor"]["placement_angles"]                    
                PLACEMENT_RADIUS = self.SENSOR_DICT["ultrasonic_sensor"]["ultrasonic_sensor_placement_radius"]

                for i, placement_angle in enumerate(PLACEMENT_ANGLES):                 
                    rotated_relative_position_vector = np.ext_rotate_vector(  np.array([PLACEMENT_RADIUS, 0.0]), placement_angle)
                    ultrasonic_sensor_i = UltrasonicSensor(robot_object=self, sensor_id = i, relative_position_vector=rotated_relative_position_vector)
                    print(ultrasonic_sensor_i)
                    self.ultrasonic_sensors_list.append(ultrasonic_sensor_i)

    def get_id(self)->int:
        """
        Returns the id of the robot
        """
        return self.ID
    
    def get_current_rotation_angle(self)->float:
        """
        Returns the current rotation angle of the robot in degrees as a float
        """
        return self.current_rotation    
    
    def get_position(self)->np.ndarray:
        """
        Returns the current (x,y) position of the robot in meters as a numpy array
        """
        return self.position
    
    def move_robot(self, del_x: float=0, del_y:float=0):
        self.position[0] += del_x
        self.position[1] += del_y        
    
    def rotate_robot(self,rotate_angle: float=0):
        self.current_rotation = (self.current_rotation + rotate_angle)%360
    
class Sensor():
    def __init__(self,  robot_object: Robot = None, sensor_id:int = None, relative_position_vector:np.ndarray = None) -> None:
        """
        robot_object: The robot object to which the sensor is attached
        sensor_id: The id of the sensor
        relative_position_vector: A vector starting from the center of the robot and ending at the position of the sensor in meters
        """
        self.ROBOT_OBJECT = robot_object
        self.SENSOR_ID = sensor_id
        self.RELATIVE_POSITION_VECTOR = relative_position_vector
    
    def get_sensor_id(self) -> int:
        """
        Returns the id of the sensor
        """
        return self.SENSOR_ID
    
    def get_relative_position_vector(self) -> np.ndarray:
        """
        Returns the relative position vector of the sensor w.r.t the robot in meters as a numpy array
        """
        return self.RELATIVE_POSITION_VECTOR
    
    def get_position(self) -> None:
        """
        Returns the current position of the sensor in meters as a numpy array
        """

        #apply the rotation matrix to the relative position vector
        current_rotation_angle = self.ROBOT_OBJECT.get_current_rotation_angle()
        rotated_relative_position_vector = np.__rotate_vector(self.RELATIVE_POSITION_VECTOR, current_rotation_angle)

        #apply the translation to the rotated position vector
        current_position = self.ROBOT_OBJECT.get_position()
        current_position += rotated_relative_position_vector       
        
class Receiver(Sensor):
    def __init__(self, robot_object: Robot = None, sensor_id:int = None, relative_position_vector: np.ndarray = None, view_angle:float =  None ) -> None:
        #initilize the parent class
        super().__init__(robot_object=robot_object, sensor_id=sensor_id, relative_position_vector=relative_position_vector)
        self.VIEW_ANGLE = view_angle

        self.receiver_output = 1 #  0: detecting a signal, 1: not detecting any signal,

    def __str__(self):
        return f"Robot {self.ROBOT_OBJECT.get_id()} | Receiver: {self.get_sensor_id()}, relative position: {self.get_relative_position_vector()}"

    def read_receiver_output(self):
        return self.receiver_output
    
    def update_receiver_output(self):
        #TODO: Implement the function by checking all other transmitters and update the receiver_output
        pass

class Transmitter(Sensor):
    def __init__(self, robot_object: Robot = None, sensor_id:int = None, relative_position_vector: np.ndarray = None, beam_angle:float = None) -> None:
        #initilize the parent class
        super().__init__(robot_object=robot_object, sensor_id=sensor_id, relative_position_vector=relative_position_vector)

        self.transmitter_state = 0 #  0: not transmitting any signal, 1: transmitting a signal
    
    def __str__(self):
        return f"Robot {self.ROBOT_OBJECT.get_id()} | Transmitter: {self.get_sensor_id()}, relative position: {self.get_relative_position_vector()}"
    
class UltrasonicSensor(Sensor):
    def __init__(self, robot_object: Robot = None, sensor_id:int = None, relative_position_vector: np.ndarray = None ) -> None:
        #initilize the parent class
        super().__init__(robot_object=robot_object, sensor_id=sensor_id, relative_position_vector=relative_position_vector)
        self.ultrasonic_sensor_output = None #  the detected distance in meters

    def __str__(self):
        return f"Robot {self.ROBOT_OBJECT.get_id()} | UltrasonicSensor: {self.get_sensor_id()}, relative position: {self.get_relative_position_vector()}"

    def measure_distance(self):
        #TODO: Implement the function by checking all other transmitters and update the receiver_output
        pass
#======================================================================================================================    

robot_initial_position = np.array([0.0, 0.0])
sensor_dict = {
    "receiver": {
        "number_of_receivers": 4,
        "receiver_placement_radius": 0.05,
        "receiver_placement_offset_angle":0,
        "view_angle": 45
    },
    "transmitter": {
        "number_of_transmitters": 4,
        "transmitter_placement_radius": 0.05,
        "transmitter_placement_offset_angle":0,
        "beam_angle": 45
    },
    "ultrasonic_sensor": {
        "placement_angles": [-45, 0, 45],
        "ultrasonic_sensor_placement_radius": 0.10
    }
}
robot_object = Robot( id=1, position = np.array([0.0, 0.0]), sensor_dict= sensor_dict)

