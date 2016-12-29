% CO2 Simulation
% Ian Gomez, 12/10/16
clc; clear; 

% Initialize the air/ambient properties
g0 = 9.81;                % m/s^2
RAir = 287;               % J/kg*K
RCO2 = 189;               % J/kg*K
recbay.air.P = 100e3;     % Pa
avbay.P = 90e3;           % Pa
cannister.CO2.m = 0:0.001:0.015;  % kg

% Initialize recovery bay properties
% Is the compartment pressurized?
% Is the AV open to ambient
recbay.T  = 293;                % K
recbay.A  = pi.*0.0508.^2;      % m^2, 4in bulkhead
recbay.L  = 0.3048;             % m
recbay.V  = recbay.A.*recbay.L; % m^3
massAbove = 18;                 % kg

% Friction modeling
muS       = 1;                  % static friction constant
normal    = 15;                 % N

% Assumptions: instantaneous mixing, total internal pressure = sum of 
% partial pressures, fixed end (the motor side) and piston (bulkhead), 
% CO2 sits at ambient recovery temperature
recbay.P = (cannister.CO2.m.*RCO2.*recbay.T./recbay.V) + recbay.air.P;

% Sum of forces on bulkhead (acting on center of mass)
% Positive forces are upward (assume rocket is pointed tip down)
Ff = muS.*normal;       % N, friction
Fg = g0.*massAbove;     % N, gravity
Fsp = 284;              % N, shear pin yield
Fpd = (recbay.P-avbay.P).*recbay.A; % N, force of AV bay and internal pressure diff

F = recbay.P.*recbay.A - Ff - Fg - Fsp + Fpd;
gforce = F./(massAbove.*g0);

figure(1)
plot(cannister.CO2.m.*1e3,gforce)
title('Acceleration of recovery deploy')
xlabel('CO2 mass (g)')
ylabel('G''s')
   
% ---
    
% Method 2: energy solution based on distance needed to push the cylinder 
% (time dependent, adiabatic, needs good friction modeling)

% ------------------------------------------------------------------------

% Sum of forces will change after initial moment due to volume change. 
% Could be useful to model.

