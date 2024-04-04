from modules.ui import UI
from objects.robot import Robot

UI_object = UI()

robot_objects = []
robot_1 = Robot(id = 1, x_pos = 0.5, y_pos =0.5)
robot_objects.append(robot_1)

#========================================

for i in range(25):
    robot_1.move_robot(del_x= 0.1, del_y = 0.1)
    
    rendered_frame = UI_object.render_frame(robot_objects=robot_objects)
    UI_object.show_frame(frame=rendered_frame, wait_duration_ms= 1000)




