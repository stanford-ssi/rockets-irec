%%% RASAero Plotter
% Ian Gomez
clc
filename = 'IREC_30K_4in_NewLayout_33kflight.CSV';
data = csvread(filename,1,0);

% Sorting data - IMPERIAL UNITS
t = data(:,1); % time
Ma = data(:,3); % Mach #
alpha = data(:,4); % angle of attack
CD = data(:,5); % coefficient of lift
CL = data(:,6); % coefficient of drag
T = data(:,7); % thrust
W = data(:,8); % weight
D = data(:,9); % drag
L = data(:,10); % lift
a_rel = data(:,11); % acceleration relative
a_vert = data(:,12); % acceleration vertical
a_horz = data(:,13); % acceleration horizontal
v_rel = data(:,14); % velocity relative
v_vert = data(:,15); % velocity vertical
v_horz = data(:,16); % velocity horizontal
pitch = data(:,17); % pitch
flight_path_angle = data(:,18);
alt = data(:,19); % altitude
dist = data(:,20); % distance

%% Speed off of launch rail

rail_l = 18;

i = 1;
while alt(i) < rail_l; i = i + 1; end
v_off_rail = v_rel(i)