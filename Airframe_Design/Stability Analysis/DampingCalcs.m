%% Calculate Pitch Damping Characteristics
% Nicholas Gloria
% 5/16/17

clear
close all

%% Settings
rowoffset = 9;
coloffset = 0;

%% Constants
R_air = 287.1; % J/kg*K
gamma = 1.4;

%% Load Data
[FileName,PathName,FilterIndex] = uigetfile('*.csv','Select OpenRocket Data Export (.csv)');
filepath = [PathName FileName];

% Extract Data
fid = fopen(filepath);

data = zeros(100000,100);

ii = 1;
while 1
    line = fgetl(fid);
    if strcmp(line(1),'#')
        continue
    elseif line == -1
        break
    end
    line_data = str2num(line);
    line_length = length(line_data);
    data(ii,1:line_length) = line_data;
    ii = ii + 1;
end

data = data(1:ii-1,1:line_length);

fclose(fid);

%% OpenRocket Inputs
X_ne = 131; % nozzle exit location from nose cone, in

% Aerodynamic
[FileName,PathName,FilterIndex] = uigetfile('*.xlsx','Select CN_alpha, CP Data (.xlsx)');
filepath = [PathName FileName];
aero_data = xlsread(filepath);
Mach_ind = aero_data(:,1)'; 
    % Range of Mach numbers over which aerodynamics are characterized
CN_alpha_ind = aero_data(:,2)'; 
CP_ind = aero_data(:,3)'; 
CN_alpha_nc_ind = aero_data(:,4)'; 
CN_alpha_cans_ind = aero_data(:,5)'; 
CN_alpha_fins_ind = aero_data(:,6)';
CN_alpha_tc_ind = aero_data(:,7)';
CP_nc_ind = aero_data(:,8)'; % in
CP_cans_ind = aero_data(:,9)'; % in
CP_fins_ind = aero_data(:,10)'; % in
CP_tc_ind = aero_data(:,11)'; % in

% Conversion
X_ne = X_ne*0.0254;
CP_ind = CP_ind*0.0254;
CP_nc_ind = CP_nc_ind*0.0254;
CP_cans_ind = CP_cans_ind*0.0254;
CP_fins_ind = CP_fins_ind*0.0254;
CP_tc_ind = CP_tc_ind*0.0254;

% Other inputs
% Damping ratio acceptable range
min_DR = 0.05;
max_DR = 0.3;

%% Extract variables
time_ind = 1;
cg_ind = 25;
air_temp_ind = 50;
air_pressure_ind = 51;
velocity_ind = 5;
area_ref_ind = 46;
prop_mass_ind = 21;
I_long_ind = 22;

time = data(:,time_ind);
CG = data(:,cg_ind);
air_temp = data(:,air_temp_ind);
air_pressure = data(:,air_pressure_ind);
velocity = data(:,velocity_ind);
area_ref = data(:,area_ref_ind);
prop_mass = data(:,prop_mass_ind);
I_long = data(:,I_long_ind);

% Conversions
CG = CG*0.0254; % convert from in to m
air_temp = (air_temp + 459.7)/1.8; % convert from F to K
air_pressure = 1e2*air_pressure; % convert from mbar to Pa
velocity = velocity*0.3048; % ft/s to m/s
area_ref = area_ref*6.452e-4; % in^2 to m^2
prop_mass = prop_mass*0.4536; % lb to kg
I_long = I_long*0.04214; % lb*ft^2 to kg*m^2

% Calculations
rho_air = air_pressure./(R_air*air_temp);
prop_m_dot = -[diff(prop_mass)./diff(time); 0];
sound_speed = sqrt(gamma*R_air*air_temp);
mach = velocity./sound_speed;
L_ref = sqrt(area_ref*4/pi);
q = 1/2*rho_air.*velocity.^2;

% Aerodynamic Mach-interpolation
CN_alpha = interp1(Mach_ind, CN_alpha_ind, mach);
CP = interp1(Mach_ind, CP_ind, mach);
CN_alpha_nc = interp1(Mach_ind, CN_alpha_nc_ind, mach);
CN_alpha_cans = interp1(Mach_ind, CN_alpha_cans_ind, mach);
CN_alpha_fins = interp1(Mach_ind, CN_alpha_fins_ind, mach);
CN_alpha_tc = interp1(Mach_ind, CN_alpha_tc_ind, mach);
CP_nc = interp1(Mach_ind, CP_nc_ind, mach);
CP_cans = interp1(Mach_ind, CP_cans_ind, mach);
CP_fins = interp1(Mach_ind, CP_fins_ind, mach);
CP_tc = interp1(Mach_ind, CP_tc_ind, mach);


%% Calculations
C1 = 1/2*rho_air.*velocity.^2.*area_ref.*CN_alpha.*(CP-CG); 
    % Corrective moment coefficient
C2R = prop_m_dot.*(X_ne-CG).^2; % Propulsive damping coefficient
C2A = 1/2*rho_air.*velocity.*area_ref.*(...
    CN_alpha_nc.*(CP_nc-CG).^2+...
    CN_alpha_cans.*(CP_cans-CG).^2+...
    CN_alpha_fins.*(CP_fins-CG).^2+...
    CN_alpha_tc.*(CP_tc-CG).^2);
    % Aerodynamic damping coefficient
C2 = C2R + C2A; % Damping coefficient
DR = C2./(2*(C1.*I_long).^0.5); % Damping Ratio
SM = (CP-CG)./L_ref;

DR_valid_ind = find(~isnan(DR),1,'last');

%% Plots
figure
subplot(3,1,1)
plot(time,DR,time(1:DR_valid_ind),min_DR*ones(DR_valid_ind,1),'--',...
    time(1:DR_valid_ind),max_DR*ones(DR_valid_ind,1),'--')
xlabel('Time [s]')
ylabel('Damping Ratio (\zeta) []')
ylim([0 0.35])
xlim([0 20])
legend({'Flight','Recommended Min.','Recommended Max.'})
subplot(3,1,2)
plot(time(1:DR_valid_ind),SM(1:DR_valid_ind))
xlim([0 20])
xlabel('Time [s]')
ylabel('Stability Margin [caliber]')
subplot(3,1,3)
plot(time(1:DR_valid_ind),q(1:DR_valid_ind)/1e3)
xlim([0 20])
xlabel('Time [s]')
ylabel('Dynamic Pressure [kPa]')
suptitle('Dynamic Stability Analysis')

figure
subplot(2,1,1)
plot(time(1:DR_valid_ind),C1(1:DR_valid_ind))
xlabel('Time [s]')
ylabel({'Corrective Moment Coefficient','[J rad^{-1}]'})
subplot(2,1,2)
plot(time(1:DR_valid_ind),C2R(1:DR_valid_ind),...
    time(1:DR_valid_ind),C2A(1:DR_valid_ind),...
    time(1:DR_valid_ind),C2(1:DR_valid_ind))
xlabel('Time [s]')
ylabel({'Damping Moment Coefficient','[kg m^2 s^{-1} rad^{-1}]'})
legend({'Propulsive','Aerodynamic','Total'})
