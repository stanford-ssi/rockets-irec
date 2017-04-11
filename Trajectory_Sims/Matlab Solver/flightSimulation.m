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
r = [0, site_elevation, 0]; % m, position,         r = [x,  y,  theta]
u = [0, 0, 0];              % m/s, velocity,       u = [vx, vy, omega]
a = [0, 0, 0];              % m/s^2, acceleration, a = [ax, ay, alpha]
aoa = 0;                    % deg, angle of attack (changes with time)

% Receive motor data [struct, Thrust curve, how many seconds of thrust]
% t.powered is the length of time the motor is on 
[motor, T, t.powered] = getMotorData(motor, t.step);
rocket.mass = rocket.wetmass;

for i = 2:2:length(t.ime) %????????????? by 2
    % determine the set of forces on the system
    % forces calls aerodynamics module to get those forces, gravity force
    % is calculated on the latest rocket struct, wind force will be
    % calculated in the next revision, motor thrust is calculated by the
    % motor curve (csv). 
    % pass in the t struct for both time step and time powered 
    [f_x, f_y, f_theta] = forces(rocket, t.ime(i), r, u, a, aoa, T, i);
    
    % forces are passed into the solver and the next set of r, u, and a are
    % appended to their respective vectors
    [r, u, a] = solver(rocket, f_x, f_y, f_theta, r, u, a, t, i);
end

% Plotter
makePlots(r, u, a, f_x, f_y, f_theta)
