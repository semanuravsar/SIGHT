
current_row1 =2 ;
current_column1=2;
mu1 =[current_row1,current_column1];
current_row2=1;
current_column2=4;
current_row3=5;
current_column3=6;
target_row=5;
target_col=5;
num1 =randi([1 4],1,1);
num2 =randi([1 4],1,1);
num3 =randi([1 4],1,1);


for i=1:500


if num1==1
   
  current_row1=move_up(current_row1);

elseif num1==2

    current_row1=move_down(current_row1,10);

elseif num1==3
    current_column1= right(current_column1,10);
elseif num1==4
   current_column1= left(current_column1);
end

mu1=[current_row1,current_column1]

if num2==1
   
 current_row2= move_up(current_row2);

elseif num2==2

 current_row2=  move_down(current_row2,10);

elseif num2==3
  current_column2= right(current_column2,10);
elseif num2==4
    current_column2=left(current_column2);
end

mu2=[current_row2,current_column2]

if num3==1
   
  current_row3=move_up(current_row3);

elseif num3==2

    current_row3=move_down(current_row3,10);

elseif num3==3
       current_column3= right(current_column3,10);
elseif num3==4
    current_column3 =left(current_column3);
end

mu3=[current_row3,current_column3]





if (current_row1==target_row&& current_column1==target_col)

    num1=0;
    mu1=[current_row1,current_column1];

else 
    num1 =randi([1 4],1,1);

end 

if (current_row2==target_row&& current_column2==target_col)

   num2=0;
   mu2=[current_row2,current_column2];
else 
    num2 =randi([1 4],1,1);

end 
if (current_row3==target_row&& current_column3==target_col)
    mu3=[current_row3,current_column3];
    num3=0;

else 
    num3 =randi([1 4],1,1);

end 





end




