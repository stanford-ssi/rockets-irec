% Drag Fin Analysis
% Ian Gomez,  08/15/16
% Joan Creus, 08/16/16
% This script runs a simulation of the rocket in flight with the goal of
% simulating the flight profile + drag fins

% Implementation of drag fins
% Look at the potential energy (PE = m*g*h) on the control flight
% We want to reduce PE to PE_desired = m*g*h_desired
% PE-PE_desired = D_df * t_open
% where D_df is the drag of the drag fins and t_open is time the drag fins
% are deployed
% You can choose the deployment time of the drag fins and the percentage of
% extra drag they cause the rocket

clear; close all; clc

% -------------------------------------------------------------------------
% Inputs
% -------------------------------------------------------------------------

% Plots
linesize = 2;                % line width on plots
plot_landing       = 0;      % 1 plots time up to landing, 0 plots to apogee
plot_thrust        = 0;      % plot the thrust curve
plot_h_u_a         = 0;      % plot h, u, and a separately
plot_combined_hu   = 0;      % plot h & u together
plot_h             = 1;      % plot just h
plot_forces        = 1;      % doesn't plot the parachute or drogue drag
plot_recovery_drag = 0;      % plots the parachute and drogue drag

% IREC Rocket Baseline is current rocket
rocket.l   = 3.44;           % length, m
rocket.d   = 0.14;           % diameter, m
rocket.Cd  = 0.6;            % coeff of drag
rocket.nomotormass  = 19.43; % kg
rocket.deploydrogue = 1;     % 1 will deploy drogue, 0 will not
rocket.deployparachute = 1;  % 1 will deploy parachute, 0 will not

% Recovery Information
parachute.d        = 6.1;    % m
parachute.deploy_h = 450;    % m
parachute.Cd       = 0.8;    % coefficient of drag
drogue.Cd          = 0.8;    % coefficient of drag
drogue.d           = 1.22;   % m
drogue.deploy_u    = -1;     % m/s

% Motor Selection
motors = {'M1939','M2500'};
motor.name = motors{1};

% Simulation Inputs
time.step = 0.02;            % Choose time step, currently only <0.02 works
time.end  = 400;             % Choose the duration of the simulation
altitude.launch_site = 1219;      % m
altitude.target = 3048;           % m
dragfin.deploy_t = -1;            % s, -1 will not deploy drag fins
dragfin.extra_drag_percent = 1.2; % *100%
rocket.launch_angle = 0.*pi./180; % rad
g = 9.81;

% -------------------------------------------------------------------------
% Simulation
% -------------------------------------------------------------------------

[h,u,a,time,t,t_powered,mach1,forces,rocket,gravityloss,T,dragloss,...
    parachutedrag,droguedrag] = drag_fin_sim(rocket,motor,parachute,...
    drogue,altitude,dragfin,time);

% -------------------------------------------------------------------------
% Simulation Plots
% -------------------------------------------------------------------------

% If an error occurs at t_land, you need to let the sim run longer
time.xlim = time.apogee;
if plot_landing == 1; time.xlim = time.land; end
xlimit = [0 time.xlim];  % plots up to the specified limits

% Makes apogee label
apogee_label_dim = [.4 .3 .6 .1];
apogee_label_str = strcat(strcat(strcat({'Apogee = '},num2str(max(h)))),'m');

% Plot the thrust data
if plot_thrust == 1
    figure
    plot(t_powered,T,'-mo','LineWidth',linesize)
    xlim([0 t_powered(end)])
    thrust_title = strcat({'Thrust Curve of '},motor.name);
    title(thrust_title)
    xlabel('Time (s)')
    ylabel('Thrust (N)')
    grid on
end

if plot_h_u_a == 1
    figure
    hold on
    plot(t,altitude.target.*ones(1,length(t)),'--','LineWidth',linesize)
    plot(t,h,'LineWidth',linesize)
    title(strcat(strcat({'Altitude ('},motor.name),')'))
    xlabel('Time (s)')
    ylabel('Height (m)')
    xlim(xlimit)
    grid on
    legend('3048m above launch site','Location','Southwest')
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
    plot(t,h,t,altitude.target.*ones(1,length(t)),'LineWidth',linesize)
    title(strcat(strcat({'Altitude and Velocity ('},motor.name),')'))
    xlabel('Time (s)')
    ylabel('Height (m)')
    xlim(xlimit)
    grid on
    yyaxis left
    plot(t,u,t(1:length(mach1)),mach1,'LineWidth',linesize)
    ylabel('Velocity (m/s)')
end

if plot_h == 1
    
    figure
    hold on
    y1=get(gca,'ylim');
    plot(t,altitude.target.*ones(1,length(t)),'--','LineWidth',linesize)
    plot(t,h,'LineWidth',linesize)
    plot([t_powered(end) t_powered(end)],y1)
    title(strcat(strcat({'Altitude ('},motor.name),')'))
    xlabel('Time (s)')
    ylabel('Height (m)')
    xlim(xlimit)
    
    grid on
    legend('3048m above launch site','Location','Southwest')
    % label apogee
    annotation('textbox',apogee_label_dim,'String',...
        apogee_label_str,'FitBoxToText','on');
end

if plot_forces == 1
    figure
    plot(t,gravityloss,t_powered,T,t,dragloss,'LineWidth',linesize);
    legend('Gravity','Thrust','Drag + Drag Fins')
    hold on
    if dragfin.deploy_t > -1
        yl = get(gca,'YLim');
        plot([dragfin.deploy_t dragfin.deploy_t], yl,'black')
        txt = text(dragfin.deploy_t+0.1, yl(2)*0.9, 'Drag fin deployment');
        txt.Color = 'black';
    end
    title('In-Flight Forces')
    xlabel('Time (s)')
    ylabel('Force (N)')
    xlim(xlimit)
    grid on
    hold off;
end

if plot_recovery_drag == 1
    figure
    hold on
    plot(t,parachutedrag,'LineWidth',linesize)
    plot(t,droguedrag,'LineWidth',linesize);
    title('In-Flight Forces')
    xlabel('Time (s)')
    ylabel('Force (N)')
    xlim(xlimit)
    grid on
    legend('Parachute','Drogue')
end

% -------------------------------------------------------------------------
% Drag fin energy characteristics
% -------------------------------------------------------------------------

% Energy calculations [J]
g = 9.81;
e_net = rocket.drymass.*g(1).*rocket.apogee;
e_want = rocket.drymass.*g(end).*altitude.target;
e_loss = e_net - e_want;
e_loss_perc = (e_net - e_want)/e_want;
disp('Additional percentage of energy need to lose to drag')
disp(strcat(num2str(e_loss_perc.*100),'%'))

% Find index of distance to altitude target from altitude at fin deployment
% Setting t_fins_deployed below 0 will effectively stop them from deploying
if dragfin.deploy_t > 0
    disp('Drag fins were deployed')
    
    tol = time.step; % this allows you to put in precise times for t_deploy
    for i = 1:length(t)
        if abs(t(i)-dragfin.deploy_t) < tol
            dragfin.deploy_index = i;
        end
    end
    dragfin.deploy_u = u(dragfin.deploy_index);
    dragfin.deploy_h = h(dragfin.deploy_index);
    d2at =  altitude.target - dragfin.deploy_h; % m
    D_df = e_loss./d2at;                          % N
    
else
    disp('Drag fins were not deployed')
    % If we deploy fins at 3113m
    d2at = altitude.target - 3113; % m
    D_df = e_loss./d2at;
end

disp('Additional drag needed to hit target')
disp(strcat(num2str(D_df),'N'))

