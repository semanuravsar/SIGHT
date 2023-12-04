% Set the grid size
gridSize = 10;
bu=[2,5];

mu1_info=zeros(10,10);
mu1_info(4,5)=1;
mu1_info(5,8)=2;
mu1_info(2,7)=3;

targetLocation = [4, 4];

mu2_info=zeros(10,10);
mu2_info(4,5)=1;
mu2_info(5,8)=2;
mu2_info(2,7)=3;

mu3_info=zeros(10,10);
mu3_info(4,5)=1;
mu3_info(5,8)=2;
mu3_info(2,7)=3;



[mu1_all,mu1_info]=searchAlgorithm1(1,mu1_info,targetLocation);
[mu2_all,mu2_info]=searchAlgorithm1(2,mu2_info,targetLocation);
[mu3_all,mu3_info]=searchAlgorithm1(3,mu3_info,targetLocation);

disp(mu2_info)
disp(mu3_info)
robotSimulation(mu1_all,mu2_all,mu3_all,bu,targetLocation,55)










