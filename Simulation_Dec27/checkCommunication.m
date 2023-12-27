function communicationMatrix = checkCommunication(obstaclePositions, unitPositions, maxRange)
    numUnits = size(unitPositions, 1);
    communicationMatrix = zeros(numUnits, numUnits);

    for i = 1:numUnits
        for j = 1:numUnits
            if i ~= j
                % Calculate distance between units i and j
                distance = sqrt((unitPositions(i, 1) - unitPositions(j, 1))^2 + ...
                                (unitPositions(i, 2) - unitPositions(j, 2))^2);

                if distance <= maxRange
                    obstacleInPath = false;
                    % Check if obstacles obstruct the line of sight between units i and j
                    for k = 1:size(obstaclePositions, 1)
                        if isObstacleInPath(unitPositions(i, :), unitPositions(j, :), obstaclePositions(k, :))
                            obstacleInPath = true;
                            break;
                        end
                    end
                    communicationMatrix(i, j) = ~obstacleInPath;
                end
            end
        end
    end
end
