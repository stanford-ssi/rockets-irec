clc
clear all
R = 286.9;
k = 1.4;
T = 273;
P_atm = 10^5;

r = 2 * 2.54 / 100;
L = 36 * 2.54 / 100;
V = r^2 * 3.14159 * L;

r_hole = .125 * 2.54 / 100;
A_hole = r_hole^2 * 3.14159;

rho_air = P_atm / R / T;
P_drop_sec = 10^4;
%mass flow out = proportion of mass out per sec * mass
m_dot = P_drop_sec / P_atm * V * rho_air
v_hole_req = m_dot / A_hole / rho_air

P_diff = 1/2 * rho_air * v_hole_req^2

F = P_diff * r^2 * 3.14159

