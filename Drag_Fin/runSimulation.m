% Drag Fin Sim
% This script runs a simulation of the rocket in flight with the goal of
% simulating the flight profile + drag fins
% Ian Gomez,  08/15/16
% Joan Creus, 08/16/16

% Primary Method:
% 1. Load the thrust data from ThrustCurve.org
% 2. Interpolate the data for more steps
% 3. Calculate change in mass from burnt prop
% 4. Calculate velocity from the combined impulse due to drag, gravity and thrust
% 5. Calculate altitude and acceleration from velocity
% https://spacegrant.carthage.edu/ojs/index.php/wsc/article/viewFile/23/23

function [h,u,a,time,t,t_powered,mach1,rocket,gravityloss,T,dragloss,...
    parachutedrag,droguedrag,e,dragfin] = runSimulation(rocket,motor,...
    parachute,drogue,altitude,dragfin,time,g)

lam = cos(rocket.launch_angle.*pi./180); % launch angle multiplier for drag terms
drogue.deployed    = rocket.deploydrogue;
parachute.deployed = rocket.deployparachute;
parachute.deploy_h = altitude.launch_site + parachute.deploy_h;
parachute.S        = pi.*(parachute.d./2)^2; % m^2
drogue.S   = pi.*(drogue.d./2)^2;            % m^2
rocket.S   = pi.*(rocket.d./2).^2;           % m^2
parachute.S_normal = lam.*parachute.S;
drogue.S_normal    = lam.*drogue.S;
rocket.S_normal    = lam.*rocket.S;

[motor,rocket,T,t_powered] = getMotorData(motor,rocket,time);

t = 0:time.step:time.end;                    % s

% Initialize gravity and mass for flight
m = rocket.drymass.*ones(1,length(t));       % kg
g = g.*ones(1,length(t));                    % m/s^2
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
    k = 0.5.*rocket.Cd.*rocket.S_normal.*rho;
    if (t(i)>dragfin.deploy_t && dragfin.deploy_t > 0 && u(i) > 0)
        k = dragfin.extra_drag_percent*k;
    end
    dragloss(i)        = k.*u(i).^2;
    
    % Gravity loss and current momentum calculation
    % need to not allow gravity to move the rocket below starting altitude
    % if thrust doesn't take off right away
    gravityloss(i)     = m(i).*g(i);
    currentmomentum(i) = u(i).*m(i);
    
    % Recovery drag calculation
    % First if statement checks if the rocket is past the drogue deployment
    % speed and above parachute height
    % Second if statemnet checks if the rocket is below the parachute
    % deployment height
    if u(i) < drogue.deploy_u && h(i) > parachute.deploy_h && rocket.deploydrogue == 1
        droguedrag(i)    = 0.5.*drogue.Cd.*drogue.S_normal.*rho.*u(i).^2;
    elseif h(i) < parachute.deploy_h && u(i) < 0 && t(i) > 1 && rocket.deployparachute == 1
        parachutedrag(i) = 0.5.*parachute.Cd.*parachute.S_normal.*rho.*u(i).^2;
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
    if t(i) > 1 && h(i) >= (0 + altitude.launch_site)
        time.land = t(i);
    end
    if t(i) > 1 && u(i) > 0
        time.apogee = t(i);
        time.apogee_index = i;
    end
   
end

% Reset altitude for plotting. Air density already taken into account
h = h-altitude.launch_site;

% Store useful information
rocket.flight_time = time.land;
rocket.burnout_h = h(length(t_powered));
rocket.apogee = max(h);

% Energy calculations [J]
e.net = rocket.drymass.*g(1).*rocket.apogee;
e.want = rocket.drymass.*g(end).*altitude.target;
e.loss = e.net - e.want;
e.loss_perc = (e.net - e.want)/e.want;

% Find index of distance to altitude target from altitude at fin deployment
% Walk through all the steps in the time vector and find the index where
% the drag fins deploy. Drag fins deploy at time, t, and finding the exact
% index of t allows us to find other important values at that time step
if dragfin.deploy_t > 0
    tol = time.step; % this allows you to put in precise times for t_deploy
    for i = 1:length(t)
        if abs(t(i)-dragfin.deploy_t) < tol
            dragfin.deploy_index = i; % once t(index) = dragfin deploy time
        end
    end
    dragfin.deploy_u = u(dragfin.deploy_index); % finds dragfin deploy u
    dragfin.deploy_h = h(dragfin.deploy_index); % finds dragfin deploy h
    dragfin.dist_to_apogee =  altitude.target - dragfin.deploy_h; % m
    dragfin.extra_D_req = e.loss./dragfin.dist_to_apogee;         % N
    disp(strcat(strcat('Drag fins were deployed at ',...
        num2str(dragfin.deploy_t),'s')))
else
    % If we deploy fins at 3113m
    default_deploy_h = 3113; % m
    dragfin.dist_to_apogee = altitude.target - default_deploy_h; % m
    dragfin.extra_D_req = e.loss./dragfin.dist_to_apogee;
    disp(strcat(strcat(...
        'Drag fins were not deployed, but if we did deploy at',...
        num2str(default_deploy_h),'m')))
end

end