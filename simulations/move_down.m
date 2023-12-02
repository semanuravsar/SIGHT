function [current_row] = move_down(current_row,n)
if (current_row+1<n+1)

   current_row=current_row+1;

else 
    current_row=current_row;
end
end