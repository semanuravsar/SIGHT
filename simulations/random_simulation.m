
current_row1 =0 ;
current_column1=0;
mu1 =[current_row1,current_column1];
current_row2=0;
current_column2=0;
current_row3=0;
current_column3=0;
target_row=5;
target_col=5;
num1 =randi([1 4],1,1);
num2 =randi([1 4],1,1);
num3 =randi([1 4],1,1);


%There are 4 movement choices for all mobile units (num1,num2,num3)
%This number are randomly chosen to randomize the movement of mobile units

for i=1:100

% movement options for mu1
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


% movement options for mu2
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

% movement options for mu3

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



% if the target is found numbers are equalized to 0 so that they do not
% move anymore. Otherwise, they are equalized to another random number to
% keep moving

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




