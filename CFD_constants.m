% CFD_constants
% Ian Gomez, 07/11/16
% This script calculates useful CFD constants for the End Station X and End
% Station Y RANS simulations. 

clearvars; close all; clc

%% Reynolds Number

rho_air = 1.225; % kg/m^3
u = 10; % m/s
lchar = 23; % m
mu_air = 1.846; % kg/m*s @ 300K
Re = rho_air*u*lchar/mu_air;

%% Intensity 
% High-turbulence case: High-speed flow inside complex geometries (5-20%)
% Medium-turbulence case: Flow in not-so-complex devices and low Re (1-5%)
% Low-turbulence case: Flow originating from a fluid that stands still and
% over aerodynamic objects 
I  = 0.1;   
lt = lchar; %m

%% Modified turbulent viscosity
nu_wiggle = sqrt(3/2)*(u*I*lt);

%% k-epsilon & k-omega models
% We can estimate the inlet parameters using turbulence intensity, I, and 
% and turbulence length scale, lt. The Fluent length scale is based on
% mixing length, and therefore has a different formula than Solidworks. 

% k is equivalent to turbulent energy, epsilon is the turbulent
% dissipation rate and w is the specific turbulent dissipation rate
% Cu const is from http://www.cfd-online.com/Wiki/Turbulence_free-stream_boundary_conditions

Cu = 0.09; 
k = (3/2)*(u*I)^2; 
ep_Fluent = Cu^(3/4)  * k^(3/2) / lt;
w_Fluent  = Cu^(-1/4) * sqrt(k) / lt;

% ep_SW = Cu*k^(3/2)/lt;
% w_SW = sqrt(k) /  lt;

%% Results

disp('Re, nu_wiggle') 
disp([Re nu_wiggle]) 
disp('k, epsilon, omega')
disp([k ep_Fluent w_Fluent])

