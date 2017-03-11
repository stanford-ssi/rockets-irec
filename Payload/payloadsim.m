%=======================================================================
%                               Goal 1:
% 
% -Create a 3D path for two payloads (done)
%
%  At each time step, payloads must:
% -Calculate distance between payloads (done)
% -Obtain acceleration information of k-1 and k (done)
% -Integrate odometry (done)
% -Determine relative position
%%
%Trajectory taken from Union College Aero Design Team and further edited by Calvin
close all
clear
clc
r = 0.05; % meters, radius of payload package
tr = 50; % feet, radius of target
m = 1.5; % pounds, max of package
CD = 0.6;
posG(1) = 30; %x coordinate of ground station
posG(2) = 15; %y coordinate of ground station
posG(3) = 0;   %z coordinate of ground station
% Conversions
mphToMetersSecond = 0.44704;
metersSecondToMPH = 1/0.44704;
feetToMeters = 0.3048;
metersToFeet = 1/0.3048;
% Initial Conditions
alt = 100; % Feet

vx0u = 30; % mph of payload U
vx0w = 15; % mph of payload W

vy0u = 5; % mph of payload U
vy0w = -5; % mph of payload W

vz0u = 0; % mph of payload U
vz0w = 0; % mph of payload W
%Initial conditions converted into metric
alt = alt*feetToMeters; % Meters, drop height of 100 feet = 30.5 meters

vx0u = vx0u*mphToMetersSecond; % m/s, initial x velocity
vx0w = vx0w*mphToMetersSecond; % m/s, initial x velocity

vy0u = vy0u*mphToMetersSecond; % m/s, initial y velocity
vy0w = vy0w*mphToMetersSecond; % m/s, initial y velocity

vz0u = vz0u*mphToMetersSecond; % m/s, initial z velocity
vz0w = vz0w*mphToMetersSecond; % m/s, initial z velocity

% Coefficents
rho = 1.225; % kg/m^3
CD;
% Constants
A = pi*r^2; % Area of package, estimated as a 5cm radius
g = 9.81; % Gravity, m/s/s
m = m*0.453592; % mass of package, kg
targetR = tr*feetToMeters; % Radius of target area meters
dt = 0.1; % Time step
t = rot90(0.1:dt:2.7, 3);
n = 0; % Step stopping number

vxu(1) = vx0u;
vxw(1) = vx0w;

vyu(1) = vy0u;
vyw(1) = vy0w;

vzu(1) = vz0u;
vzw(1) = vz0w;

dxu(1) = 0;
dxw(1) = 0;

dyu(1) = 0;
dyw(1) = 0;

dzu(1) = alt;
dzw(1) = alt + 1; %Payloads deploy at different altitudes

posU = zeros(27,1);
posW = zeros(27,1);


i = 1;
while dzu(i) >= 0
    azu(i) = -g + 0.5*rho*CD*A*vzu(i)^2;             %acceleration in z-direction
    vzu(i+1) = vzu(i) + azu(i)*dt;                   %velocity in z-direction
    dzu(i+1) = dzu(i) + vzu(i)*dt + 0.5*azu(i)*dt^2; % new distance in z
    posU(i,3) = dzu(i);
    axu(i) = -0.5*rho*CD*A*vxu(i)^2;                 % acceleration in x-direction
    vxu(i+1) = vxu(i) + axu(i)*dt;                   % velocity in x-direction
    dxu(i+1) = dxu(i) + vxu(i)*dt + 0.5*axu(i)*dt^2; % new distance in x
    posU(i,1) = dxu(i);
    ayu(i) = -0.5*rho*CD*A*vyu(i)^2;                 % acceleration in y-direction
    vyu(i+1) = vyu(i) + ayu(i)*dt;                   % velocity in y-direction
    dyu(i+1) = dyu(i) + vyu(i)*dt + 0.5*ayu(i)*dt^2; % new distance in y
    posU(i,2) = dyu(i);
    i = i + 1;
end

i = 1;
while dzw(i) >= 0
    azw(i) = -g + 0.5*rho*CD*A*vzw(i)^2;              %acceleration in z-direction
    vzw(i+1) = vzw(i) + azw(i)*dt;                    %velocity in z-direction
    dzw(i+1) = dzw(i) + vzw(i)*dt + 0.5*azw(i)*dt^2;  % new distance in z
    posW(i,3) = dzw(i);
    axw(i) = -0.5*rho*CD*A*vxw(i)^2;                  % acceleration in x-direction
    vxw(i+1) = vxw(i) + axw(i)*dt;                    % velocity in x-direction
    dxw(i+1) = dxw(i) + vxw(i)*dt + 0.5*axw(i)*dt^2;  % new distance in x
    posW(i,1) = dxw(i);
    ayw(i) = -0.5*rho*CD*A*vyw(i)^2;                  % acceleration in y-direction
    vyw(i+1) = vyw(i) + ayw(i)*dt;                    % velocity in y-direction
    dyw(i+1) = dyw(i) + vyw(i)*dt + 0.5*ayw(i)*dt^2;  % new distance in y
    posW(i,2) = dyw(i);
    i = i + 1;
end

axu = rot90(axu,3); ayu = rot90(ayu,3); azu = rot90(azu,3);
axw = rot90(axw,3); ayw = rot90(ayw,3); azw = rot90(azw,3);
%Acceleration in x, y, and z for payload U and W
accdatau = horzcat(axu, ayu, azu);
accdataw = horzcat(axw, ayw, azw);


leadTime     = (length(dxu)-1)*dt;
leadDistance = dxu(length(dxu)-1);
    
if max(dxu) > max(dzu)
    axisLimit = ceil(max(dxu)*1.15);
else
    axisLimit = ceil(max(dzu)*1.15);
end

figure;

hold on
grid on
plot3(dxu, dyu, dzu, '.-');
plot3(dxw, dyw, dzw,'.-');

axis([0 axisLimit -axisLimit/2 axisLimit/2 0 axisLimit]);
axis square
title('Payload U and Payload W');
xlabel('X-Direction (meters)');
ylabel('Y-Direction (meters)');
zlabel('Z-Direction (meters)');
legend('Projectile Path of Payload U', 'Projectile Path of Payload W');

dUW = zeros(26,1);

%Euclidian distance btwn payloads at each time step
for j = 1:1:26
    dUW(j) = pdist2(posU(j,:), posW(j,:));
end

%%
%=======================================================================
%Integrate Odometry ? Dead Reckoning (i >= j-3)
%=======================================================================
% i = first round
% j = any round after that
% i < j
% k = current round

%For the purpose of first simulation and algorithm checking, 
%we assume we have initial velocity.

for i = 1:1:25
    velzu(1) = vz0u;
    velzu(i+1,1) = velzu(i) + azu(i)*dt;
end

%Computed displacement for payload U
dispxu = 0.5*(dt^2)*axu + vx0u*dt;
dispyu = 0.5*(dt^2)*ayu + vy0u*dt;
dispzu = 0.5*(dt^2)*azu + velzu*dt;

for i = 1:1:25
    velzw(1) = vz0w;
    velzw(i+1,1) = velzw(i) + azw(i)*dt;
end

%Computed displacement for payload W
dispxw = 0.5*(dt^2)*axw + vx0w*dt;
dispyw = 0.5*(dt^2)*ayw + vy0w*dt;
dispzw = 0.5*(dt^2)*azw + velzw*dt;


dispu = horzcat(dispxu, dispyu, dispzu); %odometry of payload U
dispw = horzcat(dispxw, dispyw, dispzw); %odometry of payload W

%%
%=======================================================================
% Calculate relative position using ranging and odometry
%=======================================================================

relpose = zeros(25,6); %initialization of relative pose equation unknowns
relpose0 = [0,0]; %initial guess 

for s = 2:1:25 %Collapse everything into one function
    %dispu = displacement of U
    %dispw = displacement of W
    %dUW = distance between U and W
    fun = @(relpose) posefunx(relpose, s, dispu, dispw, dUW); 
    relposetempx = lsqnonlin(fun, relpose0);
    relposex(s-1,:) = relposetempx;
    
    fun = @(relpose) posefuny(relpose, s, dispu, dispw, dUW);
    relposetempy = lsqnonlin(fun, relpose0);
    relposey(s-1,:) = relposetempy;
    
    fun = @(relpose) posefunz(relpose, s, dispu, dispw, dUW);
    relposetempz = lsqnonlin(fun, relpose0);
    relposez(s-1,:) = relposetempz;
end

%Two unknowns per coordinate for six unknowns:

relpose = horzcat(relposex, relposey, relposez);

relposition = zeros(24,3);
for i = 1:1:24
    relposition(i,1) = dUW(i,1)*relpose(i,2);
    relposition(i,2) = dUW(i,1)*relpose(i,4);
    relposition(i,3) = dUW(i,1)*relpose(i,6);
end
