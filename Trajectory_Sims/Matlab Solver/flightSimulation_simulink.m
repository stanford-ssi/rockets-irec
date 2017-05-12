%%% Flight Simulation 
% Ian Gomez, 03/5/17
clc; clear all;

NM = 1293; % m, site elevation
motor.name  = 'N2900';

% Local site conditions and make rocket struct
% Note: measure CP and CM from nose of rocket
Temp = 291; % K
wind = 2.2352; % m/s 
launch_angle = 0; % deg
site_elevation = NM; % m
ground_conditions = [Temp, wind, launch_angle, site_elevation];
rocket = makeRocket(); % returns rocket struct
CP = 0.2032; % time varying - pull from RASAero!
CM = 0; % time varying - can set up beforehand or done in getMotorData

% Set up simulation
t_step = 0.01; % for motor data interpolation
rINT = [0, site_elevation, 0]; % m, position,         r = [x,  y,  theta]
uINT = [0, 0, 0];              % m/s, velocity,       u = [vx, vy, omega]

% Receive motor data [struct, Thrust curve, how many seconds of thrust]
% t.powered is the length of time the motor is on 
[motor, T, ~] = getMotorData(motor, t_step);
T = [(0:t_step:motor.burntime)', T'];  % make thrust a paired vector
mdot = (rocket.wetmass-rocket.drymass)./motor.burntime;
mass = [(0:t_step:motor.burntime)',... % make thrust a paired vector
    (rocket.wetmass:-mdot*t_step:rocket.drymass)']; 

% Pull RASAero data
csvnum1 = 2500; csvnum2 = csvnum1*2;
RASdata = csvread('RASAero_N2900.csv', 1, 0);
RASMa = RASdata(1:csvnum1,1);
CD0 = RASdata(1:csvnum1,3);
CD2 = RASdata(csvnum1+1:csvnum2,3);
CD4 = RASdata(csvnum2:end,3);
CL0 = RASdata(1:csvnum1,8);
CL2 = RASdata(csvnum1+1:csvnum2,8);
CL4 = RASdata(csvnum2:end,8);
aerodata = [zeros(1,7); RASMa, CD0, CD2, CD4, CL0, CL2, CL4];

% figure out how to clear simulink workspace
save('import2simulink','rocket')
sim('solver_simulink.slx')

%%

% Plotter
figure(1)
plot(tout,stateout(:,2))
title('Altitude')
xlabel('time (s)')
ylabel('altitude (m)')
ylim([site_elevation site_elevation+10e3])

figure(2)
plot(tout,stateout(:,5))
title('Y Velocity')
xlabel('time (s)')
ylabel('velocity (m/s)')

