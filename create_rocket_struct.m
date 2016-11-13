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
% Make sure to save the struct in the correct folder (the root folder)
clear; clc; 
ft2in = 12;          % in/ft
lbs2kg = 0.453592;   % kg/lbs
in22m2 = 0.00064516; % m^2/in^2
in2m   = 0.0254;     % m/in
fts2ms = 0.3048;     % (m*s^-1)/(ft*s^-1)

%% General rocket parameters

rocket.maxMa = 2;                    % Mach number
rocket.alpha = 0:0.1:8;              % angle of attack in deg
rocket.cg = 79;                      % in from nosecone tip
rocket.drymasswpayload = 42.03;      % lbs
rocket.drymass = 32.67;              % lbs
rocket.massunit = 'lbs';
rocket.lengthunit = 'in';

%% Structures

% Nose cone
rocket.nosecone.wetmass = 11.8178;       % lb
rocket.nosecone.drymass = 2.45178;       % lb
rocket.nosecone.OD = 4;                  % in 
rocket.nosecone.length = 28;             % in 
rocket.nosecone.finenessratio = 4;       % in 
rocket.nosecone.S_x = rocket.nosecone.length.*rocket.nosecone.OD.*0.5; %in^2
rocket.nosecone.type = 'conical';
rocket.nosecone.Cd = 1; % crossflow Cd

% Forward airframe
rocket.bodytube.mass =  13.81;                              % lbs
rocket.bodytube.length = 42;                                % in
rocket.bodytube.OD = 4;                                     % in
rocket.bodytube.OR = rocket.bodytube.OD/2;                  % in
rocket.bodytube.t = 0.08;                                   % in 
rocket.bodytube.ID = rocket.bodytube.OD-rocket.bodytube.t;  % in 
rocket.bodytube.IR = rocket.bodytube.ID/2;                  % in
rocket.bodytube.S_ref = pi * rocket.bodytube.OR^2;          % in^2
rocket.bodytube.S_x   = rocket.bodytube.OD.*rocket.bodytube.length;
rocket.bodytube.Cd = 1; % crossflow Cd


% Aft airframe
rocket.engine.wetmass = 31.23;                              % lbs
rocket.engine.drymass = 12.6;                               % lbs
rocket.engine.OD = rocket.bodytube.OD;                      % in
rocket.engine.length =  52;                                 % in
rocket.engine.S_x = rocket.engine.OD.*rocket.engine.length;
rocket.engine.Cd = 1; % crossflow Cd


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
rocket.drogue.d  = 48;                         % in
rocket.drogue.S  = pi.*(rocket.drogue.d/2).^2; % in^2
rocket.drogue.Cd = 1.5;      
rocket.drogue.deploy_u  = -0.2;                % ft/s
rocket.main.d    = 96;                         % in
rocket.main.S    = pi.*(rocket.main.d/2).^2;   % in^2
rocket.main.Cd   = 2.2;                 
rocket.main.deploy_h    =  10000.*ft2in;       % in
rocket.payload.chute__d = 1;                   % in^2
rocket.payload.chute_Cd = 2; 


%% Save Rocket

% Clean up workspace and save struct
save('rocket') 
