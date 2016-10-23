function [P, rho, T] = Atmosphere_Model(h)

%Upper stratosphere
if h > 25000
    T = -131.21 + .00299 * h;
    P = 2.488 * ((T + 273.1) / 216.6)^-11.388;
    
%Lower stratosphere
elseif h > 11000
    T = -56.46;
    P = 22.65 * exp(1.73 - .000157 * h);
   
%Troposphere
else
    T = 15.04 - .00649 * h;
    P = 101.29 * ((T + 273.1) / 288.08)^5.256;
end

rho = P / (.2869 * (T + 273.1));

T = T + 273;                                                    %K and Pa
P = P * 1000;
end