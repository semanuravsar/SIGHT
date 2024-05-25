import cv2
import copy

from objects import Unit
from objects import Obstacle
from objects import Target

from serial_class import SerialPortReader
from serial_class import DataParser
import datetime

class Picasso():

    def __init__(self, window_name = "Sight - observatory"):
        self.WINDOW_NAME = window_name        
        cv2.namedWindow(self.WINDOW_NAME, cv2.WND_PROP_FULLSCREEN)
        cv2.setWindowProperty(self.WINDOW_NAME, cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)  

        self.ICON_PATHS = {
            "base_unit": "src/icons/base_unit_png.png",
            "mu_dark_blue": "src/icons/mu_dark_blue_png.png",
            "mu_light_blue": "src/icons/mu_light_blue_png.png",
            "mu_purple": "src/icons/mu_purple_png.png",
            "not_found_target": "src/icons/not_found_target_png.png",
            "found_target": "src/icons/found_target_png.png",
            "obstacle": "src/icons/obstacle_png.png",
            "UI_background": "src/icons/UI_background_png.png",          
        }

        self.IMAGES = {
            "UI_background": cv2.imread(self.ICON_PATHS["UI_background"], cv2.IMREAD_UNCHANGED),
            "base_unit": cv2.imread(self.ICON_PATHS["base_unit"], cv2.IMREAD_UNCHANGED),
            "mu_dark_blue": cv2.imread(self.ICON_PATHS["mu_dark_blue"], cv2.IMREAD_UNCHANGED),
            "mu_light_blue": cv2.imread(self.ICON_PATHS["mu_light_blue"], cv2.IMREAD_UNCHANGED),
            "mu_purple": cv2.imread(self.ICON_PATHS["mu_purple"], cv2.IMREAD_UNCHANGED),
            "not_found_target": cv2.imread(self.ICON_PATHS["not_found_target"], cv2.IMREAD_UNCHANGED),
            "found_target": cv2.imread(self.ICON_PATHS["found_target"], cv2.IMREAD_UNCHANGED),
            "obstacle": cv2.imread(self.ICON_PATHS["obstacle"], cv2.IMREAD_UNCHANGED),
        }
        
        self.last_header_text = ""
        self.last_header_text_change_time = datetime.datetime.now()
        self.frame = self.get_image("UI_background")

    def get_image(self, image_name:str):
        return copy.deepcopy(self.IMAGES[image_name])
    
    def map_grid_cord_to_pixel(self, x_grid:int, y_grid:int) -> tuple[int,int]:
        origin_coordinates = (1,1)
        origin_px = (757,655)
        UI_WIDTH = 1920   
 
        y_base_vector = (61,-36) 
        x_base_vector = (60,35)
        
        x_dif = x_grid - origin_coordinates[0]
        y_dif = y_grid - origin_coordinates[1]

        ratio = self.IMAGES["UI_background"].shape[1] / UI_WIDTH
        
        x_px = int( ratio*(origin_px[0] + x_dif * x_base_vector[0] + y_dif * y_base_vector[0]))
        y_px = int(ratio*(origin_px[1] + x_dif * x_base_vector[1] + y_dif * y_base_vector[1]))

        return (x_px, y_px)

    def reset_frame(self):
        self.frame = self.get_image("UI_background")
    
    def show_frame(self):
        cv2.imshow(self.WINDOW_NAME, self.frame)
        key = cv2.waitKey(1)

    def release(self):
        cv2.destroyAllWindows()

    def set_header_text(self, text:str=None):
        self.last_header_text = text
        self.last_header_text_change_time = datetime.datetime.now()
       
    def draw_visited_coordinates_of_unit(self, unit:Unit=None):
        for coordinate in unit.get_visited_coordinates():
            px, py = self.map_grid_cord_to_pixel(coordinate[0], coordinate[1])
            cv2.circle(self.frame, (px, py), 5, (255, 0, 0), -1)

    def draw_header(self):
        bottom_left = (350,100)
        timestamp = self.last_header_text_change_time.strftime("%Y-%m-%d %H:%M:%S")
        cv2.putText(self.frame, f"{timestamp} - {self.last_header_text}", bottom_left, cv2.FONT_HERSHEY_SIMPLEX, 0.85, (0, 0, 0), 2, cv2.LINE_AA)

    def draw_unit(self, mu: Unit):

        if mu.should_draw():
            icon = mu.get_icon()
            mu_coordinates = mu.get_coordinates()
            px, py = self.map_grid_cord_to_pixel(mu_coordinates[0], mu_coordinates[1])

            # Calculate the top-left corner position to center the icon at (px, py)
            start_x = px - icon.shape[1] // 2
            start_y = (py - icon.shape[0] // 2) -20

            if icon.shape[2] == 4:  # Ensure the icon has an alpha channel
                alpha_s = icon[:, :, 3] / 255.0
                alpha_l = 1.0 - alpha_s

                for c in range(0, 3):
                    self.frame[start_y:start_y+icon.shape[0], start_x:start_x+icon.shape[1], c] = (
                        alpha_s * icon[:, :, c] + alpha_l * self.frame[start_y:start_y+icon.shape[0], start_x:start_x+icon.shape[1], c]
                    )
            else:
                self.frame[start_y:start_y+icon.shape[0], start_x:start_x+icon.shape[1]] = icon

        #draw textual information
        top_left_map = {
            1: (60, 255),
            2: (60, 445),
            3: (60, 650),
            4: (60, 844),
        }

        shifts = {
            "elapsed_time" : (240, 32),
            "number_of_packages": (240, 70),
            "state": (88, 80),
            "x":(50 , 118),
            "y":(140, 118),
            "x_p": (50, 158),
            "y_p": (140, 158),

        }

        robot_id = mu.get_id()

        # draw the elapsed time since the last package
        elapsed_time = mu.return_elapsed_time_since_last_package()
        cv2.putText(self.frame, f"{elapsed_time}", (top_left_map[robot_id][0]+shifts["elapsed_time"][0], top_left_map[robot_id][1] + shifts["elapsed_time"][1]), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 0), 1, cv2.LINE_AA)

        # draw the number of packages
        cv2.putText(self.frame, f"{mu.get_number_of_packages()}", (top_left_map[robot_id][0]+shifts["number_of_packages"][0], top_left_map[robot_id][1] + shifts["number_of_packages"][1]), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 0), 1, cv2.LINE_AA)

        # draw the state of the unit
        cv2.putText(self.frame, f"{mu.get_state()}", (top_left_map[robot_id][0]+shifts["state"][0], top_left_map[robot_id][1] + shifts["state"][1]), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 0), 1, cv2.LINE_AA)
    

        # draw the x and y coordinates of the unit
        # x
        cv2.putText(self.frame, f"{mu.get_coordinates()[0]}", (top_left_map[robot_id][0]+shifts["x"][0], top_left_map[robot_id][1] + shifts["x"][1]), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 0), 1, cv2.LINE_AA)
        # y
        cv2.putText(self.frame, f"{mu.get_coordinates()[1]}", (top_left_map[robot_id][0]+shifts["y"][0], top_left_map[robot_id][1] + shifts["y"][1]), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 0), 1, cv2.LINE_AA)

        # draw the x and y coordinates of the target
        # x
        cv2.putText(self.frame, f"{mu.get_target_coordinates()[0]}", (top_left_map[robot_id][0]+shifts["x_p"][0], top_left_map[robot_id][1] + shifts["x_p"][1]), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 0), 1, cv2.LINE_AA)
        # y
        cv2.putText(self.frame, f"{mu.get_target_coordinates()[1]}", (top_left_map[robot_id][0]+shifts["y_p"][0], top_left_map[robot_id][1] + shifts["y_p"][1]), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 0), 1, cv2.LINE_AA)

    def draw_target(self, target: Target):
        icon = target.get_icon()
        mu_coordinates = target.get_coordinates()
        px, py = self.map_grid_cord_to_pixel(mu_coordinates[0], mu_coordinates[1])

        # draw dashed line
        icon_center_px = (px, 240)
        cv2.line(self.frame, icon_center_px, (px, py), (0, 0, 0), 2, cv2.LINE_AA)

        # draw the target icon
        
        px, py = icon_center_px

        # Calculate the top-left corner position to center the icon at (px, py)
        start_x = px - icon.shape[1] // 2
        start_y = (py - icon.shape[0] // 2) 


        if icon.shape[2] == 4:  # Ensure the icon has an alpha channel
            alpha_s = icon[:, :, 3] / 255.0
            alpha_l = 1.0 - alpha_s

            for c in range(0, 3):
                self.frame[start_y:start_y+icon.shape[0], start_x:start_x+icon.shape[1], c] = (
                    alpha_s * icon[:, :, c] + alpha_l * self.frame[start_y:start_y+icon.shape[0], start_x:start_x+icon.shape[1], c]
                )
        else:
            self.frame[start_y:start_y+icon.shape[0], start_x:start_x+icon.shape[1]] = icon
        
    def draw_obstacle(self, obstacle: Obstacle):
        icon = obstacle.get_icon()
        obs_coordinates = obstacle.get_coordinates()
        px, py = self.map_grid_cord_to_pixel(obs_coordinates[0], obs_coordinates[1])

        # Calculate the top-left corner position to center the icon at (px, py)
        start_x = px - icon.shape[1] // 2
        start_y = (py - icon.shape[0] // 2) -20

        if icon.shape[2] == 4:  # Ensure the icon has an alpha channel
            alpha_s = icon[:, :, 3] / 255.0
            alpha_l = 1.0 - alpha_s

            for c in range(0, 3):
                self.frame[start_y:start_y+icon.shape[0], start_x:start_x+icon.shape[1], c] = (
                    alpha_s * icon[:, :, c] + alpha_l * self.frame[start_y:start_y+icon.shape[0], start_x:start_x+icon.shape[1], c]
                )
        else:
            self.frame[start_y:start_y+icon.shape[0], start_x:start_x+icon.shape[1]] = icon


if __name__ == "__main__":
    picasso = Picasso()
    
    # open the serial port
    serial_port_reader = SerialPortReader(port = None, baud_rate = 9600, timeout = 1)
    serial_port_reader.print_available_ports()
    choosen_port = input("Enter the port name to open: ")
    serial_port_reader.set_port(choosen_port)
    serial_port_reader.open()

    # define parser
    serial_data_parser = DataParser()
    
    # define the targets
    target = Target(x = 5, y = 8, found_target_icon = picasso.get_image("found_target"), not_found_target_icon = picasso.get_image("not_found_target"))
    target.set_found_status(False)

    # define the units
    base_unit_1 = Unit(id = 4, x=5, y=5, icon_image = picasso.get_image("base_unit"))
    mobile_unit_2 = Unit(id = 2,x=None, y=None, icon_image = picasso.get_image("mu_dark_blue"))
    mobile_unit_3 = Unit(id = 3,x=None, y=None, icon_image = picasso.get_image("mu_light_blue"))
    mobile_unit_4 = Unit(id = 1,x=None, y=None, icon_image = picasso.get_image("mu_purple"))

    # define the obstacles
    obstacle_1 = Obstacle(x = 3, y = 2, icon_image = picasso.get_image("obstacle"))
    obstacle_2 = Obstacle(x = 2, y = 5, icon_image = picasso.get_image("obstacle"))

    units = [base_unit_1, mobile_unit_2, mobile_unit_3, mobile_unit_4]
    obstacles = [obstacle_1, obstacle_2]

    # draw the UI
    while True:
        #TODO: check for the serial data: if the serial data is received, update the units' positions

        r = serial_port_reader.read_line()
        if r is not None and r != "":
            print("RECEIVED ->", r)
            picasso.set_header_text(text=f"RECEIVED ->{r}")
    
        parsed_data_dict = serial_data_parser.parse_data(received_data= r)
        if parsed_data_dict is not None:
            print("PARSED ->", parsed_data_dict)
            picasso.set_header_text(text=f"PARSED ->{parsed_data_dict}")
            for unit in units:
                unit.update_with_info(parsed_data_dict)


        # draw the units
        picasso.reset_frame()
            
        # draw header
        picasso.draw_header()

        # draw the visited coordinates of the units
        for unit in units:
            picasso.draw_visited_coordinates_of_unit(unit)

        # draw the target
        target.set_found_status(False)   
        for unit in units:
            if unit.is_target_found():
                target.set_found_status(True) 

    

        picasso.draw_target(target)

        for unit in units:
                try:
                    picasso.draw_unit(unit)       
                except:
                    print("Error while drawing the unit: ID ", unit.get_id())
                    continue

        picasso.draw_obstacle(obstacle_1)
        picasso.draw_obstacle(obstacle_2)

        picasso.show_frame()


    # release the window
    picasso.release()

    # close the serial port
    serial_port_reader.close()

