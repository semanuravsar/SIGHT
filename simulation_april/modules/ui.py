import cv2
import copy

class UI:    
    def __init__(self):
        self.BACKGROUND_IMAGE = cv2.imread('images/src/backgrounds/background_1.png')

    def get_background_template(self):
        return copy.deepcopy(self.BACKGROUND_IMAGE)
    
    def map_real_coord_to_frame(self,x,y):
        px = int((490/2.5)*x)
        py = int((490/2.5)*y)
        return [px,py]

    def render_frame(self, robot_objects = []):        
        frame = self.get_background_template()
        height, width, _ = frame.shape
        
        first_square_x = height/18
        first_square_y = width/18
        square_size = width/9
    
        for robot_object in robot_objects:
            robot_object.draw_robot(frame, mapper_function = self.map_real_coord_to_frame)
        
                
        return frame
    
    def show_frame(self, frame, wait_duration_ms = 0):        
        cv2.imshow("SIGHT-UI", frame)
        cv2.waitKey(wait_duration_ms)
        
        
        