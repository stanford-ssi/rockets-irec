% Master Structures Script
% Ian Gomez,  10/25/2016
% This script is meant to call the new methods we've implemented to drive
% airframe design (ie flutter, fin ability to generate lift and drag, and
% max Q)
% Units: speed = [ft/s or m/s], length = [in], pressure = [psi or Pa],
% force = [lb or N], moment = [lb-in or N-m]

clc; clear; close all;

ft2m = 0.3048;                   % ft/m
RASAero_csv = '4in_CF.CSV';      % Flight data from RASAero
data = csvread(RASAero_csv,2,1); % Need to modify csv first for parsing
RAD.Ma = data(:,2);              % Mach number
RAD.v  = data(:,13).*ft2m;       % m/s
RAD.h  = data(:,18).*ft2m;       % m

% Rocket characteristics

rocket.alpha = 0:0.1:8;                  % angle of attack in deg
rocket.cg = 81;                      % in from nosecone tip
rocket.length = 107;                 % in

bodytube.lengthunit = 'in';
bodytube.OD = 4;                     % in
bodytube.OR = bodytube.OD/2;         % in
bodytube.t = 0.08;                   % in 
bodytube.ID = bodytube.OD-bodytube.t;% in 
bodytube.S_ref = pi * bodytube.OR^2; % in^2

fin.lengthunit = 'in';
fin.t = (1/16):(1/16):(1/4);         % thickness
fin.h = 4;                           % span
fin.rootlength = 8;                  % root chord
fin.tiplength = 2;                   % tip chord
fin.sweepdistance = fin.rootlength/2 - fin.tiplength/2; % from top of root chord
fin.S = 0.5.*(fin.rootlength + fin.tiplength).*fin.h;   % in^2
fin.AR = (fin.sweepdistance.^2)/fin.S;                  % aspect ratio
fin.number = 3;

rocket.bodytube = bodytube;
rocket.fin = fin;

% Max dynamic pressure
metric = 1; % not in metric
maxq = max_q(RAD,metric);
[compression, sigma] = aero_loads(maxq,rocket,metric);

% Flutter Velocity based on different fin thickness
% currently set to aluminum
fin = flutter_velocity(RAD,rocket.fin);

% Only useful for subsonic flight
Ma_CNalpha = 0.8;
CN_alpha_total = CNalpha_compute(rocket.fin, rocket.bodytube, Ma_CNalpha);