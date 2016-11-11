% Dispersion
% Ian Gomez, 11/11/16

% Initial conditions, measurements, variables, etc.
clc; clear; 

% Make sure to run create_rocket_struct first and you're in Recovery
% directory
cd ..
load('rocket')
cd('Recovery')

windspeed = 7;  % m/s

dt = 0.02;      % s
t_end = 10*60;   % s
t = 0:dt:t_end; % s

[ax,ay,ux,uy,x,h,time_land,index_land] = dispersion_fun(rocket,dt,t,windspeed);
t = t(1:index_land); % cut off the vector 

%% Plotting

scaling = 1e3;
s2min   = 1/60; % min/s 


figure(1)
plot(t,x./scaling)
xlabel('time (s)')
ylabel('displacement (km)')
title('Displacement in x direction')
xlim([0 time_land])

figure(2)
plot(t,ux)
xlabel('time (s)')
ylabel('velocity (m/s)')
title('Velocity in x direction')
xlim([0 time_land])

figure(3)
plot(t,h./scaling)
ylabel('height (km)')
xlabel('time (s)')
title('Height v time')
xlim([0 time_land])

figure(4)
plot(t,uy)
ylabel('velocity (m/s)')
xlabel('time (s)')
title('Velocity in y direction')
xlim([0 time_land])

figure(5)
plot(x./scaling,h./scaling)
xlabel('displacement (km)')
ylabel('h (km)')
title('Height v Displacement')
