% RASAero Sim Data Analysis
% Ruqayya Toorawa
% 02/14/17

%Plot Wind Speed Variations
WindSpeed = [4 8 12 16 20]
Altitude_Wind = [44.927 44.098 43.126 42.001 40.720]
plot (WindSpeed, Altitude_Wind,'-*')
xlabel('Wind Speed(m/s)')
ylabel('Apogee (kft)')
title('Wind Speed Variations')
axis([4, 20, 40, 46])
figure() 

%Plot Mass Variations
Mass = [59.3, 60.3, 61.3, 62.3, 64.3, 69.3, 74.3, 79.3]
Altitude_Mass = [45.001 44.102 43.226 42.376 40.733 36.982 33.678 30.748] 
plot (Mass, Altitude_Mass, '-*')
xlabel('Mass (lbs)')
ylabel('Apogee (kft)')
title('Mass Variations')
axis([59, 80, 0, 46])
figure() 

%Launch Angle Variations
Launch_Angle = [5 6 7]
Altitude_Angle = [45.631 45.335 45.001] 
plot (Launch_Angle, Altitude_Angle, '-*')
xlabel('Angle (deg from vertical)')
ylabel('Apogee (kft)')
title('Launch Angle Variations')
axis([5, 7, 40, 46])