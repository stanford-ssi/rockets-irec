% getTemperature
% Grabs atmospheric temperature at an altitude
% www.engineeringtoolbox.com/international-standard-atmosphere-d_985.html
% Ian Gomez,  08/18/16

% Input = altitude [m]; Output = temperature [K]

function [T,P,rho,a] = getAtmoConditions(h)

% T [K]
if h < 500
    T = 288.15;
elseif h < 1000
    T = 284.9;
elseif h < 1500
    T = 281.7;
elseif h < 2000
    T = 275.2;
elseif h < 2500
    T = 271.9;
elseif h < 3000
    T = 268.7;
elseif h < 3500
    T = 265.4;
elseif h < 4000
    T = 262.2;
elseif h < 4500
    T = 258.9;
elseif h < 5000
    T = 255.7;
elseif h < 5500
    T = 252.4;
elseif h < 6000
    T = 249.2;
elseif h < 6500
    T = 245.9;
elseif h < 7000
    T = 242.7;
elseif h < 7500
    T = 239.5;
elseif h < 8000
    T = 236.2;
elseif h < 8500
    T = 233.0;
elseif h < 9000
    T = 229.7;
elseif h < 9500
    T = 226.5;
elseif h < 10e3
    T = 223.3;
else
    warning('Temperature data only available for altitudes below 10e3')
end

% Pressure [Pa]
g = 9.81;                   % m/s^2
avogadro = 6.0221409e23;
k_boltz = 1.38064852e-23;   % J/K
m_air   = 29/avogadro*1e-3; % kg
R_air =  287;               % J/kg K
P_sl  = 101300;             % Pa, sea level pressure 
k = 1.4;                    % Air specific heats ratio
P = P_sl.*exp((-m_air.*g.*h)./(k_boltz.*T)); % Pa

% Density [kg/m^3]
rho = 1.2250.*(288.15./(288.15-0.0065.*h)).^...
    (1+9.80665.*0.0289644./(8.31432.*0.0065));

% Mach number
a  = sqrt(k.*R_air.*T); % m/s, local speed of sound

end