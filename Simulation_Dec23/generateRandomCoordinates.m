function [bu, mu1, mu2, mu3, obstaclePositions] = generateRandomCoordinates()
    % Initialize an empty array to store random coordinates
    randomCoordinates = zeros(18, 2);
    
    % Generate random coordinates for each position
    for i = 1:18
        % Generate a random coordinate until it is different from all other positions
        while true
            % Generate random coordinates for the current position
            randCoord = randi([1, 9], 1, 2);
            
            % Check if the generated coordinates are different from all other positions
            if ~any(ismember(randomCoordinates(1:i-1, :), randCoord, 'rows'))
                % If different, store the coordinates and break the loop
                randomCoordinates(i, :) = randCoord;
                break;
            end
        end
    end
    
    % Extract coordinates for each specified position
    obstaclePositions = randomCoordinates(1:3, :);
    bu = randomCoordinates(4, :);
    mu1 = randomCoordinates(5, :);
    mu2 = randomCoordinates(6, :);
    mu3 = randomCoordinates(7, :);
end