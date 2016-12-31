% CO2 Simulation
% Ian Gomez, 12/10/16
clc; clear;

% Simulation inputs
inputCO2 = 0.015; % kg

% Initialize the air/ambient properties
g0 = 9.81;                      % m/s^2
RAir = 287;                     % J/kg*K
RCO2 = 189;                     % J/kg*K
recbay.air.P = 100e3;           % Pa
avbay.P = 90e3;                 % Pa
massCO2 = 0:1e-4:inputCO2;      % kg

% Initialize recovery bay properties
% Change to fit the current recovery bay
recbay.T  = 293;                % K
recbay.A  = pi.*0.0508.^2;      % m^2, 4in bulkhead
recbay.L  = 0.3048;             % m
recbay.couplerL = 0.0762;       % m
recbay.V  = recbay.A.*recbay.L; % m^3
massAbove = 18;                 % kg, mass above motor section

% Friction modeling
% Not understood well tbh. Friction interface between CF and FG
% and cylindrical concentric objects
muS       = normrnd(1,.075);    % static  friction constant
normal    = normrnd(1000,100);  % N

% Assumptions: instantaneous mixing, total internal pressure = sum of
% partial pressures, fixed end (the motor side) and piston (bulkhead),
% CO2 sits at ambient recovery temperature, adiabatic
recbay.P = (massCO2.*RCO2.*recbay.T./recbay.V) + recbay.air.P;

% Sum of forces on bulkhead (acting on center of mass)
% Positive forces are upward (assume rocket is pointed tip down)
Ff = muS.*normal;               % N, friction
Fg = g0.*massAbove;             % N, gravity
Fco2 = recbay.P.*recbay.A;      % N, CO2 force
Fsp = 284;                      % N, shear pin yield
Fpd  = (recbay.P-avbay.P).*recbay.A; % N, force of AV bay and internal pressure diff

F = Fco2 - Ff - Fg - Fsp + Fpd; % Sum of forces

% Recovery bay cannot move backwards so negative forces are zeroed
for i = 1:length(F)
    if F(i) < 0
        F(i) = 0;
    end
end
a = F./massAbove;
gforce = a./g0;
gforcemax = max(gforce)

scaling = 1e-3;
figure(1)
plot(massCO2./scaling,gforce)
title('Acceleration of recovery deploy')
xlabel('CO2 (grams)')
ylabel('G''s')