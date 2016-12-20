clc
clear all

%Dummy height and velocity files
h_vec = zeros(1, 100);
v_vec = zeros(1, 100);
dt = .2;

for i=1:100
    v_vec(i) = 340 * 2;
    if i > 1
        h_vec(i) = h_vec(i - 1) + v_vec(i) * dt;
    else
        h_vec(i) = v_vec(i) * dt;
    end
end

%Vaguely corresponds to aluminum
length = 1;
%.08'' thick
surface_thickness = .08 * 2.54 / 100;
specific_heat = 910;
thermal_cond = 20;
density = 2700;

T_layer_history = Surface_Temperature(h_vec, v_vec, dt, length, specific_heat, thermal_cond, surface_thickness, density);

plot(h_vec / 1000, T_layer_history(:,1), h_vec / 1000, T_layer_history(:,2), h_vec / 1000, T_layer_history(:,3), ...
    h_vec / 1000, T_layer_history(:,4), h_vec / 1000, T_layer_history(:,5));
xlabel('Height, km');
ylabel('Surface Temp, K');