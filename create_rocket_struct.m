% Create Rocket Struct
% Ian Gomez, 10/30/16
% This script creates a rocket object that can be used in multiple scripts
% eg use for terminal velocity profiles, max Q, or fin flutter calculations
% Units: inches for length, lbs for mass
clear; clc; close all;

% General rocket parameters
rocket.maxMa = 2;                    % Mach number
rocket.alpha = 0:0.1:8;              % angle of attack in deg
rocket.cg = 79;                      % in from nosecone tip
rocket.massunit = 'lbs';
rocket.lengthunit = 'in';

%% Structures

% Nose cone
rocket.nosecone.m = 2;       % lb
rocket.nosecone.OD = 4;      % in 
rocket.nosecone.length = 24; % in 

% Forward airframe
rocket.bodytube.m  = 24;                                   % lbs
rocket.bodytube.length = 44;                               % in
rocket.bodytube.OD = 4;                                    % in
rocket.bodytube.OR = rocket.bodytube.OD/2;                 % in
rocket.bodytube.t = 0.08;                                  % in 
rocket.bodytube.ID = rocket.bodytube.OD-rocket.bodytube.t; % in 
rocket.bodytube.IR = rocket.bodytube.ID/2;                 % in
rocket.bodytube.S_ref = pi * rocket.bodytube.OR^2;         % in^2

% Aft airframe
rocket.engine.m = 27.2;                                    % lbs
rocket.engine.OD = rocket.bodytube.OD;                     % in
rocket.engine.length =  52;                                % in

% Fins!
fin.t = (1/16):(1/16):(1/4);         % thickness
fin.h = 4;                           % span
fin.rootlength = 8;                  % root chord
fin.tiplength = 2;                   % tip chord
fin.sweepdistance = fin.rootlength/2 - fin.tiplength/2; % from top of root chord
fin.S = 0.5.*(fin.rootlength + fin.tiplength).*fin.h;   % in^2
fin.AR = (fin.sweepdistance.^2)/fin.S;                  % aspect ratio
fin.number = 3;

rocket.length = rocket.nosecone.length + rocket.bodytube.length +...
    rocket.engine.length;                              

%% Recovery

%Diameter of the parachute
rocket.nosecone.chute_S = (36.^2)/4 * pi;%36 inches
rocket.bodytube.chute_S = (48.^2)/4 * pi;%48 inches
rocket.engine.chute_S =(84.^2)/4 * pi;%84 inches 

%coefficient of drag for the parachutes
%Parachute drag based on circular parachute, from Knacke 5-25
rocket.parachutes.cd = 2.2;


%% Save Rocket

% Clean up workspace and save struct
rocket.fin = fin; clear fin
save('rocket') 
