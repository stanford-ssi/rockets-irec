% Drag Fin Stress Analysis
% Ian Gomez,  08/17/16
% This script attempts to do a simplified analysis of a the drag fins in
% order to characterize the order of magnitudes of stress

function   [dragfin,plate,rod] = getStressAnalysis(dragfin,plate,rod,h)

if dragfin.deploy_t < 0; disp('No analysis required'); end

Al.E = 68.9e9;                   % Pa
Al.tensile_yield  = 276e6;       % Pa
Al.shear_strength = 207e6;       % Pa
Al.thermal_cond   = 167;         % W/(m*K)
Al.poisson        = 0.334;

% Solve for atmospheric conditions: temperature, pressure, density and
% speed of sound at deployment altitudes
[T,P,rho,sp_sound] = getAtmoConditions(h);
k = 1.4; % specific heat ratio of air

% Grab highest Mach number that dragfins experience (deployment) and solve
% for the stagnation pressure (maximum pressure)
dam = sin(dragfin.max_theta.*pi./180); % deployment angle multiplier
rho = rho(dragfin.deploy_index); 
Ma  = dragfin.deploy_u./sp_sound;
P0  = P(dragfin.deploy_index).*(1+((k-1)/2).*Ma.^2).^(k./(k-1));
P0_normal = P0.*dam;

% -------------------------------------------------------------------------
% Simple Beam Analysis
% -------------------------------------------------------------------------
% Worst case scenario + beam theory. Basically assume that stag pressure is
% impinging on the entirety of the plate (open at 90deg to the flow and
% isentropically slowing it down to 0 m/s)
% Moment center at beginning of the rod

plate.Fy = -P0_normal.*plate.b.*plate.h;     % N, in negative y direction
plate.My = -plate.Fy.*(plate.b./2 + rod.b);  % N*m
plate.Ix = (plate.t.*plate.h.^3)./12;        % m^4
rod.Fy   = -P0_normal.*rod.b.*rod.h;         % N, in negative y direction
rod.My   = -rod.Fy.*(rod.b./2);              % N*m
rod.Ix   = (rod.t.*rod.h.^3)./12;            % m^4

rod.My_total = plate.My + rod.My;
rod.sigma = rod.t./2.*rod.My_total/rod.Ix;

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
else
    disp('Nothing broke... yet')
end

% -------------------------------------------------------------------------
% Aerodynamic Analysis
% -------------------------------------------------------------------------

dragfin.S_normal = dam.*(plate.S + rod.S);
dragfin.drag = -(plate.Fy + rod.Fy);
dragfin.Cd   = dragfin.drag./(0.5.*rho.*dragfin.deploy_u.*dragfin.S_normal);

end