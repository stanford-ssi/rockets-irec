function[X1] = X1_method1(mass, drag_area)
% Logan Herrera
% Stanford Student Space Initiative - IREC
% 
% References:
% Parachute Recovery Systems Design Manual - T. W. Knacke - 1992

% digitize Figure 5-48
% the results of this interpolation are off by up to 10% from book plot
% I am not sure why

g = 9.780318;

force = mass * g;
canopy_loading_Pa = force / drag_area;
canopy_loading_PSF = 0.02089 * canopy_loading_Pa;

if canopy_loading_PSF > 100
    X1 = 1;
    
else
    figure5_48 = csvread('canopyloadingPSF_vs_forcereductionfactorX1.csv');

    X1 = interp1(figure5_48(:,1),figure5_48(:,2),canopy_loading_PSF,'spline');
end