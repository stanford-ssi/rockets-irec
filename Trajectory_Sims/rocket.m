function yprime = rocket (t,y)

m = 10;      % Mass in kg
g = 9.81; 

% ode45 requires the form d/dt([state vars]) = f([state vars])
% Package will use RK4 to solve step by step.

% dx/dt = Velocity from previous time step
yprime(1,1) = y(2);
% dv/dt = Net force / Mass
yprime(2,1) = linear_acceleration (m, y(1), y(2));
% d(theta)/ dt = angular velocity
yprime(3,1) = y(4);
yprime(4,1) = angular_acceleration (m, y(3), y(4));



% NEED TO UPDATE force function for calculating angular accelearation
% Only accounts for gravity and simple drag force model
function a = linear_acceleration (mass, height, velocity)
%f = -20;
a = (-mass*9.81 - .01*velocity^2)/ mass;

function alpha = angular_acceleration (m, angle, angular_velocity)
alpha = 0;

