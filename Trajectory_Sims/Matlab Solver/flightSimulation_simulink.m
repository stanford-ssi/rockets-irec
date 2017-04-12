%%% Flight Simulation 
% Ian Gomez, 03/5/17
clc; clear all;

% Set up launch site (currently just site elevations)
sites = csvread('launchSites.csv',2,0);
NAR = sites(:,1); TCC = sites(:,2);
FAR = sites(:,3); NM  = sites(:,4);

% Choose site and launch configuration
site = NAR; % gives a site elevation output (for now)
launch_site = 4; % corresponds to a configuration 
motor.name  = 'N2900';

% Local site conditions and make rocket struct
Temp = 291; % K
wind = 2.2352; % m/s
launch_angle = 0; % deg
site_elevation = site(1,1); % m
ground_conditions = [Temp, wind, launch_angle, site_elevation];
rocket = makeRocket(launch_site); % returns rocket struct
CP = NaN; % time varying - pull from RASAero!
CM = NaN; % time varying - can set up beforehand or done in getMotorData

% Set up simulation
% t.property is a way to track when events occur.
t.step = 0.01;              % s, dt
t.ime = 0:t.step:120;        % s, time
rINT = [0, site_elevation, 0]; % m, position,         r = [x,  y,  theta]
uINT = [0, 0, 0];              % m/s, velocity,       u = [vx, vy, omega]
a = [0, 0, 0];              % m/s^2, acceleration, a = [ax, ay, alpha]
aoa = 0;                    % deg, angle of attack (changes with time)

% Receive motor data [struct, Thrust curve, how many seconds of thrust]
% t.powered is the length of time the motor is on 
[motor, T, t.powered] = getMotorData(motor, t.step);
rocket.mass = rocket.wetmass;

% sim('solver_simulink.slx')

% Plotter
% makePlots(rINT, uINT, a, f_x, f_y, f_theta)
