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

% Characteristics of the plate
in2m = 0.0254;             % in/m
t = .125.*in2m;            % m, thickness
b = 6.*in2m;               % m, distance from rod
h = 3.*in2m;               % m
Al.E = 68.9e9;             % Pa
Al.tensile_yield  = 276e6; % Pa
Al.shear_strength = 207e6; % Pa
Al.thermal_cond   = 167;   % W/(m*K)
Al.poisson        = 0.334;

% Solve for atmospheric conditions: temperature, pressure, density and
% speed of sound at deployment altitude
[T,P,~,sp_sound] = getAtmoConditions(h);

% Grab highest Mach number that dragfins experience (deployment) and solve
% for the stagnation pressure (maximum pressure)
Ma = dragfin.deploy_u./sp_sound;
P0 = P*(1+((k-1)/2).*Ma.^2).^(k./(k-1));

% Simple Beam Analysis
% Worst case scenario + beam theory. Basically assume that stag pressure is
% impinging on the entirety of the plate (open at 90deg to the flow and
% isentropically slowing it down to 0 m/s)
% Fin is cantilevered at the rod (since it resists both moment and x & y
% motion)
dragfin.failure = 0; % resets failure of drag fins
Fy = -P0.*b.*h; % in negative y direction
My = -Fy.*b./2;
Ix = (b.*h.^3)./12; % m^4
sigma = My./Ix;
dragfin.beam_bending_stress = sigma;

% Failure Checks
if dragfin.beam_bending_stress > Al.tensile_yield
    dragfin.failure = 1;
    disp('Drag fins failed in bending')
end

dragfin