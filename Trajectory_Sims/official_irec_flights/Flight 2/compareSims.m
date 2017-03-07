%%% Compare Sims
% Ian Gomez, 03/07/17

% Pull raven data out
motor = 'H242';
% motor = 'J1799';
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

% OpenRocket data extraction
OR_filename = 'OR_subscaleH242.csv';
OR_data = csvread(OR_filename, 1, 0);
OR_t = OR_data(:,1);
OR_alt = OR_data(:,2);
OR_v = OR_data(:,5);
OR_Ma = OR_data(:,27);

% RASAero (remove stage column)
RA_filename = 'RA_subscaleH242.csv';
RA_data = csvread(RA_filename, 1, 0);
RA_t = RA_data(:,1);
RA_Ma = RA_data(:,3);
RA_alt = RA_data(:,19);


% Speed of sound calculation
gamma = 1.4;
R = 1716; % ft*lb/(slug*Rankine)
T_R = 459.67 + T; % Rankine
local_c = sqrt(gamma*R*T_R); % local speed of sound

%%
% figure(2)
% plot(t_v, v, 'r', t_T, local_c)
% title(strcat('Velocity - ', motor)); 
% legend('Velocity profile', 'Local speed of sound')
% xlabel('Time (s)'); ylabel('Velocity (ft/s)')
% xlim([0, 25]); ylim([0, max(v)*1.1])
% saveas(gcf, strcat(strcat('velocity', motor), '.png'))

figure(3)
yyaxis left
plot(t_altBaro, alt_Baro, OR_t, OR_alt, RA_t, RA_alt)
title(strcat('Altitude and Velocity - ', motor))
xlabel('Time (s)'); ylabel('Altitude (ft)')
yyaxis right
plot(t_v, v./local_c, OR_t, OR_Ma, RA_t, RA_Ma, t_v, local_c./local_c)
legend('Raven Baro','OR','RASAero','Raven','OR','RASAero')
ylabel('Local Mach')
xlim([0, 20])
saveas(gcf, strcat(strcat('alt_v', motor), '.png'))
