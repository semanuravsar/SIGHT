function [current_column] = left(current_column,n)
if(current_column-1>0)
    
   current_column=current_column-1;
else 
   current_column=current_column; 
end
end