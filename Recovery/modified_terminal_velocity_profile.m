% Terminal Velocity Simulation v2
% Simlulates terminal velocity based on parachutes and air density.
% Unknown author
% Modified: Ian Gomez, 10/30/16

% Initial conditions, measurements, variables, etc.
clc; clear; 

% Make sure to run create_rocket_struct first and you're in Recovery
% directory
cd ..
load('rocket')
cd('Recovery')

% Calculations of the Decent Profile 
h0 = 9000; % m, ~ 30,000ft
dt = 0.1; % s, time step

[rocket.v,rocket.h,n] = modified_terminal_velocity_fun(rocket,dt);


% Each iteration represents .1 seconds, so multiply by dt to get
% seconds
rocket.t = n*dt;


rocket.t = (1:length(rocket.h)).*dt;
%%

linewidth = 2;
figure(1)
plot(rocket.t, rocket.v,'LineWidth',linewidth);
xlabel('Time (s)'); 
ylabel('Terminal Velocity (m/s)'); 
title('Descent Profile'); 
grid on; 