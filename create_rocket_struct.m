% Create Rocket Struct
% Ian Gomez, 10/30/16
% This script creates a rocket object that can be used in multiple scripts
% eg use for terminal velocity profiles, max Q, or rocket.fin flutter calculations
% Units: inches for length, lbs for mass

% Instructions on how to use:
% The rocket is represented by a struct which can hold any property
% (relevant for simulation). To add a property to the struct, the syntax
% (as seen below) is rocket.object.property or if it applies directly to
% the entire rocket, rocket.property
% This struct is saved as a .mat workspace which can be loaded in a script
% easily using load('rocket') (as long as you're in the correct directory)
% The script has a difficult time rewriting information, so delete the old
% struct if you actually change anything and rerun the script.
clear; clc; close all;

%% General rocket parameters

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
rocket.fin.t = (1/16):(1/16):(1/4);         % thickness
rocket.fin.h = 4;                           % span
rocket.fin.rootlength = 8;                  % root chord
rocket.fin.tiplength = 2;                   % tip chord
rocket.fin.sweepdistance = rocket.fin.rootlength/2 - rocket.fin.tiplength/2; % from top of root chord
rocket.fin.S = 0.5.*(rocket.fin.rootlength + rocket.fin.tiplength).*rocket.fin.h;   % in^2
rocket.fin.AR = (rocket.fin.sweepdistance.^2)/rocket.fin.S;                  % aspect ratio
rocket.fin.number = 3;

rocket.length = rocket.nosecone.length + rocket.bodytube.length +...
    rocket.engine.length;                              

%% Recovery

% Parachutes
rocket.nosecone.chute_D = 36; % in 
rocket.bodytube.chute_D = 48; % in 
rocket.engine.chute_D   = 84; % in
rocket.nosecone.chute_S = (rocket.nosecone.chute_D.^2)/4 * pi; % in^2
rocket.bodytube.chute_S = (rocket.bodytube.chute_D.^2)/4 * pi; % in^2
rocket.engine.chute_S   = (rocket.engine.chute_D.^2)/4 * pi;   % in^2 

% Coefficient of drag for the parachutes
rocket.parachutes.cd = 2.2;


%% Save Rocket

% Clean up workspace and save struct
save('rocket') 
