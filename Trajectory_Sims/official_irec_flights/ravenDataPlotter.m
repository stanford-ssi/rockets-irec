% Raven Data Plotter
% Ian Gomez, 03/04/2017
% Export Raven data as a csv and save it as a normal excel file
clc

% Pull data out
filename = 'subscale_J1799.xlsx';
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
title('Axial Acceleration')
xlabel('Time (s)'); ylabel('Acceleration (G''s)')

figure(2)
plot(t_v, v, 'r', t_T, local_c)
title('Velocity'); legend('Velocity profile', 'Local speed of sound')
xlabel('Time (s)'); ylabel('Velocity (ft/s)')
xlim([0, 25]); ylim([0, max(v)*1.1])

figure(3)
yyaxis left
plot(t_altAccel, alt_Accel, t_altBaro, alt_Baro)
title('Altitude'); legend('Accelerometer data', 'Barometer data', 'Charges')
xlabel('Time (s)'); ylabel('Altitude (ft)')
yyaxis right
plot(t_Amp, Amp)
ylabel('Current Draw (A)')

figure(4)
yyaxis left
plot(t_altBaro, alt_Baro, t_altAccel, alt_Accel)
title('Altitude and Velocity')
xlabel('Time (s)'); ylabel('Altitude (ft)')
legend('Barometer', 'Accelerometer')
yyaxis right
plot(t_v, v./local_c, t_v, local_c./local_c)
ylabel('Local Mach')