% Set the grid size
gridSize = 10;
bu=[2,5];
mu2=[9,9];
mu3=[5,8];
% Generate a random mu info 10x10 grid matrix with values between 0 and 5
mu1_info=zeros(10,10);
mu1_info(4,5)=1;
mu1_info(5,8)=2;
mu1_info(2,7)=3;

targetLocation = [4, 4];



[mu1_all,mu1_info]=searchAlgorithm1(1,mu1_info,targetLocation);
disp(mu1_all)
disp(mu1_info)
robotSimulation(mu1_all,mu2,mu3,bu,targetLocation,100)
% Define possible moves (up, down, right, left)
% moves = [0, 1; 0, -1; 1, 0; -1, 0];
% nextPosition = currentPosition + moves(1, :);
% display(nextPosition)




