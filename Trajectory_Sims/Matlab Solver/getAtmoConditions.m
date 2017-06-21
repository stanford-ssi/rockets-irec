% getTemperature
% Grabs atmospheric temperature at an altitude
% www.engineeringtoolbox.com/international-standard-atmosphere-d_985.html
% Ian Gomez,  08/18/16

% Input = altitude [m]; Output = temperature [K]

function [T, P, rho, a, mu] = getAtmoConditions(h, h0, Tlaunch)

% h = h + h0;
T0 = [Tlaunch, 216.66, 216.66, 282.66, 282.66, 165.66, 165.66, 225.66];
Trate = [-6.5e-3, 0, 3e-3, 0, -4.5e-3, 0, 4e-3]; % uses standard model lapse rate
hpos  = [11e3, 25e3, 47e3, 53e3, 79e3, 90e3, 105e3];

if h <= hpos(1)
    T = T0(1) + Trate(1)*(hpos(1)-h);
elseif h <= hpos(2)
    T = T0(2) + Trate(2)*(hpos(2)-h);
elseif h <= hpos(3)
    T = T0(3) + Trate(3)*(hpos(3)-h);
elseif h <= hpos(4)
    T = T0(4) + Trate(4)*(hpos(4)-h);
elseif h <= hpos(5)
    T = T0(5) + Trate(5)*(hpos(5)-h);
elseif h <= hpos(6)
    T = T0(6) + Trate(6)*(hpos(6)-h);
elseif h <= hpos(7)
    T = T0(7) + Trate(7)*(hpos(7)-h);
else
    T = 5e10; % yikes
end

% Viscosity - definitely is temperature dependent 
mu = 1.846e-5; % kg/(m*s)

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
rho_sl = 1.225;
rho = rho_sl.*exp(-(g/(R_air.*T)).*h);

% Mach number
a  = sqrt(k.*R_air.*T); % m/s, local speed of sound

end