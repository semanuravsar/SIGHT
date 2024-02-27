import random
import cv2
import numpy as np

NUMBER_OF_ROBOTS = 3
NUMBER_OF_OBSTACLES = random.choice([0,1,2])
POSSIBLE_DESIRED_DIRECTIONS = [ (0,1), (1,0), (0,-1), (-1,0)] #the robot wants to move in one of these directions
POSSIBLE_FACING_DIRECTIONS = [ (0,1), (1,0), (0,-1), (-1,0)] #initially, the robot can face in any direction

class Robot:
    def __init__(self, ID:int, h_cell:int, v_cell:int, facing_direction:list[int], desired_direction:list[int]):
        self.ID = ID
        self.DESIRED_DIRECTION = desired_direction #the direction the robot wants to move
        self.h_cell = h_cell
        self.v_cell = v_cell
        self.facing_direction = facing_direction #the direction the robot is facing
    
    def is_here(self, other_h_cell:int, other_v_cell:int):
        return self.v_cell == other_v_cell and self.h_cell == other_h_cell
    
    def __str__(self):
        return f'Robot at ({self.h_cell},{self.v_cell})'
        
class Obstacle:
    def __init__(self, h_cell:int, v_cell:int):
        self.h_cell= h_cell
        self.v_cell = v_cell
        
    def is_here(self,  other_h_cell:int, other_v_cell:int):
        return self.v_cell == other_v_cell and self.h_cell == other_h_cell
    
    def __str__(self):
        return f'Obstacle at ({self.h_cell},{self.v_cell})'

def draw_grid(grid, facing_directions, desired_directions):
    cell_size = 50
    img_size = cell_size * len(grid) + 1
    img = np.full((img_size, img_size, 3), 200, dtype=np.uint8)  # Light gray background

    # Colors
    obstacle_color = (0, 0, 0)  # Black
    robot_color = (255, 255, 255)  # White
    facing_color = (0, 0, 255)  # Red for facing direction
    desired_color = (255, 0, 0)  # Blue for desired direction
    grid_color = (50, 50, 50)  # Dark gray
    outside_color = (150, 250, 150)  # Gray for outside the grid

    for i in range(len(grid)):
        for j in range(len(grid[i])):
            top_left = (j * cell_size + j, i * cell_size + i)
            bottom_right = (top_left[0] + cell_size, top_left[1] + cell_size)
            cell_center = (int((top_left[0] + bottom_right[0]) / 2), int((top_left[1] + bottom_right[1]) / 2))
            
            if i == 0 or j == 0 or i == len(grid) - 1 or j == len(grid[i]) - 1:
                cv2.rectangle(img, top_left, bottom_right, outside_color, -1)
            elif grid[i][j] == -1:  # Obstacle
                cv2.rectangle(img, top_left, bottom_right, obstacle_color, -1)
            elif grid[i][j] == 1:  # Robot
                cv2.rectangle(img, top_left, bottom_right, robot_color, -1)
                # Draw facing direction vector
                facing_dir = facing_directions[i][j]
                if facing_dir is not None:
                    facing_end = (cell_center[0] + facing_dir[0] * 15, cell_center[1] + facing_dir[1] * 15)
                    cv2.arrowedLine(img, cell_center, facing_end, facing_color, 2, tipLength=0.5)
                # Draw desired direction vector
                desired_dir = desired_directions[i][j]
                if desired_dir is not None:
                    desired_end = (cell_center[0] + desired_dir[0] * 20, cell_center[1] + desired_dir[1] * 20)
                    cv2.arrowedLine(img, cell_center, desired_end, desired_color, 2, tipLength=0.5)

            cv2.rectangle(img, top_left, bottom_right, grid_color, 1)

    cv2.imshow('Grid', img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

def main():
    NUMBER_OF_OBSTACLES = random.choice([0,1,2])
    #create the robots
    robots:list[Robot] = []
    for i in range(NUMBER_OF_ROBOTS):
        facing_direction = random.choice(POSSIBLE_FACING_DIRECTIONS)
        desired_direction = random.choice(POSSIBLE_DESIRED_DIRECTIONS)  
        while True:
            h_cell = random.choice([-1,0,1])
            v_cell = random.choice([-1,0,1])
            is_cell_empty = True
            for robot in robots:
                if robot.is_here(h_cell, v_cell):
                    is_cell_empty = False
                    break
            if is_cell_empty:
                robots.append(Robot(i, h_cell=h_cell, v_cell=v_cell, desired_direction=desired_direction, facing_direction=facing_direction))
                print(f'Robot {i} created at ({h_cell},{v_cell})')
                break

    #create the obstacles
    obstacles:list[Obstacle] = []
    for i in range(NUMBER_OF_OBSTACLES):
        while True:
            h_cell = random.choice([-1,0,1])
            v_cell = random.choice([-1,0,1])
            is_cell_empty = True
            for robot in robots:
                if robot.is_here(h_cell, v_cell):
                    is_cell_empty = False
                    break
            
            for obstacle in obstacles:
                if obstacle.is_here(h_cell, v_cell):
                    is_cell_empty = False
                    break

            if is_cell_empty:
                obstacles.append(Obstacle(h_cell, v_cell))
                print(f'Obstacle {i} created at ({h_cell},{v_cell})')
                break       

    center_offset = 2
    grid_size = 5
    grid = [[0 for i in range(grid_size)] for j in range(grid_size)]
    facing_directions = [[None for i in range(grid_size)] for j in range(grid_size)]
    desired_directions = [[None for i in range(grid_size)] for j in range(grid_size)]

    for robot in robots:
        grid[robot.v_cell+center_offset][robot.h_cell+center_offset] = 1
        #facing_directions[robot.v_cell+1][robot.h_cell+1] = robot.facing_direction
        desired_directions[robot.v_cell+center_offset][robot.h_cell+center_offset] = robot.DESIRED_DIRECTION
    for obstacle in obstacles:
        grid[obstacle.v_cell+center_offset][obstacle.h_cell+center_offset] = -1

    draw_grid(grid, facing_directions, desired_directions)

if __name__ == '__main__':
    while True:
        main()