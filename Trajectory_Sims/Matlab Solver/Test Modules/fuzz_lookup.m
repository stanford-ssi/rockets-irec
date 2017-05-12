clear all; clc;

lbs2kg = 0.453592;   % kg/lbs
rocket.drymass = 30*lbs2kg;     % lb
rocket.wetmass = 61.27*lbs2kg;     % lb

motor.name  = 'N2900';

% Set up simulation
t_step = 0.001; % for motor data interpolation

% Receive motor data [struct, Thrust curve, how many seconds of thrust]
% t.powered is the length of time the motor is on 
[motor, T, ~] = getMotorData(motor, t_step);
T = [(0:t_step:motor.burntime)', T'];  % make thrust a paired vector
mdot = (rocket.wetmass-rocket.drymass)./motor.burntime;
mass = [(0:t_step:motor.burntime)',... % make thrust a paired vector
    (rocket.wetmass:-mdot*t_step:rocket.drymass)'];

k = 1; tol = t_step;
while(1)
    delta = abs(mass(k,1)-t);
    if delta < tol; break; end
    k = k+1;
end
