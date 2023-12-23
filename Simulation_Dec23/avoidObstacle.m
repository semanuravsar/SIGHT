function modified_trajectory = avoidObstacle(unit_position, trajectory, obstacle_position, n)
    % Initialize modified trajectory as the original trajectory
    modified_trajectory = trajectory;

    % Check if obstacle is on the trajectory
    for i = 1:size(trajectory, 1)
        % Calculate the next position according to the trajectory
        next_position = unit_position + trajectory(i, :);
        
        % Check if the next position matches the obstacle position
        if isequal(next_position, obstacle_position)
            % If obstacle is on the trajectory, modify the trajectory to avoid it
            % Here, you can choose to change the movement or stop the trajectory
            % For example, let's change the movement to go around the obstacle
            
            % Calculate the direction to move away from the obstacle
            direction = obstacle_position - unit_position;
            
            % Adjust the movement to avoid the obstacle by moving in the opposite direction
            modified_trajectory(i, :) = -direction;
            
            % Check if the modified trajectory goes out of bounds
            if any(abs(modified_trajectory(i, :)) > n)
                % If the modified trajectory goes out of bounds, stop movement
                modified_trajectory(i, :) = [0, 0];
            end
            
            % Break the loop as we've handled the obstacle
            break;
        end
    end
end
