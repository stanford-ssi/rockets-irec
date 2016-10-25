clc; close all; clear

target_alt = 3048; % m
Ma = 3; 

h_vec = zeros(1, 100);
v_vec = zeros(1, 100);

for i=1:100
    h_vec(i) = i * 340 * 3;
    v_vec(i) = 340 * Ma;
end

length = 3.5; % m, of rocket
surface_thickness = 0.00635; % m, which thickness should this be?
specific_heat = 1130; % J/kg*K of CF
thermal_cond = 78.8; % W/m*K
density = 2700;
dt = 1;

T_layer_history = Surface_Temperature(h_vec, v_vec, dt, length,...
    specific_heat, thermal_cond, surface_thickness, density);

plot(h_vec, T_layer_history(:,1));
xlim([h_vec(1) target_alt])
xlabel('Altitude (m)')
ylabel('Surface Temperature (C)')
title(strcat('Surface Temperature at Mach',num2str(Ma)))