function [ax,ay,ux,uy,x,h,time_land,index_land] =...
    dispersion_fun(rocket,dt,t,windspeed)
% Uses metric values, so we convert from lbs to kg and

% Starting altitude
altitude_launch_site = 1402.08;  % m, =  4,600 ft
h0 = 9144;                       % m, = 30,000 ft

% Puts values in metric
lbs2kg = 0.453592;   % kg/lbs
in22m2 = 0.00064516; % m^2/in^2
in2m   = 0.0254;     % m/in
fts2ms = 0.3048;     % (m*s^-1)/(ft*s^-1)

%% Density profile of the atmosphere (0-13000m)
% Seems unnecessary to run this everytime
g0 = 9.8;
T0 = 288.16;   % sea level temperature (kelvin)
rho0 = 1.225;  % sea level density (kg/m^3)
ay = -0.0065;  % lapse rate (K/m)
R = 287.05;    % gas constant (J/kg*K)
rho = zeros(1,h0);
for h = 1:11000
    T = T0 + ay*h;
    rho(h) = rho0 * (T/T0)^((-g0/(ay*R)) - 1 );
end
T_isot = 217; % isothermal temp (K)
for h = 11001:13000
    rho(h) = rho(11000) * (exp(1))^((-g0/(T_isot*R))* (h-11000));
end

%% Simulation

% Vector initialization for speed
currentmomentumx = zeros(1,length(t));currentmomentumy = zeros(1,length(t));
Fy = zeros(1,length(t)); uy = zeros(1,length(t)); Fx = zeros(1,length(t));
ux = zeros(1,length(t)); ax = zeros(1,length(t)); winddrag = zeros(1,length(t));
ay = zeros(1,length(t)); h = zeros(1,length(t)); gravityloss = zeros(1,length(t));
parachutedrag = zeros(1,length(t)); droguedrag = zeros(1,length(t));
x = zeros(1,length(t));
% Initial conditions
h(1)  = altitude_launch_site+h0; % m,   rocket is not at sea level
uy(1) = 0;                       % m/s, rocket is at apogee
ux(1) = 0;

%%  Flight simulation
for i = 1:length(t)
    
    % Y direction
    %---------------------------------------------------------------------%
    
    % Gravity loss and current momentum calculation
    gravityloss(i)      = rocket.drymass.*lbs2kg.*g0;
    currentmomentumy(i) = uy(i).*rocket.drymass.*lbs2kg;
    
    % Recovery drag calculation
    % First if statement checks if the rocket is past the drogue deployment
    % speed and above parachute height
    % Second if statemnet checks if the rocket is below the parachute
    % deployment height
    if uy(i) < rocket.drogue.deploy_u*fts2ms && h(i) > rocket.main.deploy_h*in2m
        droguedrag(i)    = 0.5.*rocket.drogue.Cd.*rocket.drogue.S.*in22m2.*rho(round(h(i))).*uy(i).^2;
    elseif h(i) < rocket.main.deploy_h*in2m && uy(i) < 0
        if round(h(i)) > 0
            parachutedrag(i) = 0.5.*rocket.main.Cd.*rocket.main.S.*in22m2.*rho(round(h(i))).*uy(i).^2;
        else % rho(0) cannot be accessed
            parachutedrag(i) = 0.5.*rocket.main.Cd.*rocket.main.S.*in22m2.*rho0.*uy(i).^2;
        end
    end
    
    % Solve out forces on rocket
    Fy(i) = - gravityloss(i) + droguedrag(i) + parachutedrag(i);
    
    % X Direction
    %---------------------------------------------------------------------%
    
    % Current currrent momentum
    currentmomentumx(i) = ux(i).*rocket.drymass.*lbs2kg;
    
    % Drag/Lift in the x direction from the wind
    % Modeled as drag on the body of the rocket and parachute
    k_nosecone  = rocket.nosecone.Cd.*rocket.nosecone.S_x.*in22m2;
    k_bodytube  = rocket.bodytube.Cd.*rocket.bodytube.S_x.*in22m2;
    k_engine    = rocket.engine.Cd  .*rocket.engine.S_x.*in22m2;
    k           = k_nosecone + k_bodytube + k_engine;
    if round(h(i)) > 0
        winddrag(i) = 0.5.*k.*rho(round(h(i))).*windspeed.^2;
    else
        winddrag(i) = 0.5.*k.*rho0.*windspeed.^2;
    end
    
    % Solve for new velocity and acceleration
    Fx(i) = winddrag(i);
    
    % Solve for trajectory
    %---------------------------------------------------------------------%
    
    % Solve for new velocity and acceleration
    if(i < length(t)) % checks to not exceed index
        uy(i+1)  = (1./(rocket.drymass.*lbs2kg)).*(currentmomentumy(i) + ((Fy(i)).*dt));
        ux(i+1)  = (1./(rocket.drymass.*lbs2kg)).*(currentmomentumx(i) + ((Fx(i)).*dt));
        ay(i)    = Fy(i)./(rocket.drymass.*lbs2kg);
        ax(i)    = Fx(i)./(rocket.drymass.*lbs2kg);
        h(i+1)   = trapz([0 dt],[uy(i) uy(i+1)]) + h(i);
        x(i+1)   = trapz([0 dt],[ux(i) ux(i+1)]) + x(i);
        
    end
    
    % h(i) < 0 is landing
    if h(i) >= 0
        time_land = t(i);
        index_land = i;
    end
    
end

% Cut vectors to right length
uy = uy(1:index_land); ux = ux(1:index_land);
ay = ay(1:index_land); ax = ax(1:index_land);
h  = h(1:index_land) ; x  = x(1:index_land) ;

end