% Payload ballast
% Ian Gomez, 10/29/16
clc; clear; close all;

g2lbs = 0.00220462;  % lbs/g
cm2in = 0.393701;    % in/cm
price = 0;
ballast.names = {'Steel, tool; ','Wrought Iron; ', 'Carbon Tool Steel; '...
    'Steel, cold-drawn; ','Carbon Steel; ','Steel, C1020, HR; ',... 
    'Pure Iron; ','Soft Steel (0.06% C); ','Stainless Steel, 304; ',...
    'Bismuth; ','Tungsten Carbide; ','Tungsten'};

ballast.rho   = [7.72,7.75,7.82,7.83,7.84,7.85,7.86,7.87,8.03,9.78,...
    15.60,19.25];    % g/cm^3
ballast.side = 5;    % cm
V = ballast.side^3;  % cm^3

ballast.weightpercube = ballast.rho.*V.*g2lbs; % lbs
ballast.target_weight = 8.8;                   % lbs

number = zeros(1,length(ballast.rho));
for i = 1:length(ballast.rho)
    num = 0;
    while(ballast.target_weight > num*ballast.weightpercube(i))
        num = num + 1;
    end
    ballast.number(i) = num;
end
ballast.length = ballast.number.*ballast.side.*cm2in;

ballast