%Set initial conditions for mu's
mu1=[2,2];
mu2=[9,9];
mu3=[5,8];
bu=[2,5];

current_row1=mu1(1);
current_column1=mu1(2);

current_row2=mu2(1);
current_column2=mu2(2);

current_row3=mu3(1);
current_column3=mu3(2);

targetLocation = [3, 4];
target_row=targetLocation(1);
target_col=targetLocation(2);


num1 =randi([1 4]); % random number decide which way to move for mu1
num2 =randi([1 4]); % random number decide which way to move for mu2
num3 =randi([1 4]); % random number decide which way to move for mu3

numberofiterations=100;

%Create empty arrays for the mu's location for all time
mu1_all=cell(1,numberofiterations);
mu2_all=cell(1,numberofiterations);
mu3_all=cell(1,numberofiterations); 

%set initial locations for the 1st element
mu1_all{1}=[current_row1,current_column1];
mu2_all{1}=[current_row2,current_column2];
mu3_all{1}=[current_row3,current_column3];

for i=2:numberofiterations

if num1==1
  current_row1=move_up(current_row1);

elseif num1==2
    current_row1=move_down(current_row1,10);

elseif num1==3
    current_column1= right(current_column1,10);

elseif num1==4
   current_column1= left(current_column1);
end

mu1_all{i}=[current_row1,current_column1];

if num2==1
 current_row2= move_up(current_row2);

elseif num2==2
 current_row2=  move_down(current_row2,10);

elseif num2==3
  current_column2= right(current_column2,10);

elseif num2==4
    current_column2=left(current_column2);
end

mu2_all{i}=[current_row2,current_column2];

if num3==1
  current_row3=move_up(current_row3);

elseif num3==2
    current_row3=move_down(current_row3,10);

elseif num3==3
       current_column3= right(current_column3,10);

elseif num3==4
    current_column3 =left(current_column3);
end

mu3_all{i}=[current_row3,current_column3];

%If one of the mu's found the target, it stops
%--------------------------------------------------------------------------

if (current_row1==target_row && current_column1==target_col)
    num1=0;
    mu1=[current_row1,current_column1];

else 
    num1 =randi([1 4],1,1);
end 
%--------------------------------------------------------------------------
if (current_row2==target_row&& current_column2==target_col)
   num2=0;
   mu2=[current_row2,current_column2];

else 
    num2 =randi([1 4],1,1);
end 
%--------------------------------------------------------------------------
if (current_row3==target_row&& current_column3==target_col)
    mu3=[current_row3,current_column3];
    num3=0;
else 
    num3 =randi([1 4],1,1);

end 
%--------------------------------------------------------------------------

end

%Simulate the locations of mu's during whole time
robotSimulation(mu1_all,mu2_all,mu3_all,bu,targetLocation,numberofiterations)



