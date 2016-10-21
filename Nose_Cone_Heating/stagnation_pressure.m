% Stagnation Pressure Calculator
% Ian Gomez

clear; clc; close all;

Ma = 0:0.1:2.5;
Patm = 101300; % Pa
k = 1.4; % specific heat ratio
P0 = Patm * (1 + (k - 1)/2 * Ma.^2).^(k./(k-1));
P0 = P0./1e3; % kPa

figure(1)
hold on
set(gcf,'color','w');
plot(Ma,P0,'b')
title('P_0 vs changing Mach numbers')
xlabel('Mach Number'); ylabel('P_0 (kPa)')
plotfixer