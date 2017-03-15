function [new_x, new_v] = rk4_step(x, v, F)

%ASSUME mass is constant
ROCKET_MASS = 5;
TIME_STEP = .01;

x1 = x;
v1 = v;
a1 = F(x1, v1, 0)*1.0 / (ROCKET_MASS);

x2 = x + 0.5*v1*TIME_STEP;
v2 = v + 0.5*a1*TIME_STEP;
a2 = F(x2, v2, TIME_STEP/2.0) / (ROCKET_MASS);

x3 = x + 0.5*v2*TIME_STEP;
v3 = v + 0.5*a2*TIME_STEP;
a3 = F(x3, v3, TIME_STEP/2.0) / (ROCKET_MASS);

x4 = x + v3*TIME_STEP;
v4 = v + a3*TIME_STEP;
a4 = F(x4, v4, TIME_STEP) / (ROCKET_MASS);

new_x = x + (TIME_STEP/6.0)*(v1 + 2*v2 + 2*v3 + v4);
new_v = v + (TIME_STEP/6.0)*(a1 + 2*a2 + 2*a3 + a4);

   