function [T_stag, P_stag, rho_stag, Tr] = Stagnation_Properties(v, P, rho, T, Pr, Re)

%Gets speed of sound
%Should probably adjust for variable specific heats. Does not do so
gamma = 1.4;
c = sqrt(gamma * P / rho);

%Gets Mach number
M = v / c;

%Gets properties
P_stag = P * (1 + (gamma - 1) / 2 * M^2)^(gamma / (gamma - 1));
T_stag = T * (1 + (gamma - 1) / 2 * M^2);
rho_stag = rho * (1 + (gamma - 1) / 2 * M^2)^(1 / (gamma - 1));

%Laminar
if Re < 2300
    r = sqrt(Pr);
else
%Turbulent or transient
    r = Pr^(1/3);
end

Tr = T * ( 1 + r * (gamma - 1) / 2 * M^2);
end