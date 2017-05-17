% Initial Conditions
% These items don't need to run every single time the simulation is run
% Ian Gomez, May 16, 2017

m2mi = 0.000621371;
in2m = 0.0254;
m2ft = 3.28084;
lbs2N = 4.44822162825;

NM = 1293; % m, site elevation
motor.name  = 'N2900';

% Local site conditions and make rocket struct
% Note: measure CP and CM from nose of rocket
site_elevation = NM; % m
goal = 9144;
goalkft = 30e3; % ft
band = 2e3; % ft

% Set up simulation
t_step = 0.01; % for motor data interpolation
uINT = [0, 0, 0];              % m/s, velocity,       u = [vx, vy, omega]

% Receive motor data [struct, Thrust curve, how many seconds of thrust]
% t.powered is the length of time the motor is on
[motor, T, ~] = getMotorData(motor, t_step);
T = [(0:t_step:motor.burntime)', T'];  % make thrust a paired vector

% Pull RASAero aerodynamics data
csvnum1 = 2500; csvnum2 = csvnum1*2;
RASdata = csvread('RASAero_N2900_bestweather_aerodata.csv', 1, 0);
RASMa = RASdata(1:csvnum1,1);
CD0 = RASdata(1:csvnum1,3);
CD2 = RASdata(csvnum1+1:csvnum2,3);
CD4 = RASdata(csvnum2:end,3);
CL0 = RASdata(1:csvnum1,8);
CL2 = RASdata(csvnum1+1:csvnum2,8);
CL4 = RASdata(csvnum2:end,8);
CP0 = RASdata(1:csvnum1,13);
CP2 = RASdata(csvnum1+1:csvnum2,13);
CP4 = RASdata(csvnum2:end,13);
aerodata = [zeros(1,10); RASMa, CD0, CD2, CD4, CL0, CL2, CL4,...
    CP0*in2m, CP2*in2m, CP4*in2m];
clearvars csvnum1 csvnum2 RASdata RASMa CD0 CD2 CD4 CL0 CL2 CL4

% Pull RASAero flight data
RASFlight = csvread('RASAero_N2900_bestweather.CSV',1,0);
ras_t   = RASFlight(:,1);        % s, time
ras_CD  = RASFlight(:,4);        % coefficient of drag
ras_CL  = RASFlight(:,5);        % coefficient of lift
ras_T   = RASFlight(:,6).*lbs2N; % lb, thrust
ras_W   = RASFlight(:,7).*lbs2N; % lb, weight
ras_D   = RASFlight(:,8).*lbs2N; % lb, drag
ras_L   = RASFlight(:,9).*lbs2N; % lb, lift
ras_CM  = RASFlight(:,10);       % in from nose
ras_CP  = RASFlight(:,11);       % in from nose
ras_alt = RASFlight(:,21);       % ft, altitude
ras_v   = RASFlight(:,16);       % ft/s, velocity mag
ras_vy  = RASFlight(:,17);       % ft/s
ras_vx  = RASFlight(:,18);       % ft/s
ras_theta = RASFlight(:,20);     % degrees, path angle