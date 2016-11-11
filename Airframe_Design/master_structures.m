% Master Structures Script
% Ian Gomez,  10/25/2016
% This script is meant to call the new methods we've implemented to drive
% airframe design (ie flutter, fin ability to generate lift and drag, and
% max Q)
% Units: speed = [ft/s or m/s], length = [in], pressure = [psi or Pa],
% force = [lb or N], moment = [lb-in or N-m]

clc; clear;

ft2m = 0.3048;                   % ft/m
RASAero_csv = 'Final2900.CSV';   % Flight data from RASAero
data = csvread(RASAero_csv,2,1); % Need to modify csv first for parsing
RAD.Ma = data(:,2);              % Mach number
RAD.v  = data(:,16).*ft2m;       % m/s
RAD.h  = data(:,21).*ft2m;       % m
RAD.cd = data(:,4);              % coefficient of drag
RAD.t  = data(:,1);

%%

% Rocket characteristics

% Make sure to run create_rocket_struct first and you're in Recovery
% directory
cd ..
load('rocket')
cd('Airframe_Design')

% Max dynamic pressure
metric = 0; % in metric == 1
maxq = max_q(RAD,metric);
[compression_force, sigma] = aero_loads2(RAD,maxq,rocket,metric);

% Flutter Velocity based on different fin thickness
% currently set to aluminum
fin = flutter_velocity(RAD,rocket.fin);

% Only useful for subsonic flight
Ma_CNalpha = 0.8;
CN_alpha_total = CNalpha_compute(rocket.fin, rocket.bodytube, Ma_CNalpha);