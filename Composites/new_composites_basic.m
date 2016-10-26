clc
clear all
t1 = pi / 4;
t2 = -pi / 4;
    
%From 256, 6.89 converts from msi to GPa
%Only value from Rockwest composites is 242GPa instead of 139 GPa (for Ex)
Ex = 20.2 * 6.89;
Ey = 1.4 * 6.89;
vx = .29;
vy = .29;
Gxy = .8 * 6.89;

Q = [Ex / (1 - vx * vy), vx * Ey / (1 - vx * vy), 0; ...
    vy * Ex / (1 - vx * vy), Ey / (1 - vx * vy), 0; ...
    0, 0, Gxy];

T1 = [cos(t1)^2, sin(t1)^2, 2 * sin(t1) * cos(t1); ...
    sin(t1)^2, cos(t1)^2, -2 * sin(t1) * cos(t1); ...
    -sin(t1) * cos(t1), sin(t1) * cos(t1), cos(t1)^2 - sin(t1)^2];
i_T1 = inv(T1);

T2 = [cos(t2)^2, sin(t2)^2, 2 * sin(t2) * cos(t2); ...
    sin(t2)^2, cos(t2)^2, -2 * sin(t2) * cos(t2); ...
    -sin(t2) * cos(t2), sin(t2) * cos(t2), cos(t2)^2 - sin(t2)^2];
i_T2 = inv(T2);

Q_bar_1 = i_T1 * Q * transpose(i_T1)
Q_bar_2 = i_T2 * Q * transpose(i_T2)

Q_bar_1 * 242 / 139
Q_bar_2 * 242 / 139