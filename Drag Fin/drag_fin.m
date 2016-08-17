% Drag Fin Analysis
% Ian Gomez, 08/15/16
% This script runs a simulation of the rocket in flight with the goal of
% optimizing the size, number, and max angle of the drag fins.

% Primary Method:
% 1. Load the thrust data from ThrustCurve.org
% 2. Interpolate the data for more steps
% 3. Calculate change in mass from burnt prop
% 4. Calculate velocity from the combined impulse due to drag, gravity and thrust
% 5. Calculate altitude and acceleration from velocity
% https://spacegrant.carthage.edu/ojs/index.php/wsc/article/viewFile/23/23

% Implementation of drag fins
% Look at the potential energy (PE = m*g*h) on the control flight
% We want to reduce PE to PE_desired = m*g*h_desired
% PE-PE_desired = D_df * t_open
% where D_df is the drag of the drag fins and t_open is time the drag fins
% are deployed
% Use the drag equation to get an estimate on size of fins
% Cd_df = D_df/(0.5 * rho * u^2 * S)
% Guess a Cd_df so to close the problem to solve for total area, S, required
% of drag fins

clear; close all; clc;

% Plots
linesize = 2;  % line width
plot_thrust = 0;
plot_h_u_a = 0;
plot_combined_hu = 0;
plot_h = 1;
plot_forces = 0;

%% Rocket and motor characteristics

% IREC Rocket Baseline is current rocket
rocket.l   = 3.44; % length, m
rocket.d   = 0.14; % diameter, m
rocket.Cd  = 0.6;  % coeff of drag
rocket.S   = pi.*(rocket.d./2).^2; % front cross sectional area, m^2
rocket.nomotormass = 19.428; % kg

% Specify the motor you want
motor.name = 'M1939';
% Input motor data (always check that these values match chosen motor)
motor.wetmass  = 8.9882; % kg
motor.propmass = 5.7191; % kg

% Load thrust curve
motor_str = strcat('AeroTech_',strcat(motor.name,'.txt'));
fileID = fopen(motor_str);
thrust_curve = textscan(fileID,'%f %f','headerLines',5);
fclose(fileID);

% Converts the cells to a matrix and removes the last line
thrust_curve = cell2mat(thrust_curve);
t_thrust = thrust_curve(1:length(thrust_curve)-1,1);
thrust_curve = thrust_curve(1:length(thrust_curve)-1,2);

% Interpolate the data, original data samples at 0.05s or 20 Hz
% Change time step to add discrete data points to the simulation
time_step = 0.02;
t_powered = 0:time_step:t_thrust(end);
T = interp1(t_thrust,thrust_curve,t_powered);

% Do some calculations for powered ascent properties
motor.drymass  = motor.wetmass-motor.propmass; % kg
motor.burntime = t_powered(end);   % s
motor.mdotavg  = motor.propmass./motor.burntime; % kg/s
rocket.drymass = rocket.nomotormass + motor.drymass; % kg
rocket.wetmass = rocket.nomotormass + motor.wetmass; % kg

% Plot the thrust data
if plot_thrust == 1
    figure
    plot(t_thrust,thrust_curve,'-mo','LineWidth',linesize)
    xlim([0 t_thrust(end)])
    thrust_title = strcat({'Thrust Curve of '},motor.name);
    title(thrust_title)
    xlabel('Time (s)')
    ylabel('Thrust (N)')
    grid on
end

%% Simulation

% In order to change the time_step, go to the interpolation of the thrust
% data. Specify your altitude goal, when you want the fins deployed, and
% how much you want the fins to increase the intrinsic drag of the rocket
t = 0:time_step:60;     % s
altitude_target = 3048; % m
t_fins_deployed = 7.3;    % s
per_normal_drag = 1.544;    % *100%

% Displays your input parameters
disp(strcat(strcat(({'Fins deployed at '}),strcat(num2str(t_fins_deployed),'s')),...
strcat({', creating '},...
    strcat(num2str(per_normal_drag*100),'% more drag than just the rocket'))))

% Atmospheric properties
mach1const = 343; % m/s
density = @(x) 1.2250.*(288.15/(288.15-0.0065.*x)).^...
    (1+9.80665.*0.0289644/(8.31432.*0.0065));

% Flight conditions
m = rocket.drymass.*ones(1,length(t)); % kg
g = 9.81.*ones(1,length(t));           % m/s^2
mach1 = mach1const.*ones(1,length(t)); % m/s

% Vector initialization for speed
currentmomentum = zeros(1,length(t));
F = zeros(1,length(t)); u = zeros(1,length(t)); dragloss = zeros(1,length(t));
a = zeros(1,length(t)); h = zeros(1,length(t)); gravityloss = zeros(1,length(t));

% Initial conditions
h(1) = 0;                              % rocket is at sea level
u(1) = 0;                              % rocket is stationary
m(1) = rocket.wetmass;                 % rocket is full of prop

% Flight simulation
for i = 1:length(t)
    
    if t(i) <= motor.burntime % if motor is on
        m(i+1) = m(i)-motor.mdotavg.*time_step;
    end
    
    % Drag calculation
    rho = density(h(i));
    k = 0.5.*rocket.Cd.*rocket.S.*rho;
    if (t(i)>t_fins_deployed) k = per_normal_drag*k; end % Drag fin multiplier
    dragloss(i) = k.*u(i).^2;
    
    currentmomentum(i) = u(i).*m(i);
    gravityloss(i) = m(i).*g(i);
    
    % Solve out forces on rocket
    if t(i) <= motor.burntime % powered ascent
        F(i) = T(i) - gravityloss(i) - dragloss(i);
    elseif u(i) > 0           % coasting to apogee
        F(i) = -gravityloss(i) - dragloss(i);
    else                      % descent without a parachute
        F(i) = -gravityloss(i) + dragloss(i);
    end
    
    % Solve for new velocity and acceleration
    if(i < length(t)) % checks to not exceed index
        u(i+1) = (1./m(i+1)).*(currentmomentum(i) + ((F(i)).*time_step));
        a(i) = F(i)./m(i);
        h(i+1) = trapz([0 time_step],[u(i) u(i+1)])+h(i);
    end
    
    % silly bullshit to find the apogee or landing time
    % u(i) < 0 is apogee
    % h(i) < 0 is landing
    if t(i) > 10 && u(i) < 0
        t_land(i) = t(i);
        t_land = unique(t_land(:));
        t_land = t_land(2);
    end
end

xlimit = [0 t_land];  % plots up to the specified limits

%% Simulation Plots

apogee_label_dim = [.4 .3 .6 .1];
apogee_label_str = strcat(strcat(strcat({'Apogee = '},num2str(max(h)))),'m');

if plot_h_u_a == 1
    figure
    hold on
    plot(t,altitude_target.*ones(1,length(t)),'--','LineWidth',linesize)
    plot(t,h,'LineWidth',linesize)
    title(strcat(strcat({'Altitude ('},motor.name),')'))
    xlabel('Time (s)')
    ylabel('Height (m)')
    xlim(xlimit)
    grid on
    legend('3048m','Location','Southwest')
    % label apogee
    annotation('textbox',apogee_label_dim,'String',...
        apogee_label_str,'FitBoxToText','on');
    
    figure
    hold on
    plot(t,mach1,'--','LineWidth',linesize)
    plot(t,u,'LineWidth',linesize)
    title(strcat(strcat({'Velocity ('},motor.name),')'))
    xlabel('Time (s)')
    ylabel('Velocity (m/s)')
    xlim(xlimit)
    grid on
    legend('Mach 1','Location','Southwest')
    
    figure
    plot(t(1:length(a)),a./g,'LineWidth',linesize)
    title(strcat(strcat({'Acceleration ('},motor.name),')'))
    xlabel('Time (s)')
    ylabel('g')
    xlim(xlimit)
    grid on
end

if plot_combined_hu == 1
    figure
    yyaxis right 
    plot(t,h,t,altitude_target.*ones(1,length(t)),'LineWidth',linesize)
    title(strcat(strcat({'Altitude and Velocity ('},motor.name),')'))
    xlabel('Time (s)')
    ylabel('Height (m)')
    xlim(xlimit)
    grid on    
    yyaxis left
    plot(t,u,t,mach1,'LineWidth',linesize)
    ylabel('Velocity (m/s)')
end

if plot_h == 1
    figure
    hold on
    plot(t,altitude_target.*ones(1,length(t)),'--','LineWidth',linesize)
    plot(t,h,'LineWidth',linesize)
    title(strcat(strcat({'Altitude ('},motor.name),')'))
    xlabel('Time (s)')
    ylabel('Height (m)')
    xlim(xlimit)
    grid on
    legend('3048m','Location','Southwest')
    % label apogee
    annotation('textbox',apogee_label_dim,'String',...
        apogee_label_str,'FitBoxToText','on');
end

if plot_forces == 1
    figure
    plot(t,gravityloss,t_powered,T,t,dragloss,'LineWidth',linesize);
    title('In-Flight Forces')
    xlabel('Time (s)')
    ylabel('Force (N)')
    xlim(xlimit)
    grid on
    legend('Gravity','Thrust','Drag')
end

%% Drag fin characteristics

rocket.burnout_h = h(length(t_powered));
rocket.apogee = max(h);

% Energy calculations [J]
e_net = rocket.drymass.*g(1).*rocket.apogee;
e_want = rocket.drymass.*g(end).*altitude_target;
e_loss = e_net - e_want;               
e_loss_perc = (e_net - e_want)/e_want; 
disp('Percentage of energy need to lose to drag')
disp(strcat(num2str(e_loss_perc.*100),'%'))

% find index of distance to altitude target from altitude at fin deployment
for i = 1:length(t)
    if t(i) == t_fins_deployed
        i_fins_deployed = i;
    end
end
d2at =  altitude_target - h(i_fins_deployed); % m
D_df = e_loss./d2at;                          % N

disp(strcat(strcat('Amount of drag needed if fins open after ',...
    num2str(t_fins_deployed)),'s'))
disp(strcat(num2str(D_df),'N'))
disp('Altitude Achieved')
disp(strcat(num2str(rocket.apogee),'m'))

% small angle
theta_max_deg = 17.5; % degrees
theta_max_rad = 0.3054326; % rad

% factor of safety for control authority
fs = 1.5;