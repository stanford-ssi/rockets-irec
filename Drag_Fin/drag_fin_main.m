% Drag Fin Analysis
% Ian Gomez,  08/15/16
% Joan Creus, 08/16/16
% This script runs a simulation of the rocket in flight with the goal of
% simulating the flight profile + drag fins

% Implementation of drag fins
% Look at the potential energy (PE = m*g*h) on the control flight
% We want to reduce PE to PE_desired = m*g*h_desired
% PE-PE_desired = D_df * t_open
% where D_df is the drag of the drag fins and t_open is time the drag fins
% are deployed
% You can choose the deployment time of the drag fins and the percentage of
% extra drag they cause the rocket

clear; close all; clc

% -------------------------------------------------------------------------
% Inputs
% -------------------------------------------------------------------------

% Plots
linesize = 2;                % line width on plots
plot_landing       = 0;      % 1 plots time up to landing, 0 plots to apogee
plot_thrust        = 0;      % plot the thrust curve
plot_h_u_a         = 0;      % plot h, u, and a separately
plot_combined_hu   = 0;      % plot h & u together
plot_h             = 1;      % plot just h
plot_forces        = 1;      % doesn't plot the parachute or drogue drag
plot_recovery_drag = 0;      % plots the parachute and drogue drag

% IREC Rocket Baseline is current rocket
rocket.l   = 3.44;           % length, m
rocket.d   = 0.14;           % diameter, m
rocket.Cd  = 0.6;            % coeff of drag
rocket.nomotormass  = 19.43; % kg
rocket.deploydrogue = 1;     % 1 will deploy drogue, 0 will not
rocket.deployparachute = 1;  % 1 will deploy parachute, 0 will not

% Recovery Information
parachute.d        = 6.1;    % m
parachute.deploy_h = 450;    % m
parachute.Cd       = 0.8;    % coefficient of drag
drogue.Cd          = 0.8;    % coefficient of drag
drogue.d           = 1.22;   % m
drogue.deploy_u    = -1;     % m/s

% Motor Selection
motors = {'M1939','M2500'};
motor.name = motors{1};

% Simulation Inputs
time.step = 0.02;            % Choose time step, currently only <0.02 works
time.end  = 400;             % Choose the duration of the simulation
altitude.launch_site = 1219;      % m
altitude.target = 3048;           % m
dragfin.deploy_t = -1;            % s, -1 will not deploy drag fins
dragfin.extra_drag_percent = 1.2; % *100%
rocket.launch_angle = 0.*pi./180; % rad
g = 9.81;                         % m/s^2

% -------------------------------------------------------------------------
% Simulation
% -------------------------------------------------------------------------

[h,u,a,time,t,t_powered,mach1,rocket,gravityloss,T,dragloss,...
    parachutedrag,droguedrag] = runSimulation(rocket,motor,parachute,...
    drogue,altitude,dragfin,time,g);

% -------------------------------------------------------------------------
% Simulation Plots
% -------------------------------------------------------------------------

plot_options = [plot_landing,plot_thrust,plot_h_u_a,plot_combined_hu,...
    plot_h,plot_forces,plot_recovery_drag];
getPlots(plot_options,time,t,t_powered,mach1,gravityloss,T,dragloss,...
    parachutedrag,droguedrag,h,u,a,altitude,motor,dragfin,g);

% -------------------------------------------------------------------------
% Drag fin energy characteristics
% -------------------------------------------------------------------------

% Energy calculations [J]
e_net = rocket.drymass.*g(1).*rocket.apogee;
e_want = rocket.drymass.*g(end).*altitude.target;
e_loss = e_net - e_want;
e_loss_perc = (e_net - e_want)/e_want;
disp('Additional percentage of energy need to lose to drag')
disp(strcat(num2str(e_loss_perc.*100),'%'))

% Find index of distance to altitude target from altitude at fin deployment
% Setting t_fins_deployed below 0 will effectively stop them from deploying
if dragfin.deploy_t > 0
    disp('Drag fins were deployed')
    
    tol = time.step; % this allows you to put in precise times for t_deploy
    for i = 1:length(t)
        if abs(t(i)-dragfin.deploy_t) < tol
            dragfin.deploy_index = i;
        end
    end
    dragfin.deploy_u = u(dragfin.deploy_index);
    dragfin.deploy_h = h(dragfin.deploy_index);
    d2at =  altitude.target - dragfin.deploy_h; % m
    D_df = e_loss./d2at;                          % N
    
else
    disp('Drag fins were not deployed')
    % If we deploy fins at 3113m
    d2at = altitude.target - 3113; % m
    D_df = e_loss./d2at;
end

disp('Additional drag needed to hit target')
disp(strcat(num2str(D_df),'N'))

