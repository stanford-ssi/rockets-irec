function [maxq] = maxQ(RASAero_csv, metric)
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

RAD = csvread(RASAero_csv,2,1); % you will error if the RASAero csv still has stage coloumn
RAD_Ma = RAD(:,02);
RAD_v  = RAD(:,13).*ft2m; % m/s
RAD_h  = RAD(:,18).*ft2m; % m

[~,~,rho,~] = getAtmoConditions(RAD_h); % kg/m^3

maxq = 0.5.*rho.*RAD_v.^2; % Pa

figure
set(gcf,'color','w');
pointsize = 40;
scaling = 1e3; % Pa -> kPa, ft to kft
if metric == 1
    scatter(RAD_h./scaling,maxq./scaling,pointsize,RAD_Ma)
    ylabel('Dynamic Pressure (kPa)')
    xlabel('Height AGL (km)')
else
    maxq = maxq.*pa2psi; % for imperial conversion is here
    scatter(RAD_h.*m2ft./scaling,maxq,pointsize,RAD_Ma)  % converts to EE
    ylabel('Dynamic Pressure (psi)')
    xlabel('Height AGL (kft)')
end
title('Max Q over flight')
legend('Higher Ma = yellower')
grid on

end