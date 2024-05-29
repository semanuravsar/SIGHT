import numpy as np
import time

class Unit():

    def __init__(self, id:int=0, icon_image:np.ndarray=None, x:int=1, y:int=1, characteristic_color = (255,0,0)):

        self.ICON_IMAGE = icon_image
        self.ID = id # ID of the mobile unit
        self.CHARACTERISTIC_COLOR = characteristic_color # color of the mobile unit

        self.x = x # x coordinate of the mobile unit
        self.y = y # y coordinate of the mobile unit
        self.x_target = None # x coordinate of the target of the mobile unit
        self.y_target = None # y coordinate of the target of the mobile unit
        self.state = "NA" # state of the mobile unit

        self.visited_coordinates = [] # list of coordinates that the mobile unit has visited

        self.number_of_packages = 0 # number of packages that the mobile unit has sent and received
        self.last_time_package_received = None # time when the last package wasa sent by the mobile unit and received
    

    def is_target_found(self):
        if self.x_target is None or self.y_target is None:
            return False
        
        if self.x_target == 10 and self.y_target == 10:
            return False
        
        return True
    
    def get_characteristic_color(self):
        return self.CHARACTERISTIC_COLOR
    
    def should_draw(self):
        if self.x is None or self.y is None:
            return False
        if self.x == 15 and self.y == 15:
            return False
        return True
    
    def get_visited_coordinates(self):
        return self.visited_coordinates
    
    def get_icon(self):
        return self.ICON_IMAGE
    
    def get_coordinates(self):
        return (self.x, self.y)
    
    def get_target_coordinates(self):
        return (self.x_target, self.y_target)
    
    def update_with_info(self, info_dict:dict):
        id = info_dict['received_ID']
        if id != self.ID:
            return
        
        self.number_of_packages += 1
        
        self.x = info_dict['x']
        self.y = info_dict['y']

        visited_coord = (self.x, self.y)
        if visited_coord not in self.visited_coordinates:
            self.visited_coordinates.append(visited_coord)
        
        self.x_target = info_dict['target_x']
        self.y_target = info_dict['target_y']
        self.state = info_dict['state']

        self.last_time_package_received = time.time()
        pass

    def get_id(self):
        return self.ID
    
    def return_elapsed_time_since_last_package(self):
        if self.last_time_package_received is None:
            return "NA"
        
        elapsed_time_s = time.time() - self.last_time_package_received

        if elapsed_time_s < 60:
            return f"{(elapsed_time_s):.2f} s"
        
        elapsed_time_m = elapsed_time_s / 60
        if elapsed_time_m < 60:
            return f"{int(elapsed_time_m)} m"
        
        elapsed_time_h = elapsed_time_m / 60
        return f"{int(elapsed_time_h)} h"
        
    def get_number_of_packages(self):
        return self.number_of_packages
    
    def get_state(self):
        return self.state
    
        
class Obstacle():

    def __init__(self, x:int, y:int, icon_image:np.ndarray):
        self.ICON_IMAGE = icon_image
        self.x = x # x coordinate of the obstacle
        self.y = y # y coordinate of the obstacle

    def get_icon(self):
        return self.ICON_IMAGE
    
    def get_coordinates(self):
        return (self.x, self.y)
    

class Target():

    def __init__(self, x:int, y:int, found_target_icon:np.ndarray, not_found_target_icon:np.ndarray):
        self.x = x # x coordinate of the target
        self.y = y
        self.found = False
        self.found_target_icon = found_target_icon
        self.not_found_target_icon = not_found_target_icon
            
    def set_found_status(self, found:bool):
        self.found = found

    def get_coordinates(self):
        return (self.x, self.y)
    
    def get_icon(self):
        if self.found:
            return self.found_target_icon
        else:
            return self.not_found_target_icon







