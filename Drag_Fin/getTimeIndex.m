% getTimeIndex
% Find time index of an event at a particular t(index) since time is not
% always created with the same time step in the simulation
% Ian Gomez, 08/28/16

function [index] = getTimeIndex(t,t_find,tol)

for i = 1:length(t)
    if abs(t(i)-t_find) < tol
        index = i; 
    end
end

end