function [maxq] = max_q(RAD, metric)
% Max Q from RASAero
% Ian Gomez, 10/24
% This will output maxQ in either metric (Pa) or english units (psi)
% One way to make this better is to pass in a height vector instead of
% having matlab parse the file here (ie do it in master script)

% Calculation based on RASAero Data
% Done in metric then converted at the end

ft2m = 0.3048; % m/ft
m2ft = 1/ft2m; % ft/m
pa2psi = 0.000145038; % psi/Pa

[~,~,rho,~] = atmo_conditions(RAD.h); % kg/m^3

maxq = 0.5.*rho.*RAD.v.^2; % Pa

if metric == 0
    maxq = maxq.*pa2psi; % for imperial conversion is here
end

figure(1)
set(gcf,'color','w');
pointsize = 20;
scaling = 1e3; % Pa -> kPa, ft to kft
if metric == 1
    scatter(RAD.h./scaling,maxq./scaling,pointsize,RAD.Ma)
    ylabel('Dynamic Pressure (kPa)')
    xlabel('Height AGL (km)')
else
    scatter(RAD.h.*m2ft./scaling,maxq,pointsize,RAD.Ma)  % converts to EE
    ylabel('Dynamic Pressure (psi)')
    xlabel('Height AGL (kft)')
end
title('Q over flight')
legend('Higher Ma = yellower')
grid on

% send only max q
maxq = max(maxq);

end