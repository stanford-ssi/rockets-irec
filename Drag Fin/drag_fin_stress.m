% Drag Fin Analysis
% Ian Gomez,  08/17/16
% This script attempts to do a simplified analysis of a the drag fins in
% order to characterize the order of magnitudes of stress 

clc; close all;

g = 9.81; % m/s^2
avogadro = 6.0221409e23;
k_boltz = 1.38064852e-23;   % J/K
atmos.T   = 300;            % K 
atmos.P  = 101300;          % Pa, sea level pressure 
m_air   = 29/avogadro*1e-3; % kg
P_inflight = @(h) P_sl*exp((-m_air*g*h)./(k_boltz*T_atm)); % Pa

Ma = 0.81;

P0 = P_inflight(dragfin.deploy_h)*(1+((k-1)/2).*Ma.^2).^(k/(k-1))