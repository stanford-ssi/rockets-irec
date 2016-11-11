% Terminal Velocity Simulation v2
% Simlulates terminal velocity based on parachutes and air density.
% Unknown author
% Modified: Ian Gomez, 10/30/16

% Initial conditions, measurements, variables, etc.
clc; clear; close all

% Make sure to run create_rocket_struct first and you're in Recovery
% directory
cd ..
load('rocket')
cd('Recovery')

% Calculations of the Decent Profile 
h0 = 9000; % m, ~ 30,000ft
dt = 0.1; % s, time step

[rocket.nosecone.v,rocket.nosecone.h,n_ns] = terminal_velocity_fun(...
    rocket.nosecone.m,rocket.parachutes.cd,rocket.nosecone.chute_S,dt);

[rocket.bodytube.v,rocket.bodytube.h,n_pl] = terminal_velocity_fun(...
    rocket.bodytube.m,rocket.parachutes.cd,rocket.bodytube.chute_S,dt);

[rocket.engine.v,rocket.engine.h,n_eng] = terminal_velocity_fun(...
    rocket.engine.m,rocket.parachutes.cd,rocket.engine.chute_S,dt);

% Each iteration represents .1 seconds, so multiply by dt to get
% seconds
rocket.nosecone.t = n_ns*dt;
rocket.bodytube.t = n_pl*dt;
rocket.engine.t = n_eng*dt;

rocket.nosecone.t = 1:length(rocket.nosecone.h);
rocket.bodytube.t = 1:length(rocket.bodytube.h);
rocket.engine.t   = 1:length(rocket.engine.h);

%%

% Velocities are negative, so multiplied by -1 to make positive
linewidth = 2;
figure(1)
plot(rocket.nosecone.h,  abs(rocket.nosecone.v),...
     rocket.bodytube.h,  abs(rocket.bodytube.v),... 
     rocket.engine.h,    abs(rocket.engine.v),'LineWidth',linewidth);
legend('Nosecone', 'Body', 'Engine');
xlabel('Heights (m)'); 
ylabel('Terminal Velocities (m/s)'); 
xlim([0 h0])
title('Descent Profiles'); 
grid on; 