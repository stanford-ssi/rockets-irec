clc
clear all

%Dummy height and velocity files
h_vec = zeros(1, 100);
v_vec = zeros(1, 100);

for i=1:100
    v_vec(i) = 340 * 3;
    if i > 1
        h_vec(i) = h_vec(i - 1) + v_vec(i);
    else
        h_vec(i) = v_vec(i);
    end
end

%Vaguely corresponds to aluminum
length = 1;
surface_thickness = .005;
specific_heat = 910;
thermal_cond = 200;
density = 2700;
dt = 1;

T_layer_history = Surface_Temperature(h_vec, v_vec, dt, length, specific_heat, thermal_cond, surface_thickness, density);

plot(h_vec / 1000, T_layer_history(:,1));
xlabel('Height, km');
ylabel('Surface Temp, K');