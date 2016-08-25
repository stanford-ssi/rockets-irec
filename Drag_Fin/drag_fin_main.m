% Drag Fin Analysis
% Ian Gomez,  08/15/16
% Joan Creus, 08/16/16

% This script runs a simulation of the rocket in flight
% Implementation of drag fins
% Look at the potential energy (PE = m*g*h) on the control flight
% We want to reduce PE to PE_desired = m*g*h_desired
% PE-PE_desired = Drag * dragfin.dist_to_apogee

clear; close all; clc
in2m = 0.0254;               % in/m

% Plots
linesize = 2;                % line width on plots
plot_landing       = 0;      % 1 plots time up to landing, 0 plots to apogee
plot_thrust        = 0;      % plot the thrust curve
plot_h_u_a         = 0;      % plot h, u, and a separately
plot_combined_hu   = 0;      % plot h & u together
plot_h             = 0;      % plot just h
plot_forces        = 0;      % doesn't plot the parachute or drogue drag
plot_recovery_drag = 0;      % plots the parachute and drogue drag

% IREC Rocket Baseline is current rocket
rocket.l   = 3.44;           % length, m
rocket.d   = 0.14;           % diameter, m
rocket.Cd  = 0.6;            % coeff of drag
rocket.nomotormass  = 19.43; % kg
rocket.deploydrogue = 1;     % 1 will deploy drogue,    0 will not
rocket.deployparachute = 1;  % 1 will deploy parachute, 0 will not

% Recovery Information
parachute.d        = 6.1;    % m
parachute.deploy_h = 450;    % m
parachute.Cd       = 0.8;    % coefficient of drag
drogue.Cd          = 0.8;    % coefficient of drag
drogue.d           = 1.22;   % m
drogue.deploy_u    = -1;     % m/s

% Drag Fin
dragfin.deploy_t = 10;            % s, -1 will not deploy drag fins
dragfin.extra_drag_percent = 1.2; % *100%
dragfin.max_theta = 17;           % deg

plate.max_theta   = dragfin.max_theta;
plate.t = .125.*in2m;        % m, thickness
plate.b = 6.*in2m;           % m, distance from rod
plate.h = 6.*in2m;           % m
plate.S = plate.b*plate.h;   % m^2

rod.max_theta     = dragfin.max_theta;
rod.t = 0.5.*in2m;           % m
rod.b = 3.*in2m;             % m
rod.h = rod.t;               % m
rod.S = rod.b*rod.h;         % m^2

% Motor Selection (just need impulse class + avg thrust)
% Check motors_available for what motors exist in the folder
cd('Motors'); motors_available = ls; cd ..;
motors = {'M1939'};

% Simulation Inputs
time.step = 8e-3;            % Choose time step, currently only <0.02 works
time.end  = 600;             % Choose the duration of the simulation
altitude.launch_site = 1219; % m
altitude.target      = 3048; % m
rocket.launch_angle = 0;     % deg
g = 9.81;                    % m/s^2

for i = 1:length(motors)
    motor.name = motors{i}
    
    % Simulation
    [h,u,a,time,t,t_powered,mach1,rocket,gravityloss,T,dragloss,...
        parachutedrag,droguedrag,e,dragfin] = runSimulation(rocket,motor,...
        parachute,drogue,altitude,dragfin,time,g);
    
    % Simulation Plots
    plot_options = [plot_landing,plot_thrust,plot_h_u_a,plot_combined_hu,...
        plot_h,plot_forces,plot_recovery_drag];
    getPlots(plot_options,time,t,t_powered,mach1,gravityloss,T,dragloss,...
        parachutedrag,droguedrag,h,u,a,altitude,motor,dragfin,g);
    
    % Simulation Results
    disp('Additional percentage of energy need to lose to drag')
    disp(strcat(num2str(e.loss_perc.*100),'%'))
    disp('Additional drag needed to hit target')
    disp(strcat(num2str(dragfin.extra_D_req),'N'))
    
    % Stress Analysis
    if dragfin.deploy_t > 0
        [dragfin,plate,rod] = getStressAnalysis(dragfin,plate,rod,h);
    end
    
end

% Useful for cleaning up workspace for drag_fin_stress
clearvars plot_landing plot_thrust plot_h_u_a plot_combined_hu ...
    plot_h plot_forces plot_recovery_drag linesize