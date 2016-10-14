%Volumes - SSI Structures Team

clear all
close all
clc

%Nose cone friction fit volume
dFriction = 1.78275332;
r = dFriction;
areaFriction = r.^2*pi;
hFriction = 4.82519685;

vNoseCone = hFriction*areaFriction

%Payload coupler volume
d = 5.75;
r = d/2;
hPayload = 6.75;
areaPayload = r.^2*pi;
vPayload = hPayload*areaPayload;

%Payload extra volume
h1  = .5;
d1 = 5.25;
r1 = d1/2;
area1 = r1.^2*pi;
v1 = area1*h1*2;

vtotPayload = vPayload+v1

%Aft coupler volume
rAft = 2.84;
hAft = 7.5;
areaAft = rAft.^2*pi;
vAft = areaAft*hAft;

%Aft Extra Section
rCurve = 5.5/2;
h = 2;
vAftExtra = pi*rCurve.^2;

vAftTot = vAft+vAftExtra 