function [velocities,heights,n] = terminal_velocity_fun(m,cd_parachute,S_parachute)
% Terminal velocity solver
% Uses metric values, so we convert from lbs to kg and 

% Puts values in metric
lbs2kg = 0.453592;   % kg/lbs
in22m2 = 0.00064516; % m^2/in^2
m = m.*lbs2kg;
S_parachute = S_parachute.*in22m2; 

% Density profile of the atmosphere (0-13000m) 
g_0 = 9.8;  
T_0 = 288.16; %sea level temperature (kelvin)
rho_0 = 1.225; %sea level density (kg/m^3)
a = -0.0065; %lapse rate (K/m)
R = 287.05; %gas constant (J/kg*K)
den = []; 

for h = 1:11000 
    T = T_0 + a*h; 
    den(h) = rho_0 * (T/T_0)^((-g_0/(a*R)) - 1 );
end 
T_isot = 217; %isothermal temp (K)
for h = 11001:13000
    den(h) = den(11000) * (exp(1))^((-g_0/(T_isot*R))* (h-11000));
end

% Simulation

dt = .1;
n = 0;
v_i = 0;
h_i = 9144; %(30000 ft)
v_curr = v_i;
h_curr = h_i;
heights = [];
velocities = [];


while (h_curr > 10)
    
    heights = [heights h_curr];
    velocities = [velocities v_curr];
    
    F_g = m * g_0;
    F_drag = .5 * den(round(h_curr)) * v_curr^2 * S_parachute * cd_parachute;
    F_curr = F_drag - F_g;
    dv = (F_curr/m)*dt;
    v_curr = v_i + dv;
    dh = v_i * dt;
    v_i = v_curr;
    h_curr = h_i + dh;
    h_i = h_curr;
    n = n + 1;%counting the iterations for time
    
end
end