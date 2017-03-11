% Raven Data Plotter
% Ian Gomez, 03/04/2017
% Export Raven data as a csv and save it as a normal excel file
clc

% Pull data out
% motor = 'H242';
motor = 'J1799';
filename = strcat(strcat('subscale_', motor), '.xlsx');
data = xlsread(filename);
t_axialA = data(:,1);
axialAGs = data(:,2);
t_Amp = data(:,7);
Amp = data(:,8);
t_T = data(:,13); 
T = data(:,14);
t_v = data(:,16);
v = data(:,17);
t_altAccel = data(:,34);
alt_Accel  = data(:,35);
t_altBaro  = data(:,37);
alt_Baro   = data(:,38);

% Speed of sound calculation
gamma = 1.4;
R = 1716; % ft*lb/(slug*Rankine)
T_R = 459.67 + T; % Rankine
local_c = sqrt(gamma*R*T_R); % local speed of sound

%% Plotting Code

figure(1)
plot(t_axialA, axialAGs)
title(strcat('Axial Acceleration - ', motor))
xlabel('Time (s)'); ylabel('Acceleration (G''s)')
saveas(gcf, strcat(strcat('axial_a', motor), '.png'))

figure(2)
plot(t_v, v, 'r', t_T, local_c)
title(strcat('Velocity - ', motor)); 
legend('Velocity profile', 'Local speed of sound')
xlabel('Time (s)'); ylabel('Velocity (ft/s)')
xlim([0, 25]); ylim([0, max(v)*1.1])
saveas(gcf, strcat(strcat('velocity', motor), '.png'))

figure(3)
yyaxis left
plot(t_altBaro, alt_Baro, t_altAccel, alt_Accel, 'm')
title(strcat('Altitude and Velocity - ', motor))
xlabel('Time (s)'); ylabel('Altitude (ft)')
legend('Barometer', 'Accelerometer','Location','southwest')
yyaxis right
plot(t_v, v./local_c, t_v, local_c./local_c)
ylabel('Local Mach')
saveas(gcf, strcat(strcat('alt_v', motor), '.png'))
