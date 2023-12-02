function [current_row] = move_up(current_row)

if(current_row-1>0)

   current_row=current_row-1;

else
    current_row=current_row;
end
end