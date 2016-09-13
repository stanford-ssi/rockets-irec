clc
clear all

h_vec = zeros(1, 100);
v_vec = zeros(1, 100);

for i=1:100
    h_vec(i) = i * 340 * 3;
    v_vec(i) = 340 * 3;
end

length = 1;
surface_thickness = .005;
specific_heat = 910;
thermal_cond = 200;
density = 2700;
dt = 1;

T_layer_history = Surface_Temperature(h_vec, v_vec, dt, length, specific_heat, thermal_cond, surface_thickness, density);

plot(h_vec, T_layer_history(:,1));