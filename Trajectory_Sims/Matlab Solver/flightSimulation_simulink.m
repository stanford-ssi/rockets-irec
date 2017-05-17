%%% Flight Simulation
% Ian Gomez, 03/5/17
clear all; close all; clc;

% ballasts = 6:2:12; % lbs
disp('Loading in initial conditions...')
run('initialconditions.m')
disp('Loading simulink model...')
model = 'solver_simulink'; load_system(model);

% for i = 1:length(ballasts)
disp('%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%')

%% Setup and run sim
wind = 0; % m/s
launch_angle = 7;
rINT = [0, site_elevation, launch_angle]; % m, position,         r = [x,  y,  theta]
%     ballast = ballasts(i);
ballast = 0;

% make the rocket
rocket = makeRocket(ballast); % returns rocket struct
mdot = (rocket.wetmass-rocket.drymass)./motor.burntime;
mass = [(0:t_step:motor.burntime)',... % make thrust a paired vector
    (rocket.wetmass:-mdot*t_step:rocket.drymass)'];
CM = 73.673*in2m; % inches from nose

% set up sim length
ind = 2; times = {'60', '80' '120', '480'};
stoptime = times{ind};
maxstep = 0.1;

% run the sim
save('import2simulink','rocket');
set_param(model, 'StopTime', stoptime);
set_param(model, 'MaxStep', num2str(maxstep))
%     disp(['Beginning simulation ', num2str(i), ' of ', num2str(length(ballasts))])
tic; simdata = sim(model); toc
[msg, id] = lastwarn; warning('off', id) % eliminates error message

% close the scope
shh = get(0,'ShowHiddenHandles'); set(0,'ShowHiddenHandles','On');
hscope = findobj(0,'Type','Figure','Tag','SIMULINK_SIMSCOPE_FIGURE');
close(hscope); set(0,'ShowHiddenHandles',shh);

%%% Pull out data from simulink
disp('Pulling data from simulink...')
tout = simdata.get('tout');   stateout = simdata.get('stateout');
rx   = stateout(:,1);  ry   = stateout(:,2);  theta  = stateout(:,3);
ux   = stateout(:,4);  uy   = stateout(:,5);  omega  = stateout(:,6);
forceout_data = simdata.get('forceout'); forceout = squeeze(forceout_data);
fx   = forceout(1,:);  fy   = forceout(2,:);  moment = forceout(3,:);
drag = forceout(4,:);  lift = forceout(5,:);  CD     = forceout(6,:);
CL   = forceout(7,:);  aoa  = forceout(8,:);  weight = forceout(9,:);
CP   = forceout(10,:); 
ry = ry-site_elevation; % reset altitude

% find apogee and time of apogee
flight.apogee = max(ry); % seems wrong
k = 1; tol = 1e-2;
while(1)
    delta = abs(ry(k)-flight.apogee);
    if delta < tol; break; end
    k = k+1;
end
flight.apogee = flight.apogee.*m2ft;
flight.t_apogee = tout(k);
flight.dispersion = max(rx); flight.dispersion_mi = flight.dispersion*m2mi;
flight.relerror_apogee = abs(max(ras_alt)-flight.apogee)/goalkft;
flight.abserror_apogee = abs(max(flight.apogee)-goalkft)/goalkft

% % moment calculation
% for i= 1:length(drag)
%     cal = CM-CP(i);
%     drag(i).*
% end
% moment = (Lx - Dx)*cosd(theta)*(CP-CM) + (Dy + Ly)*sind(theta)*(CP-CM);

%% Plots
disp('Plotting...')
scaling = 1e3;
xend = flight.t_apogee;
% xend = tout(end);
ballaststring = strcat(strcat(': ',num2str(ballast)),'lbs Ballast ');
thetastring = strcat(strcat(...
    ' & launch angle = ', num2str(launch_angle)),'deg');
titlestring = strcat(ballaststring, thetastring);

% altitude with rasaero
figure(1); set(gcf,'color','w'); hold on % in imperial units
goal_line = goalkft.*ones(1,length(tout));
alt = ry.*m2ft;
p = patch([0, xend, xend, 0],...
    [(goalkft-band)/scaling, (goalkft-band)/scaling,...
    (goalkft+band)/scaling, (goalkft+band)/scaling],'g', 'EdgeColor', 'none'); 
set(p,'FaceAlpha',0.25);
plot(tout, goal_line./scaling,'--')
plot(tout, alt./scaling)
plot(ras_t,ras_alt./scaling)
title(strcat('Altitude',titlestring));
legend('scoring band','goal','simulink','RASAero','Location','Southeast')
xlabel('time (s)'); ylabel('altitude (kft)'); grid on
xlim([0, xend])

% % altitude (mulitple ballast)
% figure(1); set(gcf,'color','w'); hold on % in imperial units
% alt = ry-site_elevation;
% plot(tout, alt.*m2ft./scaling)
% title('Altitude');
% if i == length(ballasts)
%     goal_line = (goal-site_elevation).*m2ft.*ones(1,length(tout));
%     plot(tout, goal_line./scaling,'--')
%     legend(strcat(strsplit(num2str(ballasts)),'lbs'), 'Location','Southeast')
% end
% xlabel('time (s)'); ylabel('altitude (kft)'); grid on
% xlim([0, xend])

% % x velocity
% figure(2); set(gcf,'color','w'); hold on % imperial units
% plot(tout,ux.*m2ft)
% plot(ras_t,ras_vx)
% title('X Velocity'); legend('simulink','RASAero')
% xlabel('time (s)'); ylabel('velocity (ft/s)'); grid on
% xlim([0, xend])
% 
% % y velocity with rasaero
% figure(3); set(gcf,'color','w'); hold on % imperial units
% plot(tout, uy.*m2ft)
% plot(ras_t, ras_vy)
% title('Y Velocity'); legend('simulink','RASAero')
% xlabel('time (s)'); ylabel('velocity (ft/s)'); grid on
% xlim([0, xend])

% drag with rasaero
figure(6); set(gcf,'color','w'); hold on
yyaxis left
plot(tout, drag, ras_t, ras_D)
xlabel('time (s)'); ylabel('drag (N)'); grid on
yyaxis right
plot(tout, CD, ras_t, ras_CD)
title('Drag'); legend('drag','RASAero drag','CD','RASAero CD')
xlim([0, flight.t_apogee])

% % theta with RASAero
% figure(8); set(gcf,'color','w'); hold on
% plot(tout, theta, ras_t, 90-ras_theta); 
% xlabel('time (s)'); ylabel('theta (degrees)'); grid on
% title('Theta'); legend('simulink','RASAero')
% xlim([0, motor.burntime])
% 
% % Moment
% figure(9); set(gcf,'color','w'); hold on
% plot(tout, moment)
% title('Moment'); xlabel('time (s)'); ylabel('moment (N-m)')
% 
% % x and y
% figure(10); set(gcf,'color','w'); hold on; S = 5;
% scatter(rx,ry,S,tout,'filled')
% plot3(rx,ry,tout)

disp('Done.') 

% score_aslt(i) = flight.apogee;
