function trajectory_matrix = trajectory_maker(current_position,destination)
    
    trajectory_matrix = [];
    a = abs(destination(1) - current_position(1));
    b = abs(destination(2) - current_position(2));

    if (destination(1)>current_position(1))
        while(a>0)
        trajectory_matrix = [trajectory_matrix; [1,0]];
        a = a-1;
        end
    else
        while(a>0)
        trajectory_matrix = [trajectory_matrix; [-1,0]];
        a = a-1;
        end
    end

    if (destination(2)>current_position(2))
        while(b>0)
        trajectory_matrix = [trajectory_matrix; [0,1]];
        b = b-1;
        end
    else
        while(b>0)
        trajectory_matrix = [trajectory_matrix; [0,-1]];
        b = b-1;
        end
    end




end