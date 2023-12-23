function [M1_K, M2_K, M3_K, BU_K, unitLocations, targetLocations, obstacleLocations] = Playground(gridSize, targetNumber, obstacleNumber, MU_number, BU_number)

while 1
    
unitLocations = randi(gridSize, BU_number+ MU_number, 2);
targetLocations = randi(gridSize,targetNumber,2);
obstacleLocations_start = randi(gridSize + 1,obstacleNumber, 2);

obstacleDirections = zeros(obstacleNumber,2);
for k = 1: obstacleNumber
    direction = randi(5);
    for i = 1:5
        if direction == 1
            obstacleDirection = [0 1];
        elseif direction == 2
            obstacleDirection = [1 0];
        elseif direction == 3
            obstacleDirection = [0 -1];
        elseif direction == 4
            obstacleDirection = [-1 0];
        else
            obstacleDirection = [0 0];
        end
    end
    obstacleDirections(k,:) = obstacleDirection;
end

obstacleLocations = [obstacleLocations_start, obstacleLocations_start + randi(2,obstacleNumber,1).*obstacleDirections];


% check if any of the random locations coincide
Locations = [unitLocations; targetLocations];
overlap = 0;
for i = 1 : size(Locations,1)
    for j = i+1 : size(Locations,1)
        if isequal(Locations(i,:), Locations(j,:))
            overlap = 1;
            break
        end
    end
    if overlap == 1
        break
    end
end

if overlap == 0
 break
end

end

% initialize knowledge matrices
BU_K = zeros(gridSize);
M1_K = zeros(gridSize);
M2_K = zeros(gridSize);
M3_K = zeros(gridSize);

BU_K(unitLocations(4,1),unitLocations(4,2)) = 4;
M1_K(unitLocations(4,1),unitLocations(4,2)) = 4;
M2_K(unitLocations(4,1),unitLocations(4,2)) = 4;
M3_K(unitLocations(4,1),unitLocations(4,2)) = 4;


M1_K(unitLocations(1,1),unitLocations(1,2)) = 1;
M2_K(unitLocations(2,1),unitLocations(2,2)) = 2;
M3_K(unitLocations(3,1),unitLocations(3,2)) = 3;


obstacleLocations = reshape(obstacleLocations, [2*obstacleNumber,2]);

for i = 1: size(obstacleLocations,1)
    obstacleLocation = obstacleLocations(i,:);
    BU_K(obstacleLocation(1),obstacleLocation(2)) = 8;
    M1_K(obstacleLocation(1),obstacleLocation(2)) = 8;
    M2_K(obstacleLocation(1),obstacleLocation(2)) = 8;
    M3_K(obstacleLocation(1),obstacleLocation(2)) = 8;
end

end

