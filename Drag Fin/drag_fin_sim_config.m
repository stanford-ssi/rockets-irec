% Drag Fin Sim Config
% Ian Gomez,  08/15/16
% Joan Creus, 08/16/16
% This script runs a simulation of the rocket in flight with the goal of
% simulating the flight profile + drag fins

clear; close all; clc

% Plots
linesize = 2;  % line width

plot_landing       = 0; % 1 plots time up to landing, 0 plots to apogee
plot_thrust        = 0;
plot_h_u_a         = 0;
plot_combined_hu   = 0;
plot_h = 1;
plot_forces        = 1; % doesn't plot the parachute or drogue drag
plot_recovery_drag = 0;

% IREC Rocket Baseline is current rocket
rocket.l   = 3.44;           % length, m
rocket.d   = 0.14;           % diameter, m
rocket.Cd  = 0.6;            % coeff of drag
rocket.nomotormass = 19.428; % kg
rocket.deploydrogue = 0;
rocket.deployparachute = 0;

% Recovery Information
parachute.d  = 6.1;       % m
parachute.deploy_h = 450; % m
parachute.Cd = 0.8;
drogue.Cd = 0.8;
drogue.d  = 1.22;         % m
drogue.deploy_u = -1;     % m/s

% Specify the motor you want
% Input motor data (always check that these values match chosen motor)
motor.name     = 'M1939';
motor.wetmass  = 8.9882;  % kg
motor.propmass = 5.7191;  % kg

time_step = 0.02;

altitude.launch_site = 1219; % m

altitude.target = 3048;           % m
dragfin.deploy_t = 10;            % s
dragfin.extra_drag_percent = 1.2; % *100%
rocket.launch_angle = 0.*pi./180; % rad

% Run sim
[dragfin] = drag_fin_sim(linesize,rocket,parachute,drogue,...
    motor,altitude,dragfin,time_step);

rocket
motor
dragfin
parachute
drogue
disp('Open rocket, motor, dragfin, parachute, & drogue structs for more info')