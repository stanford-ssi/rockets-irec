% Motor Impulse Extractor
% Ian Gomez
% 1/22/17

%% Data Import
try
    cd old_flight_data
    filename = '2016-05-21_DairyAire_Firestorm_H999_flattened.csv';
    data = xlsread(filename);
    cd ..
catch
    error('Change directory to the Trajectory_Sims folder')
end

%%
fts2ms = 0.3048;
ft2m = fts2ms;

t = data(:,1); % s
axial_accel = data(:,2); % Gs
T = data(:,6); % F
vel = data(:,16)*fts2ms; % m/s
alt_a = data(:,13)*ft2m; % m, based on accelerometer
alt_b_AGL = data(:,14)*ft2m; % m, AGL (based on barometer)
alt_b_ASL = data(:, 15)*ft2m; % m, ASL (based on barometer)

% figure(1)
% plot(t,vel)
% title('Velocity')
% xlabel('t'); ylabel('v')
% 
% figure(2)
% plot(t,alt_b_AGL)
% title('Altitude AGL')
% xlabel('t'); ylabel('h')

% find burn out velocity and time
burnout_v = -1; burnout_i = 1;
while(1)
    if burnout_v > vel(burnout_i)
        break;
    end
    burnout_v = vel(burnout_i);
    burnout_i = burnout_i + 1;
end

% jank integrator
t2burnout = t(1:burnout_i);
m_prop = 0.147; % kg
m_firestorm = 1.36078; % kg
m_dry = m_firestorm;
m_wet = m_firestorm + m_prop;
m = linspace(m_wet,m_dry,length(t2burnout));

% solve for acceleration from motor
S = (2*2.54e-2)^2 * pi;
Cd = 
rho = 
F_drag = .5*Cd*rho*S;

for i = 1:burnout_i
    % solve out for forces on rocket
    % force due to accel (i) = total accel * m(i) + force of gravity + drag
    J(i) = m(i)*a(i)*t(i);
end
% impulse = sum(J)