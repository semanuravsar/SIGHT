clear all; clc;

%network parameters
fieldLength = 5; % in km
mobileUnitNumber = 3;
baseUnitNumber = 1;
obstacleNumber = 3;
linkRange = 2; % in km

[mobileUnitLocations_inKm, baseUnitLocations_inKm, distanceMatrix_inKm, dLinkMatrix, connected, maxHop, avgHop, avgNumberNeighbors,minNumberNeighbors] = networkFunct(fieldLength, linkRange, mobileUnitNumber, baseUnitNumber);

% plot node locations
figure(1)
scatter(mobileUnitLocations_inKm(:,1),mobileUnitLocations_inKm(:,2));
hold on
scatter(baseUnitLocations_inKm(:,1),baseUnitLocations_inKm(:,2));
xlim([0.0 fieldLength]);
ylim([0.0 fieldLength]);
title('location of nodes');
xlabel('horizontal length (km)');
ylabel('vertical length (km)');
grid minor;

% plot the nodes along with the links
figure(2)
nodeLocations_inKm = [mobileUnitLocations_inKm ; baseUnitLocations_inKm];
gplot(dLinkMatrix,nodeLocations_inKm,'-*');
xlim([0.0 fieldLength]);
ylim([0.0 fieldLength]);
title('location of nodes and communication links between them');
xlabel('horizontal length (km)');
ylabel('vertical length (km)');
grid minor;

% plot the nodes along with the links
figure(3)
scatter(mobileUnitLocations_inKm(:,1),mobileUnitLocations_inKm(:,2),'*');
hold on
scatter(baseUnitLocations_inKm(:,1),baseUnitLocations_inKm(:,2),'*');
nodeLocations_inKm = [mobileUnitLocations_inKm ; baseUnitLocations_inKm];
for i = 1 : length(dLinkMatrix)
    for j = 1 : length(dLinkMatrix)
        if dLinkMatrix(i,j) == 1
            plot([nodeLocations_inKm(i,1),nodeLocations_inKm(j,1)],[nodeLocations_inKm(i,2),nodeLocations_inKm(j,2)],'b');
        end
    end
end
xlim([0.0 fieldLength]);
ylim([0.0 fieldLength]);
title('location of nodes and communication links between them');
xlabel('horizontal length (km)');
ylabel('vertical length (km)');
grid minor;


% plot the network as a graph
figure(4)
dLinkMatrixG = dLinkMatrix - diag(diag(dLinkMatrix));
networkGraph = graph(dLinkMatrixG);
plot(networkGraph);
title('network as a graph');

%see some numbers on screen
linkRange
maxHop
avgHop
avgNumberNeighbors
minNumberNeighbors