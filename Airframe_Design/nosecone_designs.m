% Nose Cone Designs
% Ian Gomez, 11/1/16
clc; clear;


% Parabolic
k = .25:0.25:1;
R = 2;  % in
L_parabola = 22; % in

x = 0:.1:L_parabola;
for i = 1:length(k)
    for j = 1:length(x)
        y_parabola(i,j) = R.*((2.*(x(j)./L_parabola)-k(i).*(x(j)./L_parabola).^2)./(2-k(i)));
    end
end

% Conic 
fineness = 3:10;
L_cone = fineness.*R;
m = R./L_cone;

for i = 1:length(fineness)
    for j = 1:length(x)
        y_cone(i,j) = m(i).*x(j);
    end
end

%% Plotting

figure(1)
set(gcf,'color','w');
plot(x,y_parabola,x,-y_parabola)
xlabel('Length (in)')
ylabel('Radius (in)')
title('Parabolic Nose Cone')
grid on 

for i = 1:length(k)
   leg{i} = strcat('k = ',num2str(k(i))); 
end
legend(leg)

OR = -2.*ones(1,length(x));
figure(2); hold on
set(gcf,'color','w');
plot(x,y_cone,x,-y_cone)
plot(x,OR,'k',x,-OR,'k')
xlabel('Length (in)')
ylabel('Radius (in)')
title('Conical Nose Cone')
grid on 

for i = 1:length(fineness)
   leg2{i} = strcat('fineness = ',num2str(fineness(i))); 
end
legend(leg2)
