% this function will take a location matrix and move to empty locations where the value of empty locations are 0
%  The values in the info matrix represent as follows;
%  mu1 is here ==1 
%  mu2 is here ==2
%  mu3 is here ==3
%  mu4 is here ==4
%  This tile is scanned before is ==5
%  This tile has not been scanned ==0
%  The target is there ==6

function [mu1_all,mu2_all,mu3_all,mu1_info,mu2_info,mu3_info]=searchAlgorithm2_with_communication_updated(bu_info,mu1_info,mu2_info,mu3_info,targetPosition,numberofiter,obstacle)

    % mu: is a number between [1,3] represents the mu number
    % mu_info:10x10 matrix representing the information known by mu
   
    % Obtain the row and column of mu
    [currentRow1,currentCol1] =find(mu1_info==1);
    currentPosition1=[currentRow1,currentCol1];
    
    [currentRow2,currentCol2] =find(mu2_info==2);
    currentPosition2=[currentRow2,currentCol2];

    [currentRow3,currentCol3] =find(mu3_info==3);
    currentPosition3=[currentRow3,currentCol3];
    
    [buRow, buCol] = find(bu_info == 4);
    buPosition=[buRow,buCol];
   
    targetRow=targetPosition(1);
    targetCol=targetPosition(2);

    numberofiterations=numberofiter;
    
    %Create empty arrays for recording the mu's locations for all time
    mu1_all=cell(1,numberofiterations);
    mu2_all=cell(1,numberofiterations);
    mu3_all=cell(1,numberofiterations);
    %set initial locations for the 1st element
    mu1_all{1}=[currentRow1,currentCol1];
    mu2_all{1}=[currentRow2,currentCol2];
    mu3_all{1}=[currentRow3,currentCol3];   

    % Define possible moves (up, down, right, left)
    moves = [0, 1; 0, -1; 1, 0; -1, 0];
    
    % initialize  the trajectory matrices
    trajectory_mu1 = [];
    trajectory_mu2 = [];
    trajectory_mu3 = [];
    aimedPosition1 = [];
    aimedPosition2 = [];
    aimedPosition3 = [];
    
    gridSize = 10;
    maxRange = gridSize/2.5;
    
    
   for i=2:numberofiterations
        %Collect the data of mu past location
         obstaclePositions = obstacle;
         unitPositions = [currentPosition1; currentPosition2; currentPosition3; buPosition];
         
         linkMatrix = checkCommunication(obstaclePositions, unitPositions, maxRange);
         [mu1_info, mu2_info, mu3_info, bu_info] = knowledgeLinkUpdater(linkMatrix,mu1_info,mu2_info,mu3_info,bu_info);
        
        [~, aimedPosition1] = moveee(trajectory_mu1, currentPosition1, mu1_info,aimedPosition1);
        [~, aimedPosition2] = moveee(trajectory_mu2, currentPosition2, mu2_info,aimedPosition2);
        [~, aimedPosition3] = moveee(trajectory_mu3, currentPosition3, mu3_info,aimedPosition3);

                
%         aimedPosition1 = currentPosition1 + sum(trajectory_mu1(2:end,:))
%         aimedPosition2 = currentPosition2 + sum(trajectory_mu2(2:end,:))
%         aimedPosition3 = currentPosition3 + sum(trajectory_mu3(2:end,:))
        
        if mu1_info(aimedPosition1(1), aimedPosition1(2)) ~= 0
            trajectory_mu1 = [];
        end
        
        if mu2_info(aimedPosition2(1), aimedPosition2(2)) ~= 0
            trajectory_mu2 = [];
        end
        
        if mu3_info(aimedPosition3(1), aimedPosition3(2)) ~= 0
            trajectory_mu3 = [];
        end
        
        [trajectory_mu1,~] = moveee(trajectory_mu1, currentPosition1, mu1_info, aimedPosition1);
        [trajectory_mu2,~] = moveee(trajectory_mu2, currentPosition2, mu2_info, aimedPosition2);
        [trajectory_mu3,~] = moveee(trajectory_mu3, currentPosition3, mu3_info, aimedPosition3);


        move_mu1 = trajectory_mu1(1,:);
        move_mu2 = trajectory_mu2(1,:);
        move_mu3 = trajectory_mu3(1,:);

        nextPosition1 = currentPosition1 + move_mu1;
        nextPosition2 = currentPosition2 + move_mu2;
        nextPosition3 = currentPosition3 + move_mu3;
        
        %Collect the data of mu past location
        mu1_all{i}=[currentPosition1(1), currentPosition1(2)];
        mu2_all{i}=[currentPosition2(1), currentPosition2(2)];
        mu3_all{i}=[currentPosition3(1), currentPosition3(2)];

%         a1 = ([currentPosition1(1), currentPosition1(2)] == [targetRow,targetCol]);
%         a2 = ([currentPosition2(1), currentPosition2(2)] == [targetRow,targetCol]);
%         a3 = ([currentPosition3(1), currentPosition3(2)] == [targetRow,targetCol]);

        if([currentPosition1(1), currentPosition1(2)] == [targetRow,targetCol])
            nextPosition1(1) = targetRow;
            nextPosition1(2) = targetCol;
            mu1_info(currentPosition1(1), currentPosition1(2))=6;
        else
           

        
        %Change the value of currentPosition as scanned
        mu1_info(currentPosition1(1), currentPosition1(2))=5;
        % Move to the empty location
        currentPosition1 = nextPosition1;
        %give the information of the location of where the mu is.
        mu1_info(nextPosition1(1), nextPosition1(2))=1;
            

        %Check if the next position is target
            
        end

        if([currentPosition2(1), currentPosition2(2)] == [targetRow,targetCol])
            nextPosition2(1) = targetRow;
            nextPosition2(2) = targetCol;
            mu2_info(currentPosition2(1), currentPosition2(2))=6;
        else
           

        
        %Change the value of currentPosition as scanned
        mu2_info(currentPosition2(1), currentPosition2(2))=5;
        % Move to the empty location
        currentPosition2 = nextPosition2;
        %give the information of the location of where the mu is.
        mu2_info(nextPosition2(1), nextPosition2(2))=2;
            
            
        end


        if([currentPosition3(1), currentPosition3(2)] == [targetRow,targetCol])
            nextPosition3(1) = targetRow;
            nextPosition3(2) = targetCol;
            mu3_info(currentPosition3(1), currentPosition3(2))=6;
        else 

        
        %Change the value of currentPosition as scanned
        mu3_info(currentPosition3(1), currentPosition3(2))=5;
        % Move to the empty location
        currentPosition3 = nextPosition3;
        %give the information of the location of where the mu is.
        mu3_info(nextPosition3(1), nextPosition3(2))=3;
            
        end
                       
    end

   
end