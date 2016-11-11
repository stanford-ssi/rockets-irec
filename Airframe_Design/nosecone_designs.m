% Nose Cone Designs
% Ian Gomez, 11/1/16
clc; clear;

L = 22; % in
x = 0:.01:L;


% Parabolic
k = .25:0.25:1;
R = 2;  % in
for i = 1:length(k)
    for j = 1:length(x)
        y_parabola(i,j) = R.*((2.*(x(j)./L)-k(i).*(x(j)./L).^2)./(2-k(i)));
    end
end

% Conic
fineness = 4;
L_cone = fineness.*2.*R;
m = R./L_cone;
for i = 1:length(fineness)
    for j = 1:length(x)
        y_cone(i,j) = m(i).*x(j);
        if y_cone(i,j) < R
            OD_index(i) = j;
        end
    end
end

%% Plotting

figure(1)
axis equal
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
axis equal
set(gcf,'color','w');
for j = 1:length(fineness)
    plot(x(1:OD_index(j)),y_cone(j,1:OD_index(j)),...
        x(1:OD_index(j)),-y_cone(j,1:OD_index(j)),'k')
end
plot(x,ones(1,length(x)).*(2*sqrt(2)/2))
xlabel('Length (in)')
ylabel('Radius (in)')
title('Conical Nose Cone')
grid on
hold off

for i = 1:length(fineness) % this is broken
    leg2{i} = strcat('fineness = ',num2str(fineness(i)));
end
legend(leg2)
