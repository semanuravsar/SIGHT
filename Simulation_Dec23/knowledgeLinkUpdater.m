function [mU1new, mU2new, mU3new, bUnew] = knowledgeLinkUpdater(linkMatrix, mU1, mU2, mU3, bU  )
[R, C] = size(mU1);
mU1new = mU1;
mU2new = mU2;
mU3new = mU3;
bUnew = bU;

if linkMatrix(1,2) == 1
    for i= 1:R
        for j = 1:C
            if mU1new(i,j) == 0 && mU2new(i,j) ~= 0
                mU1new(i,j) = mU2new(i,j);
            elseif mU1new(i,j) ~= 0 && mU2new(i,j) == 0
                mU2new(i,j) = mU1new(i,j);
            else 
                mU1new(i,j) = max(mU1new(i,j),mU2new(i,j));
                mU2new(i,j) = max(mU1new(i,j),mU2new(i,j));
            end
        end
    end
end
if linkMatrix(1,3) == 1
    for i= 1:R
        for j = 1:C
            if mU1new(i,j) == 0 && mU3new(i,j) ~= 0
                mU1new(i,j) = mU3new(i,j);
            elseif mU1new(i,j) ~= 0 && mU3new(i,j) == 0
                mU3new(i,j) = mU1new(i,j);
            else 
                mU1new(i,j) = max(mU1new(i,j),mU3new(i,j));
                mU3new(i,j) = max(mU1new(i,j),mU3new(i,j));
            end
        end
    end
end
if linkMatrix(1,4) == 1
    for i= 1:R
        for j = 1:C
            if mU1new(i,j) == 0 && bUnew(i,j) ~= 0
                mU1new(i,j) = bUnew(i,j);
            elseif mU1new(i,j) ~= 0 && bUnew(i,j) == 0
                bUnew(i,j) = mU1new(i,j);
            else 
                mU1new(i,j) = max(mU1new(i,j),bUnew(i,j));
                bUnew(i,j) = max(mU1new(i,j),bUnew(i,j));
            end
        end
    end
end
if linkMatrix(2,3) == 1
    for i= 1:R
        for j = 1:C
            if mU2new(i,j) == 0 && mU3new(i,j) ~= 0
                mU2new(i,j) = mU3new(i,j);
            elseif mU2new(i,j) ~= 0 && mU3new(i,j) == 0
                mU3new(i,j) = mU2new(i,j);
            else 
                mU2new(i,j) = max(mU3new(i,j),mU2new(i,j));
                mU3new(i,j) = max(mU3new(i,j),mU2new(i,j));
            end
        end
    end
end
if linkMatrix(2,4) == 1
    for i= 1:R
        for j = 1:C
            if mU2new(i,j) == 0 && bUnew(i,j) ~= 0
                mU2new(i,j) = bUnew(i,j);
            elseif mU2new(i,j) ~= 0 && bUnew(i,j) == 0
                bUnew(i,j) = mU2new(i,j);
            else 
                bUnew(i,j) = max(bUnew(i,j),mU2new(i,j));
                mU2new(i,j) = max(bUnew(i,j),mU2new(i,j));
            end
        end
    end
end
if linkMatrix(3,4) == 1
    for i= 1:R
        for j = 1:C
            if mU3new(i,j) == 0 && bUnew(i,j) ~= 0
                mU3new(i,j) = bUnew(i,j);
            elseif mU3new(i,j) ~= 0 && bUnew(i,j) == 0
                bUnew(i,j) = mU3new(i,j);
            else 
                mU3new(i,j) = max(mU3new(i,j),bUnew(i,j));
                bUnew(i,j) = max(mU3new(i,j),bUnew(i,j));
            end
        end
    end
end

end