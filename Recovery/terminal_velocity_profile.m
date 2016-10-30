% Terminal Velocity Simulation v2
% Simlulates terminal velocity based on parachute and air density.
% Unknown author
% Modified: Ian Gomez, 10/30/16

%% Initial conditions, measurements, variables, etc.
%units (m,kg)
clc; clear; close all

rocket.nosecone.m = 4.535;
rocket.nosecone.d = .10551;
rocket.nosecone.l = .5588;

rocket.payload.m = 11.34;
rocket.payload.d = 0.152; 
rocket.payload.l = 1.021; 

rocket.engine.m = 27.2;
rocket.engine.d = rocket.payload.d;
rocket.engine.l =  1.374; 

rocket.fin.S = .088; 

%Diameter of the parachute
rocket.nosecone.chute_S = (0.9144.^2)/4 * pi;%36 inches
rocket.payload.chute_S = (1.2192.^2)/4 * pi;%48 inches
rocket.engine.chute_S =(2.1336.^2)/4 * pi;%84 inches 

%coefficient of drag for the parachutes
%Parachute drag based on circular parachute, from Knacke 5-25
rocket.parachute.cd = 2.2;

%% Calculations of the Decent Profile 

[rocket.nosecone.v,rocket.nosecone.h,n_ns] = terminal_velocity_sim(...
    rocket.nosecone.m,rocket.parachute.cd,rocket.nosecone.chute_S);

[rocket.payload.v,rocket.payload.h,n_pl] = terminal_velocity_sim(...
    rocket.payload.m,rocket.parachute.cd,rocket.payload.chute_S);

[rocket.engine.v,rocket.engine.h,n_eng] = terminal_velocity_sim(...
    rocket.engine.m,rocket.parachute.cd,rocket.engine.chute_S);

%in seconds (each iteration represents .1 seconds, so divide by 10 to get
%seconds)
dt = 0.1;
rocket.nosecone.t = n_ns*dt;
rocket.payload.t = n_pl*dt;
rocket.engine.t = n_eng*dt;

hold on
%velocities are negative, so multiplied by -1 to make positive
plot(rocket.nosecone.h(80:end),  rocket.nosecone.v(80:end) .*-1,...
     rocket.payload.h(80:end),  rocket.payload.v(80:end) .*-1,... 
     rocket.engine.h(80:end), rocket.engine.v(80:end).* -1);
legend('Nosecone', 'Payload', 'Engine');
xlabel('Heights (m)'); 
ylabel('Terminal Velocities (m/s)'); 
title('Descent Profiles'); 
grid on; 