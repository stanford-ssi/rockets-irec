%% Plot Wind Speed Variations
WindSpeed = [4 8 12 16 20]
Altitude_Wind = [44.927 44.098 43.126 42.001 40.720]
Altitude_Wind_WC = [37.093 36.401 35.588 34.644 33.600]
scatter (WindSpeed, Altitude_Wind,'filled')
hold on 
scatter (WindSpeed, Altitude_Wind_WC,'filled')
grid on 
xlabel('Wind Speed(m/s)')
ylabel('Apogee (kft)')
title('Wind Speed Variations')
legend('Probable', 'Worst Case', 'Location', 'best')
axis([4, 20, 32, 46])
%set(gca,'fontsize', 14)
figure() 

%% Plot Mass Variations

Mass = [59.3, 60.3, 61.3, 62.3, 64.3, 69.3, 74.3, 79.3, 84.3]
Altitude_Mass = [44.411 43.537 42.686 41.860 40.261 36.607 33.384 30.552 27.987] 
Altitude_Mass_WC = [41.761, 40.919, 20.098, 39.327,37.782, 34.258, 31.161, 28.393, 25.967]
%plot (Mass, Altitude_Mass, '-*', Mass, Altitude_Mass_WC, '-*')
scatter(Mass, Altitude_Mass, 'filled')
grid on 
xlabel('Mass (lbs)')
ylabel('Apogee (kft)')
title('Mass Variations')
%legend('Probable', 'Worst Case')
axis([59, 85, 0, 45])
%xticks([75 77 79 81 83 85])
%yticks([0 5 10 15 20 25 30 35 40 45])
set(gca,'fontsize', 14)
%figure() 
%% Launch Angle Variations

Launch_Angle = [5 6 7]
Altitude_Angle = [45.631 45.335 45.001] 
Altitude_Angle_WC = [34.496 34.039, 33.563] 
plot (Launch_Angle, Altitude_Angle, '-*', Launch_Angle, Altitude_Angle_WC, '-*')
xlabel('Angle (deg from vertical)')
ylabel('Apogee (kft)')
title('Launch Angle Variations')
legend('Probable', 'Worst Case', 'Location', 'best')
%axis([5, 7, 39, 46])
%set(gca,'fontsize', 14)
figure() 
%% Surface Finish Variations
SF = [ 1 2 3 4 5 6 7]
Altitude_SF = [45.138 45.137 45.001 44.417 42.408 39.499 38.397]
scatter (SF, Altitude_SF, 'filled')
xlabel('Surface Finish')
ylabel('Apogee (kft)')
title('Surface Finish Variations')
axis([1, 7, 38, 46])
grid on 
set(gca,'fontsize', 14)