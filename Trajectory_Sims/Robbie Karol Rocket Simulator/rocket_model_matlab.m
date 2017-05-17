% Missile Model
lat = 37.4292; % deg
lon = -122.1381; % deg
dyear = decyear('13-January-2014','dd-mmm-yyyy');

phi0 = .1;
theta0 = pi/2+.2;
psi0 = -0.3;

full_mass = 50; % kg
empty_mass = 10; % kg 

full_cg = [1; 0; 0]; % m
empty_cg = [.2; 0; 0]; % m

full_intertia_matrix = eye(3); % metric
empty_intertia_matrix = eye(3)/2; % metric

href = 0;
threshold = 0;

r = .05; % m
Thruster_Thrust = 1; % N
Thruster_Moment = r*Thruster_Thrust; % Moment

cp = [1.2; 0; 0]; % m, in body axis, x is front of rocket
coef_body = [0; 0; 0; 0; 0; 0]; % Aerodynamics Block

% Create Main Enginer Thrust Block
% Thruster 1 is on the z axis. Follow with RHR around x-axis.

sim('rocket_model.slx')
%open('rocket_model.slx')

%%
plot3(xyz.Data(:,1),xyz.Data(:,2),-xyz.Data(:,3))
legend('Flight Path')
pause
plot(rpy.Time,rpy.Data)
legend('r','p','y')

%% Examples
%aeroblk_HL20
