%%% Forces

% Fdrag, Flift, Thrust curve, and Flight data are provided as inputs
% Flight Data consists of rocket dimensions, time, position, vel, 
% acceleration, angle of attack
% Assumption is time starts at t = 0;
% u >= 0, haven't hit apogee yet
% Earth Centered Earth Fixed coordinate system
% rocket contains the mass curve

% Split each force into x and y for earth centered components

function [f_x, f_y, moment] = forces(rocket, t, r, u, T)

aoa = u(3); % angle of attack

% thrust
% thrust curve assumption -- 2 column tabular input
% find index for this
if t < T(end,1)
    thrust = T(i,2);
    Tx = sind(aoa)*thrust;
    Ty = cosd(aoa)*thrust;
else
    Tx = 0;
    Ty = 0;
end

[Fdrag, Flift] = aerodynamics(rocket, r, u, aerodata);  % mostly finished

% do we need to split again?

% lift
if u >= 0
    Lx = cosd(aoa)*Flift;
    Ly = sind(aoa)*Flift;
else
    Lx = 0;
    Ly = 0;
end

% drag
if u>= 0
    Dx = sind(aoa)*Fdrag;
    Dy = cosd(aoa)*Fdrag;
else
    Dx = 0; %cylinder in flow approximation and parachutes? look up
    Dy = 0; %Parachute Drag and base of rocket (ignore blockage and swinging)
end

% gravity
% mass assumption -- 2 column tabular input
% mdot proportional to thrust (relate to impulse)
gravity = (3.986E14)/(6378000 + norm(r)) * rocket.mass(i);

% Requires CM and CP distance from bottom of the rocket
if u >= 0
    f_x = Tx + Lx - Dx;
    f_y = Ty - Dy - Lx - gravity;
    moment = (Lx - Dx)*cosd(aoa)*(CM-CP) ...
        + (Dy + Ly)*sind(aoa)*(CM-CP);
else
    f_x = Dx; 
    f_y = Dy - gravity;
    moment = 0; %not worth it
end

end
