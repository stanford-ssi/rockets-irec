% getMotorData
% Grabs the name of the motor and automatically populates the motor
% characteristics to simplify motor switching and avoid simple input mistakes
% Ian Gomez,  08/23/16

function [motor,rocket,T,t_powered] = getMotorData(motor,rocket,time)

% Enter motor data folder (You need to start from the Drag_Fin folder as
% your directory, or this will not work
cd('Motors');
aerotech_str  = strcat('AeroTech_' ,strcat(motor.name,'.eng'));
cesaroni_str = strcat('Cesaroni_',strcat(motor.name,'.eng'));
if exist(aerotech_str,'file')
    motor_str = aerotech_str;
elseif exist(cesaroni_str,'file')
    motor_str = cesaroni_str;
else
    error('Motor does not exist or Motor manufacturer not set up in getMotorData')
end
fileID = fopen(motor_str);
motor_info = '';
while(1)         % eng files are not all the same format
    motor_info = fgets(fileID); % grabs mass information
    if motor_info(1) ~= ';' 
        break
    end
end
motor.thrust_curve = textscan(fileID,'%f %f'); % scans actual thrust info
fclose(fileID);
cd .. 

% Read in useful motor data (besides the thrust curve)
motor_info = strsplit(motor_info); 
motor.diameter = strcat(motor_info{2},'mm');      % mm
motor.length   = str2double(motor_info{3}).*1e-3; % m
motor.propmass = str2double(motor_info{5});       % kg 
motor.wetmass  = str2double(motor_info{6});       % kg

% Converts the cells to a matrix and removes the last line
thrust_curve = cell2mat(motor.thrust_curve);
t_thrust = thrust_curve(1:length(thrust_curve)-1,1).';
thrust_curve = thrust_curve(1:length(thrust_curve)-1,2).';

% Interpolate the data (keep time step low ~1e-2 to catch all thrust data)
if t_thrust(1) ~= 0; warning('Thrust file must start at 0s'); end
t_powered = 0:time.step:t_thrust(end);
T = interp1(t_thrust,thrust_curve,t_powered);

% Do some calculations for powered ascent properties
motor.drymass  = motor.wetmass-motor.propmass; % kg
motor.burntime = t_powered(end);   % s
motor.mdotavg  = motor.propmass./motor.burntime; % kg/s
rocket.drymass = rocket.nomotormass + motor.drymass; % kg
rocket.wetmass = rocket.nomotormass + motor.wetmass; % kg

end