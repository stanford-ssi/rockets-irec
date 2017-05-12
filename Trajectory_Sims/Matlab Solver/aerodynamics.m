% Aerodynamics
% Defines the aerodynamic forces acting on the rocket
% Ian Gomez, Rushal Rege 04/11/2017

function [Fdrag, Flift] = aerodynamics(t, r, u, wind, aerodata, rocket)
% rev 0 = simple drag model (1/2 rho CD v^2)
% rev 1 = use CD table from RASAero
% rev 2 = use OpenRocket subsonic drag and call RASAero drag past transonic

% find angle of attack; see photo for help
% mathisfun.com/algebra/trig-solving-sas-triangles
umag = norm(u(1:2));
phi = 90-r(3);
resultant = umag.^2 + wind.^2 - 2.*umag.*wind.*cosd(phi);
lambda = asind(wind.*sind(phi)./resultant);
aoa = 180 - lambda - phi;

% Calculate atmosphere
[~, ~, rho, local_c, mu] = getAtmoConditions(r(2));

% Check for mach number
mach = umag/local_c;
k = 1; tol = 0.01;
while(1)
    delta = abs(mach-aerodata(k,1));
    if delta < tol; break; end;
    k = k + 1;
end

ft2m = 0.3048;
rocket_length = 11*ft2m;
Re_rocket_y = rho*umag*rocket_length/mu;

% determine what state the rocket is in aerodynamically
% add rail length to avoid tipping; for now use if faster than rail
% velocity

rail_length = 20; % m, which is pretty ridiculous
launch_site = 1293; % m, magic number
aoa = 0;
if u(2) >= 0
    
    if r(2) <= rail_length+launch_site
        aoa = 0;
    end
    
    % Compare angle of attack and use weighted average
    % aerodata = [RASMa, CD0, CD2, CD4, CL0, CL2, CL4];
    if abs(aoa) <= 2
        weight = aoa/2;
        CD = weight * aerodata(k,3) + (1-weight) * aerodata(k,2);
        CL = weight * aerodata(k,6) + (1-weight) * aerodata(k,5);
    elseif abs(aoa) <= 4
        weight = (aoa-2)/2;
        CD = weight * aerodata(k,4) + (1-weight) * aerodata(k,3);
        CL = weight * aerodata(k,7) + (1-weight) * aerodata(k,6);
    else
        CD = 1e8; % yikes
        CL = 1e8; % yikes
        disp('yo, aoa > 4')
    end
    
    S = pi .* (rocket.OD.^2)./4; % axial surface area
    
    % do signs
    Fdrag = CD .* S .* 0.5 .* rho .* (cosd(aoa) .* umag).^2;
    Flift = CL .* S .* 0.5 .* rho .* (sind(aoa) .* umag).^2;
    
elseif u(2) < rocket.drogue.deploy_u && r(2) > rocket.main.deploy_h
    Fdrag = 20;
    Flift = 0;
else
    Fdrag = 100;
    Flift = 0;
end

end