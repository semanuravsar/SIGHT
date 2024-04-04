import cv2
import numpy as np
class Robot:
    
    def __init__(self, id = None, x_pos=None, y_pos=None):
        
        if x_pos is None or y_pos is None:
            raise Exception("x and y position of the robot must be given")
        
        if id is None:
            raise Exception("ID is not given")
        
        self.ROBOT_WIDTH_m = 0.2 
        self.ROBOT_HEIGHT_m = 0.2
        self.ID = id
        self.x_pos = x_pos
        self.y_pos = y_pos    
    
    def move_robot(self,del_x = 0, del_y = 0):
        self.x_pos = self.x_pos + del_x
        self.y_pos = self.y_pos + del_y
        
    def get_position(self):
        return [self.x_pos, self.y_pos]
        
    def rotate_robot(self, rotation_angle = 0):
        #TODO:
        pass
    
    def draw_robot(self, frame, mapper_function = None):
        # Define the vertices of the polygon (example)
        color = (0, 255, 0)  # Green color
        thickness = 2

        top_left_x = self.x_pos - self.ROBOT_WIDTH_m/2
        top_left_y = self.y_pos - self.ROBOT_HEIGHT_m/2        
        top_left_px, top_left_py = mapper_function(top_left_x,top_left_y)
        top_left = (top_left_px, top_left_py)
        
        print(top_left_x, top_left_px)
        
        bottom_right_x = self.x_pos + self.ROBOT_WIDTH_m/2
        bottom_right_y = self.x_pos + self.ROBOT_WIDTH_m/2
        bottom_right_px, bottom_right_py = mapper_function(bottom_right_x, bottom_right_y)
        bottom_right = (bottom_right_px, bottom_right_py)       
        
        print(top_left, bottom_right)
        cv2.rectangle(frame, top_left, bottom_right, color, thickness=thickness)
        