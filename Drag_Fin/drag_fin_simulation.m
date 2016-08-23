% Drag Fin Analysis
% Ian Gomez,  08/15/16
% Joan Creus, 08/16/16
% This script runs a simulation of the rocket in flight with the goal of
% simulating the flight profile + drag fins

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
% You can choose the deployment time of the drag fins and the percentage of
% extra drag they cause the rocket

%function [dragfin] = drag_fin_sim(plot,linesize,rocket,parachute,drogue,...
%   motor,altitude,dragfin,time.step)

clear; close all; clc

% -------------------------------------------------------------------------
% Inputs
% -------------------------------------------------------------------------

% Plots
linesize = 2;                % line width on plots
plot_landing       = 0;      % 1 plots time up to landing, 0 plots to apogee
plot_thrust        = 0;      % plot the thrust curve
plot_h_u_a         = 0;      % plot h, u, and a separately
plot_combined_hu   = 1;      % plot h & u together
plot_h             = 1;      % plot just h
plot_forces        = 1;      % doesn't plot the parachute or drogue drag
plot_recovery_drag = 0;      % plots the parachute and drogue drag

% IREC Rocket Baseline is current rocket
rocket.l   = 3.44;           % length, m
rocket.d   = 0.14;           % diameter, m
rocket.Cd  = 0.6;            % coeff of drag
rocket.nomotormass  = 19.43; % kg
rocket.deploydrogue = 0;     % 1 will deploy drogue, 0 will not
rocket.deployparachute = 0;  % 1 will deploy parachute, 0 will not

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

% -------------------------------------------------------------------------
% Simulation
% -------------------------------------------------------------------------

lam = cos(rocket.launch_angle); % launch angle multiplier for drag terms
drogue.deployed    = rocket.deploydrogue;
parachute.deployed = rocket.deployparachute;
parachute.S        = pi.*(parachute.d./2)^2; % m^2
parachute.deploy_h = altitude.launch_site + parachute.deploy_h;
drogue.S   = pi.*(drogue.d./2)^2;            % m^2
rocket.S   = pi.*(rocket.d./2).^2;           % m^2

[motor,rocket,T,t_powered] = getMotorData(motor,rocket,time);

t = 0:time.step:time.end;                       % s

% Initialize gravity and mass for flight
m = rocket.drymass.*ones(1,length(t));       % kg
g = 9.81.*ones(1,length(t));                 % m/s^2
% Vector initialization for speed
currentmomentum = zeros(1,length(t));
F = zeros(1,length(t)); u = zeros(1,length(t)); dragloss = zeros(1,length(t));
a = zeros(1,length(t)); h = zeros(1,length(t)); gravityloss = zeros(1,length(t));
parachutedrag = zeros(1,length(t)); droguedrag = zeros(1,length(t));
mach1 = zeros(1,length(t));
% Initial conditions
h(1) = altitude.launch_site; % m,   rocket is not at sea level
u(1) = 0;                    % m/s, rocket is stationary
m(1) = rocket.wetmass;       % kg,  rocket is full of prop

% Flight simulation
for i = 1:length(t)
    
    % if motor is on, lose mass
    if t(i) <= motor.burntime
        m(i+1) = m(i)-motor.mdotavg.*time.step;
    end
    
    % Drag calculation
    % The launch angle is taken into account here as well as the drag
    % multiplier for the drag fins
    [~,~,rho,mach1(i)] = getAtmoConditions(h(i));
    k = 0.5.*rocket.Cd.*rocket.S.*rho;
    if (t(i)>dragfin.deploy_t && dragfin.deploy_t > 0 && u(i) > 0)
        k = dragfin.extra_drag_percent*k;
    end
    dragloss(i)        = lam.*k.*u(i).^2;
    
    % Gravity loss and current momentum calculation
    gravityloss(i)     = m(i).*g(i);
    currentmomentum(i) = u(i).*m(i);
    
    % Recovery drag calculation
    % First if statement checks if the rocket is past the drogue deployment
    % speed and above parachute height
    % Second if statemnet checks if the rocket is below the parachute
    % deployment height
    if u(i) < drogue.deploy_u && h(i) > parachute.deploy_h && rocket.deploydrogue == 1
        droguedrag(i)    = lam.*0.5.*drogue.Cd.*drogue.S.*rho.*u(i).^2;
    elseif h(i) < parachute.deploy_h && u(i) < 0 && t(i) > 1 && rocket.deployparachute == 1
        parachutedrag(i) = lam.*0.5.*parachute.Cd.*parachute.S.*rho.*u(i).^2;
    end
    
    % Solve out forces on rocket
    if t(i) <= motor.burntime          % powered ascent
        F(i) = - gravityloss(i) - dragloss(i) + lam.*T(i);
    elseif u(i) > 0                    % coasting to apogee
        F(i) = - gravityloss(i) - dragloss(i);
    elseif 0 >= u(i) > drogue.deploy_u % descent
        F(i) = - gravityloss(i) + dragloss(i) + droguedrag(i)...
            + parachutedrag(i);
    end
    
    % Solve for new velocity and acceleration
    if(i < length(t)) % checks to not exceed index
        u(i+1)  = (1./m(i+1)).*(currentmomentum(i) + ((F(i)).*time.step));
        a(i)    = F(i)./m(i);
        h(i+1)  = trapz([0 time.step],[u(i) u(i+1)])+h(i);
    end
    
    % Store time at apogee and time at landing
    % u(i) < 0 is apogee; h(i) < 0 is landing
    if t(i) > 5 && h(i) >= (0 + altitude.launch_site)
        time.land = t(i);
    end
    if t(i) > 5 && u(i) > 0
        time.apogee = t(i);
    end
end

% Reset altitude for plotting. Air density already taken into account
h = h-altitude.launch_site;

% Store useful information
rocket.flight_time = time.land;
rocket.burnout_h = h(length(t_powered));
rocket.apogee = max(h);

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
    plot(t_thrust,thrust_curve,'-mo','LineWidth',linesize)
    xlim([0 t_thrust(end)])
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

