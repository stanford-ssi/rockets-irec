%%% Make Rocket
% Ian Gomez, 03/5/17

% Enter in rocket parameters in imperial units

% The rocket is represented by a struct which can hold any property
% (relevant for simulation). To add a property to the struct, the syntax
% (as seen below) is rocket.object.property or if it applies directly to
% the entire rocket, HoS.property

% Input: Launch # (to pick configuration)
% Output: Rocket struct

% Note: Only final launch will be simulated for now

function [rocket] = makeRocket(ballast)

lbs2kg = 0.453592;   % kg/lbs
in2m   = 0.0254;     % m/in
ft2m   = 0.3048;     % m/ft

%%% General Parameters

rocket.OD = 4*in2m;          % in, outter diameter
rocket.drymass = (43+ballast)*lbs2kg;     % lb
rocket.wetmass = (62.8+ballast)*lbs2kg;     % lb

%%% Structures
% Assumptions: nose cone shoulder diameter is the same as the body

% Nose cone and shoulder
rocket.nose.wetmass = 15*lbs2kg;        % lb, with payload
rocket.nose.drymass = 13*lbs2kg;        % lb, without payload
rocket.nose.type = 'parabloid';
rocket.nose.l     = 2.104*ft2m;          % ft
rocket.shoulder.l = 5/6*ft2m;          % ft

% Forward airframe
rocket.fore.l = 3.94*ft2m;     % ft, length of forward
rocket.fore.S = pi*(rocket.OD/2).^2; % ft^2, projected surface area

% Aft airframe
rocket.aft.l = 4.7917*ft2m;       % ft, length of aft
rocket.aft.wetmass =  31*lbs2kg; % lb, fully loaded
rocket.aft.drymass =  19*lbs2kg; % lb, no propellant

% Fins
rocket.fin.t = 0.125*in2m;    % in
rocket.fin.span = 3.2*in2m; % in
rocket.fin.rootchord = 10*in2m; % in
rocket.fin.tipchord = 2*in2m; % in
rocket.fin.sweep = rocket.fin.rootchord/2 - rocket.fin.tipchord/2;
rocket.fin.S = (rocket.fin.rootchord + rocket.fin.tipchord)/2*rocket.fin.span;
rocket.fin.AR = rocket.fin.sweep.^2 / rocket.fin.S;
rocket.fin.num = 3;

%-----------------------------------------

% rocket.l = rocket.nose.l + rocket.fore.l + rocket.aft.l;
rocket.l = 10.9167*ft2m;

%%% Recovery

% Rocket parachutes
rocket.drogue.d = 31*in2m; % in                          
rocket.drogue.S = pi*(rocket.drogue.d/2).^2;
rocket.drogue.Cd = 0.75;                         % coefficient of drag
rocket.drogue.deploy_u = -5*ft2m;                   % ft/s
rocket.main.d =   96*in2m;                          % in
rocket.main.S = pi.*(rocket.main.d/2).^2;      % in^2
rocket.main.Cd = 2.2;
rocket.main.deploy_h = 1500*ft2m; %ft                     

% Payload chute
rocket.payload.chute_d = 18*in2m;
rocket.payload.chute_Cd = 1;

end