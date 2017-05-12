%%
clear
close all
clc

a = 10528;
e = .3256;
i = .7854;
OM = 1.5708;
om = 0;
M = 0;
v = 0;

mu = 3.986E5;
J2 = 1.0826E-3;
Re = 6378;

[rINT, vINT] = orbit2ECI(a,e,i,OM,om,v);

sim('Pset8.slx')

plotearth
hold on
stateout = stateout./Re;
plot3(stateout(:,1), stateout(:,2), stateout(:,3), 'r')

figure
plot(tout(50:end),RAAN_out(50:end))
hold on
plot(tout(50:end), 90 + -1.7658E-5.*tout(50:end))
title('RAAN vs. time')
xlabel('Time (s)')
ylabel('RAAN (deg)')

figure
plot(tout(50:end), AoP_out(50:end))
hold on
plot(tout(50:end), 1.8729E-5.*tout(50:end))
title('AoP vs. time')
xlabel('Time (s)')
ylabel('AoP (deg)')

figure
plot(tout, i_out)
title('i vs. time')
xlabel('Time (s)')
ylabel('Inclination (deg)')

figure
plot(tout, a_out)
title('a vs. time')
xlabel('Time (s)')
ylabel('a (km)')

figure
plot(tout, e_out)
title('e vs. time')
xlabel('Time (s)')
ylabel('e')

%%
clear
close all
clc

rINT = [6553; 0; 0];
vINT = [0; 10.34; 0];
%[a, e, i, RAAN, AoP, TA] = ECI2orbit(rINT, vINT);

sim('dragEffects.slx')

%%
plot(tout, sqrt(sum(stateout(:,1:3).^2, 2)) - 6378)
title('altitude vs. time')
xlabel('Time (s)')
ylabel('Altitude (km)')

plotearth
hold on
stateout = stateout./6378.1;
plot3(stateout(:,1), stateout(:,2), stateout(:,3), 'r')

figure
plot(rPQWout(:,1), rPQWout(:,2))
title('Orbit Sketch in PQW')
xlabel('km')
ylabel('km')

disp(tout(end))

%%
clear
close all
clc

rINT = [6613.137; 0; 0];
vINT = [0; 7.763631; 0];
[a, e, i, RAAN, AoP, TA] = ECI2orbit(rINT, vINT);

sim('dragEffects.slx')

plot(tout, sqrt(sum(stateout(:,1:3).^2, 2)) - 6378.137)
title('altitude vs. time')
xlabel('Time (s)')
ylabel('Altitude (km)')

figure
plot(tout, sqrt(sum(stateout(:,4:6).^2, 2)))
title('speed vs. time')
xlabel('Time (s)')
ylabel('Speed (km/s)')

figure
plot(tout, sqrt(sum(stateout(:,4:6).^2, 2)).^2 ./ 2)
title('specific kinetic energy vs. time')
xlabel('Time (s)')
ylabel('Specific Kinetic Energy (J/kg)')

figure
plot(tout, -3.986E5./sqrt(sum(stateout(:,1:3).^2, 2)))
title('specifc potential energy vs. time')
xlabel('Time (s)')
ylabel('Specific Potential Energy (J/kg)')

figure
plot(tout, sqrt(sum(stateout(:,4:6).^2, 2)).^2 ./ 2 - 3.986E5./sqrt(sum(stateout(:,1:3).^2, 2)))
title('specific mechanical energy vs. time')
xlabel('Time (s)')
ylabel('Specific Mechanical Energy (J/kg)')
