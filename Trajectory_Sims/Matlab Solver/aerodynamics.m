% Aerodynamics
% Defines the aerodynamic forces acting on the rocket
% Ian Gomez, Rushal Rege 04/11/2017

function [Fdrag, Flift] = aerodynamics(rocket, r, u, aerodata)
% rev 0 = simple drag model (1/2 rho CD v^2)
% rev 1 = use CD table from RASAero 
% rev 2 = use OpenRocket subsonic drag and call RASAero drag past transonic

aoa = r(3); % angle of attack

% Calculate atmosphere 
[~,~,rho,local_c] = getAtmoConditions(r(2));

mach = norm(u(1:2))/local_c;

% 1 Check for mach number; 
k = 1; tol = 0.01;
while(abs(mach-aerodata(k,1)) > tol)
    k = k + 1;
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
    disp('yo, aoa > 4')
end

S = pi .* (rocket.OD.^2)./4; % axial surface area
Fdrag = CD .* S .* 0.5 .* rho .* (cosd(aoa) .* norm(u(1:2))).^2; 
Flift = CL .* S .* 0.5 .* rho .* (sind(aoa) .* norm(u(1:2))).^2; 

end