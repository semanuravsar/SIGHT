function [position_row,position_col,count] = search (location,n)

%{ 
this function will take a location matrix and move to empty locations
where the value of empty locations are 0
 the code below is written for mu1
 The values in the matrix represent as follows;
 mu1 is here ==1 
 mu2 is here ==2
 mu3 is here ==3
 mu4 is here ==4
 This tile is scanned before is ==5
 This tile has not been scanned ==0
 The target is there ==6
%} 

% We plot our every move to keep track. When the target is found it is
% painted with *.

% We use the count to keep track of how many moves we have done
%row_move and col_move keeps track of our last movement in case we need to
%go back
count=0;
row_move=0;
col_move=0;

% We obtained the row and column of mu1
[current_row,current_column] =find(location==1);
figure;
plot(current_column,n+1-current_row,'.')
hold on;

while 1

    % We move to the next tile if it is not scanned before or the target is
    % there

    % to move up
    if ((current_row-1>0) &&(location(current_row-1,current_column)==0||location(current_row-1,current_column)==6))
        %if the target is found, stop moving
        if location(current_row-1,current_column)==6
            current_row=current_row-1;
            position_row=current_row
            position_col=current_column
            count=count+1
             plot(current_column,n+1-current_row,'*');
             break;
         % otherwise keep moving    
        else 
       location(current_row,current_column)=5;
       current_row=current_row-1;
       location(current_row,current_column)=1;
       row_move=-1;
       col_move=0;
        end

     % to go right
    elseif ((current_column+1<n+1) && (location(current_row,current_column+1)==0||location(current_row,current_column+1)==6))
        %if the target is found, stop moving
        if location(current_row,current_column+1)==6
            current_column=current_column+1;
            position_row=current_row
            position_col=current_column
            count=count+1
              plot(current_column,n+1-current_row,'*');
            break;
        else
       location(current_row,current_column)=5;
       current_column=current_column+1;
       location(current_row,current_column)=1;
       row_move=0;
       col_move=1;
        end

  
    % to go left
    elseif ((current_column-1>0) && (location(current_row,current_column-1)==0||location(current_row,current_column-1)==6))
        %if the target is found, stop moving
        if location(current_row,current_column-1)==6
           current_column=current_column-1;
           position_row=current_row
           position_col=current_column
           count=count+1
            plot(current_column,n+1-current_row,'*');
           break;
       else
       location(current_row,current_column)=5;
       current_column=current_column-1;
       location(current_row,current_column)=1;
       row_move=0;
       col_move=-1;
       end
    
   
    % to move down
    elseif ((current_row+1<n+1) && (location(current_row+1,current_column)==0||location(current_row+1,current_column)==6))
         %if the target is found, stop moving
        if location(current_row+1,current_column)==6
            current_row=current_row+1;
            position_row=current_row
            position_col=current_column
            count=count+1
            plot(current_column,n+1-current_row,'*');

            break;
        else 
        location(current_row,current_column)=5;
       current_row=current_row+1;
       location(current_row,current_column)=1;
       row_move=1;
       col_move=0;
        end

        % This part is for when we do not have any empty tiles to move, so
        % we go back to our last tile
    else    
        if(row_move==1 && current_row>1)
        
        current_row=current_row-1;
        elseif(row_move==-1 && current_row<6)
        current_row=current_row-1;
        elseif(col_move==1 && current_column>1)
        current_column=current_column-1;clc
        elseif(col_move==-1 && current_column<6)
        current_column=current_column+1;
        end
    
    end

    position_row=current_row;
    position_col=current_column;
    count=count+1;
    plot(current_column,n+1-current_row,'.');
    hold on;
    
    
end
end


 
 

    