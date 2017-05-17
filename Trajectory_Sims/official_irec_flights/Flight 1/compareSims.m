%%% Compare Sims
% Ian Gomez, 03/07/17
clc;

motors = {'H242','J1799'};

for i = 1:length(motors)
    motor = motors{i};
    
    %%% Data extraction
    % You need an xlsx file from the raven data, and csvs from the OR and
    % RASAero simulations.
    
    % Pull raven data out
    
    filename = strcat(strcat('subscale_', motor), '.xlsx');
    data = xlsread(filename);
    t_axialA = data(:,1);      % time for axial acceleration
    axialAGs = data(:,2);      % axial acceleration in gs
    t_amp = data(:,7);         % time for current draw
    amp = data(:,8);           % current draw in amps
    t_T = data(:,13);          % time for local temperature
    T = data(:,14);            % local temperature in deg F
    t_v = data(:,16);          % time for velocity (most likely axial)
    v = data(:,17);            % velocity in ft/s
    t_alt_accel = data(:,34);  % time for accelerometer based altitude
    alt_accel  = data(:,35);   % accelerometer based altitude
    t_alt_baro  = data(:,37);  % time for barometric based altitude
    alt_baro   = data(:,38);   % barometric based altitude
    
    % RASAero (remove stage column)
    RA_filename = strcat(strcat('RA_subscale', motor), '.csv');
    RA_data = csvread(RA_filename, 1, 0);
    RA_t = RA_data(:,1);       % time
    RA_Ma = RA_data(:,3);      % local mach number
    RA_alt = RA_data(:,19);    % altitude (ft)
    
    if strcmp(motor,'H242'); OR_motor = 'H242';
    else OR_motor = 'J1999'; end
    
    % OpenRocket data extraction
    OR_filename = strcat(strcat('OR_subscale', OR_motor), '.csv');
    OR_data = csvread(OR_filename, 1, 0);
    OR_t = OR_data(:,1);       % time
    OR_alt = OR_data(:,2);     % altitude (ft)
    OR_v = OR_data(:,5);       % velocity magnitude (ft/s)
    OR_Ma = OR_data(:,27);     % local Mach number
    
    %%% Calculations
    % Speed of sound calculation
    gamma = 1.4;                    % air specific heats ratio
    R = 1716;                       % ft*lb/(slug*Rankine)
    T_R = 459.67 + T;               % Rankine
    local_c = sqrt(gamma*R*T_R);    % local speed of sound
    Ma = v./local_c;
    
    % Altitude comparisions
    max_alt    = max(alt_baro);
    OR_max_alt = max(OR_alt);
    RA_max_alt = max(RA_alt);
    disp(OR_motor)
    OR_alt_error = abs(max_alt - OR_max_alt)/max_alt
    disp(motor)
    RA_alt_error = abs(max_alt - RA_max_alt)/max_alt
    
    %%% Plots
    % figure(1)
    % plot(t_v, v, 'r', t_T, local_c)
    % title(strcat('Velocity - ', motor));
    % legend('Velocity profile', 'Local speed of sound')
    % xlabel('Time (s)'); ylabel('Velocity (ft/s)')
    % xlim([0, 25]); ylim([0, max(v)*1.1])
    % saveas(gcf, strcat(strcat('velocity', motor), '.png'))
    
    figure(i)
    yyaxis left
    plot(t_alt_baro, alt_baro, OR_t, OR_alt, RA_t, RA_alt)
    title(strcat('Altitude and Velocity - ', motor))
    xlabel('Time (s)'); ylabel('Altitude (ft)')
    yyaxis right
    plot(t_v, Ma, OR_t, OR_Ma, RA_t, RA_Ma, t_v, local_c./local_c)
    legend('Raven Baro','OR','RASAero','Raven','OR','RASAero')
    ylabel('Local Mach')
    xlim([0, 20])
    saveas(gcf, strcat(strcat('alt_v', motor), '.png'))
    
end
