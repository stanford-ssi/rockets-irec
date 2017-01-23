% Motor Impulse Extractor
% Ian Gomez
% 1/22/17 

%% Data Import

cd old_flight_data
filename = '2016-05-21_DairyAire_Firestorm_H999_flattened.csv';
data = xlsread(filename);
cd ..

t = data(:,1); % s
axial_accel = data(:,2); % Gs
T = data(:,6); % F
vel = data(:,7); % ft/s
alt_a = data(:,13); % ft, based on accelerometer 
alt_b_AGL = data(:,14); % ft, AGL (based on barometer)
alt_b_ASL = data(:, 15); % ft, ASL (based on barometer)

