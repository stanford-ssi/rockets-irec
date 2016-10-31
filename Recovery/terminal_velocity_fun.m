function [velocities,heights,n] = terminal_velocity_fun(m,cd_parachute,S_parachute,dt)
% Terminal velocity solver
% Uses metric values, so we convert from lbs to kg and 

% Starting altitude
h0 = 9144; % m, = 30,000ft

% Puts values in metric
lbs2kg = 0.453592;   % kg/lbs
in22m2 = 0.00064516; % m^2/in^2
m = m.*lbs2kg;
S_parachute = S_parachute.*in22m2; 

%% Density profile of the atmosphere (0-13000m) 
% Seems unnecessary to run this everytime
g0 = 9.8;  
T0 = 288.16;  % sea level temperature (kelvin)
rho0 = 1.225; % sea level density (kg/m^3)
a = -0.0065;  % lapse rate (K/m)
R = 287.05;   % gas constant (J/kg*K)
rho = zeros(1,h0);
for h = 1:11000 
    T = T0 + a*h; 
    rho(h) = rho0 * (T/T0)^((-g0/(a*R)) - 1 );
end 
T_isot = 217; % isothermal temp (K)
for h = 11001:13000
    rho(h) = rho(11000) * (exp(1))^((-g0/(T_isot*R))* (h-11000));
end

%% Simulation
n = 0;        % counter
v_i = 0;      % beginning speed
h_i = h0;     % 30000 ft
v_curr = v_i; % Current velocity
h_curr = h_i; % Current height

heights = []; velocities = [];
while (h_curr > 10)
    
    % Append the old heights to the new heights
    heights    = [heights,    h_curr];
    velocities = [velocities, v_curr];
    
    % Find forces on bodies (gravity and drag) and find resultant force
    F_g = m * g0;
    F_drag = .5 * rho(round(h_curr)) * v_curr^2 * S_parachute * cd_parachute;
    F_curr = F_drag - F_g;
    
    % New change in velocity is specific force * small change in time
    % Add to current velocity 
    dv = (F_curr/m)*dt;
    v_curr = v_i + dv;
    
    % New height is the change in height from the new velocity * change in
    % time
    dh = v_i * dt;
    h_curr = h_i + dh;
    
    % Set the current velocity and height to the newly found ones
    v_i = v_curr;
    h_i = h_curr;
    n = n + 1; %counting the iterations for time
    
end
end