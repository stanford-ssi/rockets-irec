flight_name = 'TCC_2017-5-19';
fname = strcat(flight_name, '_Skybass\');

START = 557200;
LEN = 6200;
%APO = 892;

if ~(exist('bmp') && exist('mma'))
    bmp = csvread(strcat(fname,'bmp.csv'));
    bmp = bmp(START:START+LEN,:);
    mma = csvread(strcat(fname,'mma.csv'));
    mma = mma(START:START+LEN,:);
end



%%


p = (bmp(:,2) + bmp(:,3))/2;
h = (1-((p)./101350).^0.190284) .* 145366.45;  %%in ft
h = (h - h(1));
t_bmp = bmp(:,1);
t_offset = t_bmp(1);
t_bmp = t_bmp - t_offset;
t_bmp = t_bmp/1000000;

ax = mma(:,2);
ay = -mma(:,3);
t_mma = mma(:,1) - t_offset;
t_mma = t_mma/1000000;

%clean up bad samples
for i = 1:LEN
    if (ax(i) < -400) || (ay(i) < -400)
        ax(i) = (ax(i-1)+ax(i+1))/2;
        ay(i) = (ay(i-1)+ay(i+1))/2;
    end
end

%shift bmp data to match MMA
t_bmp = t_bmp(3:end);
h = h(3:end);


x = zeros(3,1);
xs = zeros(3,round(LEN/5));
F = zeros(3,3);
H = [1,0,0;0,0,1];
R = [100,0,;0,1];
Q = 1e-3*[2,1,0;1,4,0;0,0,100];
P = zeros(3,3);
Ps = zeros(round(LEN/5),3,3);
t_prev = 0;
clf
hold on
for i = 1:round(LEN/5)
        %predict 
        t = t_mma(i);
        dt = t - t_prev;
        F = [1, dt, (dt^2)/2 ; 0, 1, dt; 0,0,1];
        x_p = F*x;
        P_p = F*P*F' + Q;

        %update
        R(1,1) = abs(x_p(2))*0.5 + abs(x_p(3))*0.5+10;
        if R(1,1) > 100
            R(1,1) = 100;
        end
        K = P_p * H' * inv(H*P_p*H' + R);
        x = x_p + K*([h(i);ay(i)] - H*x_p);
        P = (eye(3) - K*H)*P_p;
        
        dstr_x = (x(1)-10):0.1:(x(1)+10); 
        dstr_p = normpdf(dstr_x,x(1),P(1,1));
        %plot(dstr_x,dstr_p,'b')
        axis([x(1)-5,x(1)+5,0,max(dstr_p)])
        %R
        %pause(0.1)
        
        t_prev = t;
        xs(:,i) = x;
        Ps(i,:,:) = P;
end


clf
hold on
plot(t_bmp,h)
plot(t_mma(1:round(LEN/5)),xs(1,:));
plot(t_mma(1:round(LEN/5)),xs(2,:));
plot(t_mma,ay);
ylabel('m | m/s | m/s^2');
xlabel('Time (s)');
legend('Raw Pressure Altitude', 'Filtered Altitude', 'Filered Velocity', 'Acceleration')
title('Flight Profile from Skybass Altimeter')
%%
clf
plot(t_mma(1:round(LEN/5)),h(1:round(LEN/5))-xs(1,:)');
hold on
plot(t_mma(1:round(LEN/5)),xs(2,:));
plot(t_mma,ay);

