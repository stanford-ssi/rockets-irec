
% Pull raven data out
% filename = 'subscale_H242.xlsx';
filename = 'subscale_J1799.xlsx'; 
%filename = 'launch2_K1620.xlsx';


data = xlsread(filename);
% t_axialA = data(:,1);
% axialAGs = data(:,2);
% t_Amp = data(:,7);
% Amp = data(:,8);
% t_T = data(:,13); 
% T = data(:,14);
% t_v = data(:,16);
% v = data(:,17);
% t_altAccel = data(:,34);
% alt_Accel  = data(:,35);
t_altBaro  = data(:,1);
alt_Baro   = data(:,26);

%OpenRocket data extraction
% OR_filename = 'launch2_OR_K1620.csv';
% OR_data = xlsread(OR_filename);
% OR_t = OR_data(:,1);
% OR_alt = OR_data(:,2);
% OR_v = OR_data(:,5);
% OR_Ma = OR_data(:,27);

% % RASAero (remove stage column)
RA_filename = 'j1799n.csv';
RA_data = csvread(RA_filename);
RA_t = RA_data(:,1);
%OR_Ma = RA_data(:,11);
RA_alt = RA_data(:,22);


% Speed of sound calculation
% gamma = 1.4;
% R = 1716; % ft*lb/(slug*Rankine)
% T_R = 459.67 + T; % Rankine
% local_c = sqrt(gamma*R*T_R); % local speed of sound

%%
% figure(2)
% plot(t_v, v, 'r', t_T, local_c)
% title(strcat('Velocity - ', motor)); 
% legend('Velocity profile', 'Local speed of sound')
% xlabel('Time (s)'); ylabel('Velocity (ft/s)')
% xlim([0, 25]); ylim([0, max(v)*1.1])
% saveas(gcf, strcat(strcat('velocity', motor), '.png'))

%figure(3)
% yyaxis left
plot(t_altBaro, alt_Baro, RA_t, RA_alt, 'm');
title('Altitude - J1799N');
legend('Raven Baro','RasAero'); 
xlabel('Time (s)'); ylabel('Altitude (ft)')
% yyaxis right
% plot(t_v, v./local_c, OR_t, OR_Ma, t_v, local_c./local_c)
% legend('Raven Baro','OR','RASAero','Raven','OR','RASAero')
% % legend('Raven data','OR')
% ylabel('Local Mach')
% xlim([0, 20])
% saveas(gcf, strcat(strcat('alt_v', motor), '.png'))
