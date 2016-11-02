function [compression_force, sigma] = aero_loads2(RAD,maxq,rocket,metric)
% Ian Gomez, Oct 28, 2016
% This function spits out the maximum compressive load and bending moment
% on the rocket based on max Q, angle of attack, and rocket geometry
% Converts numbers at the end

% Finds max speed index 
maxv_index = find(RAD.v == max(RAD.v));

% Axial drag provides compression force
% Drag = cd * 1/2 * area * rho * v^2
compression_force = zeros(length(rocket.alpha),length(maxq));
for i = 1:length(rocket.alpha)
        compression_force(i) = 0.5.*maxq.*cosd(rocket.alpha(i))...
            .*rocket.bodytube.S_ref.*RAD.cd(maxv_index);
end

sigma = 0;


%% 

lbs2N = 4.448222; % N/lbs
if metric == 1
    compression_force = lbs2N.*compression_force;
end

figure(2)
set(gcf,'color','w');
plot(rocket.alpha,compression_force)
if metric == 1
    ylabel('Compression force (N)')
    xlabel('Angle of attack (deg)')
else
    ylabel('Compression force (lbs)')
    xlabel('Angle of attack (deg)')
end
title('Max compression with different angle of attacks')
grid on

end