function obstacleInPath = isObstacleInPath(positionA, positionB, obstaclePosition)
    % Function to check if an obstacle lies between two unit positions
    % positionA and positionB are positions of units
    % obstaclePosition is the position of the obstacle

    % Calculate the slope and intercept of the line between the units
    slope = (positionB(2) - positionA(2)) / (positionB(1) - positionA(1));
    intercept = positionA(2) - slope * positionA(1);

    % Check if the obstacle is between the units based on their positions
    if (obstaclePosition(2) - slope * obstaclePosition(1)) == intercept
        % Obstacle lies on the line between the units
        if (obstaclePosition(1) >= min(positionA(1), positionB(1))) && ...
           (obstaclePosition(1) <= max(positionA(1), positionB(1))) && ...
           (obstaclePosition(2) >= min(positionA(2), positionB(2))) && ...
           (obstaclePosition(2) <= max(positionA(2), positionB(2)))
            obstacleInPath = true;
            return;
        end
    end
    obstacleInPath = false;
end
