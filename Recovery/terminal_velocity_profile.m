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
[rocket.nosecone.v,rocket.nosecone.h,n_ns] = terminal_velocity_fun(...
    rocket.nosecone.m,rocket.parachutes.cd,rocket.nosecone.chute_S);

[rocket.bodytube.v,rocket.bodytube.h,n_pl] = terminal_velocity_fun(...
    rocket.bodytube.m,rocket.parachutes.cd,rocket.bodytube.chute_S);

[rocket.engine.v,rocket.engine.h,n_eng] = terminal_velocity_fun(...
    rocket.engine.m,rocket.parachutes.cd,rocket.engine.chute_S);

%in seconds (each iteration represents .1 seconds, so divide by 10 to get
%seconds)
dt = 0.1;
rocket.nosecone.t = n_ns*dt;
rocket.bodytube.t = n_pl*dt;
rocket.engine.t = n_eng*dt;

hold on
%velocities are negative, so multiplied by -1 to make positive
plot(rocket.nosecone.h(80:end),  rocket.nosecone.v(80:end) .*-1,...
     rocket.bodytube.h(80:end),  rocket.bodytube.v(80:end) .*-1,... 
     rocket.engine.h(80:end), rocket.engine.v(80:end).* -1);
legend('Nosecone', 'Body', 'Engine');
xlabel('Heights (m)'); 
ylabel('Terminal Velocities (m/s)'); 
title('Descent Profiles'); 
grid on; 