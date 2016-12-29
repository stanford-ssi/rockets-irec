% getTemperature
% Grabs atmospheric temperature at an altitude
% www.engineeringtoolbox.com/international-standard-atmosphere-d_985.html
% Ian Gomez,  08/18/16

% Input = altitude [m]; Output = temperature [K]

function [T,P,rho,a] = getAtmoConditions(h)



% Pressure [Pa]
g = 9.81; % m/s^2
avogadro = 6.0221409e23;
k_boltz = 1.38064852e-23;   % J/K
m_air   = 29/avogadro*1e-3; % kg
R_air =  287;               % J/kg K
P_sl  = 101300;          % Pa, sea level pressure 
k = 1.4;          % Air specific heats ratio
P = P_sl.*exp((-m_air.*g.*h)./(k_boltz.*T)); % Pa

% Density [kg/m^3]
rho = 1.2250.*(288.15./(288.15-0.0065.*h)).^...
    (1+9.80665.*0.0289644./(8.31432.*0.0065));

% Mach number
a  = sqrt(k.*R_air.*T); % m/s, local speed of sound

end