% getMotorData
% Grabs the name of the motor and automatically populates the motor
% characteristics to simplify motor switching and avoid simple input mistakes
% Ian Gomez,  08/23/16

function [motor,rocket,T,t_powered] = getMotorData(motor,rocket,time)

% Enter motor data folder
listOfFiles = ls; if ismember('Motors',listOfFiles); cd('Motors'); end
motor_str = strcat('AeroTech_',strcat(motor.name,'.txt'));
fileID = fopen(motor_str);
motor.thrust_curve = textscan(fileID,'%f %f','headerLines',5);
fclose(fileID);

% All masses are in kg
switch motor.name
    case 'M1939'
        motor.wetmass  = 8.9882;  
        motor.propmass = 5.7191;  
    case 'M2500'
        motor.wetmass  = 8.064;  
        motor.propmass = 4.659; 
    otherwise
        warning('Motor information does not exist in this database.')
end

% Converts the cells to a matrix and removes the last line
thrust_curve = cell2mat(motor.thrust_curve);
t_thrust = thrust_curve(1:length(thrust_curve)-1,1);
thrust_curve = thrust_curve(1:length(thrust_curve)-1,2);

% Interpolate the data, original data samples at 0.05s or 20 Hz
% Change time step to add discrete data points to the simulation
% Currently the time step doesn't like to go above 0.02
t_powered = 0:time.step:t_thrust(end);
T = interp1(t_thrust,thrust_curve,t_powered);

% Do some calculations for powered ascent properties
motor.drymass  = motor.wetmass-motor.propmass; % kg
motor.burntime = t_powered(end);   % s
motor.mdotavg  = motor.propmass./motor.burntime; % kg/s
rocket.drymass = rocket.nomotormass + motor.drymass; % kg
rocket.wetmass = rocket.nomotormass + motor.wetmass; % kg

% Once done, leave motor data folder
cd ..

end