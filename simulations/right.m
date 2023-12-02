function [current_column] = right(current_column,n)
if (current_column+1<n+1)

   current_column=current_column+1;
else 
    current_column=current_column;
end
end