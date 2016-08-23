% Drag Fin Stress Analysis
% Ian Gomez,  08/17/16
% This script attempts to do a simplified analysis of a the drag fins in
% order to characterize the order of magnitudes of stress

% Types of loading
% Distributed load on top face
% No axial loadings

% Need to populate dragfin struct
if exist('dragfin','var')==0; run('drag_fin_sim'); end
clc; close all; format short eng

in2m = 0.0254;                   % in/m

% Characteristics of the plate & rod
plate.t = .125.*in2m;            % m, thickness
plate.b = 6.*in2m;               % m, distance from rod
plate.h = 3.*in2m;               % m

rod.t = 1.4.*in2m;                 % m
rod.b = 3.*in2m;                 % m
rod.h = rod.t;                   % m

Al.E = 68.9e9;                   % Pa
Al.tensile_yield  = 276e6;       % Pa
Al.shear_strength = 207e6;       % Pa
Al.thermal_cond   = 167;         % W/(m*K)
Al.poisson        = 0.334;

% Solve for atmospheric conditions: temperature, pressure, density and
% speed of sound at deployment altitude
[T,P,~,sp_sound] = getAtmoConditions(h);

% Grab highest Mach number that dragfins experience (deployment) and solve
% for the stagnation pressure (maximum pressure)
Ma = dragfin.deploy_u./sp_sound;
P0 = P(dragfin.deploy_index).*(1+((k-1)/2).*Ma.^2).^(k./(k-1));

% Simple Beam Analysis
% Worst case scenario + beam theory. Basically assume that stag pressure is
% impinging on the entirety of the plate (open at 90deg to the flow and
% isentropically slowing it down to 0 m/s)
% Moment center at beginning of the rod

plate.Fy = -P0.*plate.b.*plate.h;     % in negative y direction
plate.My = -plate.Fy.*(plate.b./2 + rod.b);
plate.Ix = (plate.b.*plate.h.^3)./12; % m^4
rod.Fy   = -P0.*rod.b.*rod.h;         % in negative y direction
rod.My   = -rod.Fy.*(rod.b./2);
rod.Ix   = (rod.b.*rod.h.^3)./12;     % m^4

dragfin.My_total = plate.My + rod.My;
rod.sigma = dragfin.My_total/rod.Ix;

% Need shearing stress calculation
% tau = V*Q/(I*t) where V = shear derived from equations of beam under
% transverse loading, I = based on shape of entire structure, t = width of
% area of interest, and Q = the first moment of area = y_bar*A

% Failure Checks
dragfin.failure = 0; % resets failure of drag fins
if rod.sigma > Al.tensile_yield
    dragfin.failure = 1;
    disp('Rod failed in bending')
% elseif plate.sigma > Al.tensile_yield
%     dragfin.failure = 1;
%     disp('Plate failed in bending')
end

dragfin
plate
rod