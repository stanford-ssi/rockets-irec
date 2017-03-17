% Sources: 
% https://www.mathworks.com/help/matlab/ref/ode45.html
% https://www.youtube.com/watch?v=g22pZfoTNTQ

y0(1,1) = 0;       % Initial height [m]
y0(2,1) = 50;       % Initial speed [m/s]
y0(3,1) = 0;        % Initial Angle (relative to normal)
y0(4,1) = 0;         % Initial Angular velocity
tspan = [0 10];     % Guessed time span of flight in seconds
[t,y] = ode45 ('rocket', tspan, y0); 
 
% plot (t,y);
% xlabel ('Time [s]')
% ylabel ('Height, Velocity, Angle, Angular Velocity')
% title ('Rocket Trajectory')
% legend ('Height', 'Velocity', 'Angle', 'Angular Velocity','Location', 'Northwest')
