% Master Structures Script
% Ian Gomez,  10/25/2016
% This script is meant to call the new methods we've implemented to drive
% airframe design (ie flutter, fin ability to generate lift and drag, and
% max Q)
clc; clear; close all;

ft2m = 0.3048;
RASAero_csv = '4in_CF.CSV'; % flight data
data = csvread(RASAero_csv,2,1);
RAD.Ma = data(:,2);
RAD.v  = data(:,13).*ft2m; % m/s
RAD.h  = data(:,18).*ft2m; % m

% Rocket characteristics

bodytube.lengthunit = 'in';
bodytube.OD = 4; %in 
bodytube.OR = bodytube.OD/2;
bodytube.S_ref = pi * bodytube.OR^2;

fin.lengthunit = 'in';
fin.t = (1/16):(1/16):(1/4);                            % thickness
fin.h = 4;                                              % span
fin.rootlength = 8;
fin.tiplength = 2;
fin.sweepdistance = fin.rootlength/2 - fin.tiplength/2; 
fin.S = 0.5.*(fin.rootlength + fin.tiplength).*fin.h;
fin.AR = (fin.sweepdistance.^2)/fin.S;
fin.number = 3;

% Max dynamic pressure
metric = 0; % not in metric
maxq = maxQ(RAD,metric);

% Flutter Velocity based on different fin thickness
% currently set to aluminum
fin = imperial_flutter_NACA(RAD,fin);

% Only useful for subsonic flight
Ma_CNalpha = 0.8;
CN_alpha_total = CNalpha_compute(fin, bodytube, Ma_CNalpha);