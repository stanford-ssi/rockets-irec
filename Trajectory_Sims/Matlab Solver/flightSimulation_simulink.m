%%% Flight Simulation
% Ian Gomez, 03/5/17
clc; clear all;

m2ft = 3.28084;
lbs2N = 4.44822162825;

NM = 1293; % m, site elevation
motor.name  = 'N2900';

% Local site conditions and make rocket struct
% Note: measure CP and CM from nose of rocket
wind = 2;
site_elevation = NM; % m
goal = site_elevation + 9144;
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
RASdata = csvread('RASAero_N2900_aerodata.csv', 1, 0);
RASMa = RASdata(1:csvnum1,1);
CD0 = RASdata(1:csvnum1,3);
CD2 = RASdata(csvnum1+1:csvnum2,3);
CD4 = RASdata(csvnum2:end,3);
CL0 = RASdata(1:csvnum1,8);
CL2 = RASdata(csvnum1+1:csvnum2,8);
CL4 = RASdata(csvnum2:end,8);
aerodata = [zeros(1,7); RASMa, CD0, CD2, CD4, CL0, CL2, CL4];
clearvars csvnum1 csvnum2 RASdata RASMa CD0 CD2 CD4 CL0 CL2 CL4

%% Run Simulation

% figure out how to clear simulink workspace
save('import2simulink','rocket'); tic
simdata = sim('solver_simulink.slx'); toc

%% Pull out data from simulink
tout = simdata.get('tout'); stateout = simdata.get('stateout');
rx = stateout(:,1); ry = stateout(:,2); theta = stateout(:,3);
ux = stateout(:,4); uy = stateout(:,5); omega = stateout(:,6);

forceout_data = simdata.get('forceout'); forceout = squeeze(forceout_data);
fx   = forceout(1,:);  fy   = forceout(2,:);  moment = forceout(3,:);
drag = forceout(4,:);  lift = forceout(5,:);  CD     = forceout(6,:);
CL   = forceout(7,:);  aoa  = forceout(8,:);  weight = forceout(9,:);

flight.apogee = max(ry);
k = 1; tol = 1e-2;
while(1)
    delta = abs(ry(k)-flight.apogee);
    if delta < tol; break; end
    k = k+1;
end
flight.apogee = flight.apogee.*m2ft;
flight.t_apogee = tout(k);
flight
%% Extract RASAero Data

RASFlight = csvread('RASAero_N2900_bestweather.CSV',1,0);
ras_t   = RASFlight(:,1);        % s, time
ras_CD  = RASFlight(:,4);        % coefficient of drag
ras_CL  = RASFlight(:,5);        % coefficient of lift
ras_T   = RASFlight(:,6).*lbs2N; % lb, thrust
ras_W   = RASFlight(:,7).*lbs2N; % lb, weight
ras_D   = RASFlight(:,8).*lbs2N; % lb, drag
ras_L   = RASFlight(:,9).*lbs2N; % lb, lift
ras_alt = RASFlight(:,21);       % ft, altitude
ras_v   = RASFlight(:,16);       % ft, velocity

%% Plots
close all;
scaling = 1e3;
xend = tout(end);

% altitude
figure(1); set(gcf,'color','w'); hold on % in imperial units
goal_line = (goal-site_elevation).*m2ft.*ones(1,length(tout));
alt = ry-site_elevation;
plot(tout, goal_line./scaling,'--', tout, alt.*m2ft./scaling,...
    ras_t,ras_alt./scaling)
title('Altitude'); legend('goal','simulink','RASAero','Location','Southeast')
xlabel('time (s)'); ylabel('altitude (kft)'); grid on
xlim([0, xend])

% x velocity
figure(2); set(gcf,'color','w'); hold on % imperial units
plot(tout,ux.*m2ft)
title('X Velocity');
xlabel('time (s)'); ylabel('velocity (ft/s)'); grid on
xlim([0, xend])

% y velocity
figure(3); set(gcf,'color','w'); hold on % imperial units
plot(tout, uy.*m2ft)
plot(ras_t, ras_v)
title('Y Velocity'); legend('simulink','RASAero')
xlabel('time (s)'); ylabel('velocity (ft/s)'); grid on
xlim([0, xend])

% drag
figure(6); set(gcf,'color','w'); hold on
yyaxis left
plot(tout, drag, ras_t, ras_D) 
xlabel('time (s)'); ylabel('drag (N)'); grid on
yyaxis right
plot(tout, CD, ras_t, ras_CD)
title('Drag'); legend('drag','RASAero drag','CD','RASAero CD')
xlim([0, xend])

% % thrust
% figure(3); set(gcf,'color','w'); hold on
% plot(T(:,1), T(:,2), ras_t, ras_T)
% title('Thrust'); legend('thrust','RASAero thrust')
% xlabel('time (s)'); ylabel('thrust (N)'); grid on
% xlim([0, xend])

% % weight
% figure(5); hold on
% set(gcf,'color','w')
% plot(tout, weight, ras_t, ras_W)
% title('Weight'); legend('simulink','RASAero')
% xlabel('time (s)'); ylabel('weight (N)'); grid on
% xlim([0, xend])
