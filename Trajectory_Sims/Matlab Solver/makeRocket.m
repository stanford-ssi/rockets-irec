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

function [rocket] = makeRocket()

lbs2kg = 0.453592;   % kg/lbs
in22m2 = 0.00064516; % m^2/in^2
in2m   = 0.0254;     % m/in
ft2m   = 0.3048;     % m/ft

%%% General Parameters

rocket.OD = 4;          % in, outter diameter
rocket.drymass = 20     % lb
rocket.wetmass = 40     % lb

%%% Structures
% Assumptions: nose cone shoulder diameter is the same as the body

% Nose cone and shoulder
rocket.nose.wetmass = NaN        % lb, with payload
rocket.nose.drymass = NaN        % lb, without payload
rocket.nose.type = 'parabloid';
rocket.nose.l     = NaN          % ft
rocket.shoulder.l = NaN          % ft

% Forward airframe
rocket.fore.l = NaN     % ft, length of forward
rocket.fore.S = pi*(rocket.OD/2).^2; % ft^2, projected surface area

% Aft airframe
rocket.aft.l = NaN       % ft, length of aft
rocket.aft.wetmass = NaN % lb, fully loaded
rocket.aft.drymass = NaN % lb, no propellant

% Fins
rocket.fin.t = NaN    % in
rocket.fin.span = NaN % in
rocket.fin.rootchord = NaN % in
rocket.fin.tipchord = NaN % in
rocket.fin.sweep = rocket.fin.rootchord/2 - rocket.fin.tipchord/2;
rocket.fin.S = (rocket.fin.rootchord + rocket.fin.tipchord)/2*rocket.fin.span;
rocket.fin.AR = rocket.fin.sweep.^2 / rocket.fin.S;
rocket.fin.num = 3;

%-----------------------------------------

rocket.l = rocket.nose.l + rocket.fore.l + rocket.aft.l;

%%% Recovery

% Rocket parachutes
rocket.drogue.d = NaN                          % diameter
rocket.drogue.S = pi*(rocket.drogue.d/2).^2;
rocket.drogue.Cd = NaN                         % coefficient of drag
rocket.drogue.deploy_u = NaN                   % ft/s
rocket.main.d = NaN                            % in
rocket.main.S = pi.*(rocket.main.d/2).^2;      % in^2
rocket.main.Cd = NaN
rocket.main.deploy_h = NaN                     % in

% Payload chute
rocket.payload.chute_d = NaN
rocket.payload.chute_Cd = NaN

%% Convert to MKS



end