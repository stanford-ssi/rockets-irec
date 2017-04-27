%%% Forces

% Fdrag, Flift, Thrust curve, and Flight data are provided as inputs
% Flight Data consists of rocket dimensions, time, position, vel,
% acceleration, angle of attack
% Assumption is time starts at t = 0;
% u >= 0, haven't hit apogee yet
% Earth Centered Earth Fixed coordinate system
% rocket contains the mass curve

% Split each force into x and y for earth centered components

function [f_x, f_y, moment] = forces(rocket, t, r, u, T, current_mass)

ind = t/t.step;

theta = r(3); % wrt to the vertical (normal to the earth's surface)

% thrust
% thrust curve assumption -- 2 column tabular input
% find index for this
if t < T(end,1)
    thrust = T(ind,2);
    Tx = sind(theta)*thrust;
    Ty = cosd(theta)*thrust;
else
    Tx = 0;
    Ty = 0;
end

% where do we assign direction / theta and signs
[Fdrag, Flift] = aerodynamics(rocket, r, u, aerodata);

% lift
Lx = cosd(theta)*Flift;
Ly = sind(theta)*Flift;

% drag

% speed at which the drogue deploys (since it doesn't deploy at apogee)
drogue_deploy_speed = -1;

% are not assigned a sign yet
Dx = sind(theta)*Fdrag;
Dy = cosd(theta)*Fdrag;

% gravity
% mass assumption -- 2 column tabular input
% mdot proportional to thrust (relate to impulse)
G = 3.986E14; r_earth = 6378000;
gravity = (G)/(r_earth + norm(r(1:2))) * current_mass;

% Requires CM and CP distance from bottom of the rocket
if u >= 0
    f_x = Tx + Lx - Dx;
    f_y = Ty + Ly - Dy  - gravity;
    moment = (Lx - Dx)*cosd(theta)*(CM-CP) + (Dy + Ly)*sind(theta)*(CM-CP);
else
    f_x = Dx;
    f_y = Dy - gravity;
    moment = 0; %not worth it
end

end
