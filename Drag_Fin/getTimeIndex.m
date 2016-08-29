function [index] = getTimeIndex(t,t_find,tol)

for i = 1:length(t)
    if abs(t(i)-t_find) < tol
        index = i; 
    end
end

end