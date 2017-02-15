%Plot Wind Speed Variations
WindSpeed = [4 8 12 16 20]
Altitude_Wind = [44.927 44.098 43.126 42.001 40.720]
plot (WindSpeed, Altitude_Wind,'-*')
xlabel('Wind Speed(m/s)')
ylabel('Apogee (kft)')
title('Wind Speed Variations')
axis([4, 20, 39, 46])
set(gca,'fontsize', 14)
figure() 

%Plot Mass Variations
Mass = [59.3, 60.3, 61.3, 62.3, 64.3, 69.3, 74.3, 79.3]
Altitude_Mass = [45.001 44.102 43.226 42.376 40.733 36.982 33.678 30.748] 
plot (Mass, Altitude_Mass, '-*')
xlabel('Mass (lbs)')
ylabel('Apogee (kft)')
title('Mass Variations')
axis([59, 80, 0, 46])
set(gca,'fontsize', 14)
figure() 

%Launch Angle Variations
Launch_Angle = [5 6 7]
Altitude_Angle = [45.631 45.335 45.001] 
plot (Launch_Angle, Altitude_Angle, '-*')
xlabel('Angle (deg from vertical)')
ylabel('Apogee (kft)')
title('Launch Angle Variations')
axis([5, 7, 39, 46])
set(gca,'fontsize', 14)
figure() 

%Surface Finish Variations
SF = [ 1 2 3 4 5 6 ]
Altitude_SF = [45.138 45.137 45.001 44.417 42.408 39.499]
plot (SF, Altitude_SF, '-*')
xlabel('Surface Finish')
ylabel('Apogee (kft)')
title('Surface Finish Variations')
axis([1, 6, 39, 46])
set(gca,'fontsize', 14)

