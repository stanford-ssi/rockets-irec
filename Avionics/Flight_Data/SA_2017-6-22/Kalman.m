flight_name = 'SA_2017-6-22';
fname = strcat(flight_name, '_Skybass/');

START = 530000;
APO = 5000;
END = 548000;
LEN = END - START;

if ~(exist('bmp') && exist('mma') && exist('gps'))
    bmp = csvread(strcat(fname,'bmp.csv'));
    bmp = bmp(START:START+LEN,:);
    mma = csvread(strcat(fname,'mma.csv'));
    mma = mma(START:START+LEN,:);
    %gps = csvread(strcat(fname,'gps.csv'));

end

%%
p = (bmp(:,2) + bmp(:,4))/2;
h_ft = (1-((p)./101350).^0.190284) .* 145366.45;  %%in 
h_ft = h_ft - h_ft(1);
h = (h_ft)* 0.3048; %in m
t_bmp = bmp(:,1);
t_offset = t_bmp(1);
t_bmp = t_bmp - t_offset;
t_bmp = t_bmp/10000000;

%shift bmp data to match MMA
t_bmp = t_bmp(5:end);
h = h(5:end);
h_ft = h_ft(5:end);


plot(t_bmp,h_ft)
title('FAR 2017-5-20 Launch Barometric Altitude')
ylabel('Altitude (ft)')
xlabel('Time (s)')

%%

ax = mma(:,2);
ay = mma(:,3);
t_mma = mma(:,1) - t_offset;
t_mma = t_mma/10000000;

%clean up bad samples
for i = 1:LEN
    if (ax(i) < -400) || (ay(i) < -400)
        ax(i) = (ax(i-1)+ax(i+1))/2;
        ay(i) = (ay(i-1)+ay(i+1))/2;
    end
end

v_dr = zeros(APO,1);
h_dr = zeros(APO,1);
for i = 2:APO
    v_dr(i) = v_dr(i-1) + (ay(i)+ay(i+1))/2 * (t_mma(i+1) - t_mma(i));
    h_dr(i) = h_dr(i-1) + (v_dr(i-1)+v_dr(i))/2 * (t_mma(i+1) - t_mma(i));
end
plot(v_dr/343)
xlabel('Time (s)')
ylabel('Mach')


%%

FLT_LEN = round(LEN/10);

%--STATE MATRICIES--
x = zeros(3,1);
F = zeros(3,3);
H = [1,0,0;0,0,1];
R = [100,0,;0,1];
Q = 1e-4*[1,1,0;1,2,0;0,0,1000];
P = zeros(3,3);

%--HISTORIES-
Ps = zeros(FLT_LEN,3,3);
xs = zeros(3,FLT_LEN);
appo_det = zeros(1,FLT_LEN);
p_hats = [];
Rs = [];
t_prev = 0;
clf
hold on
for i = 1:FLT_LEN
        %--predict-- 
        t = t_mma(i);
        dt = t - t_prev;
        F = [1, dt, (dt^2)/2 ; 0, 1, dt; 0,0,1];
        x_p = F*x;
        P_p = F*P*F' + Q;
        
        %--update--
        %-Measurement Varience on altitude is a funciton of V and A
        if x_p(2) > 343*(.7) 
            R(1,1) = inf;
        else
            R(1,1) = abs(x_p(2)) + abs(x_p(3))*0.5+10;
            if R(1,1) > 500
                R(1,1) = 500;
            end
            Rs = [Rs,R(1,1)];
        end
        %-Value Rejection on Altitude
        p_hat = min(cdf('Normal',h(i),x_p(1),R(1,1)),cdf('Normal',h(i),x_p(1),R(1,1),'upper'));
        p_hats = [p_hats, p_hat];
        if(p_hat < 0.01)
            h(i) = x_p(1);
        end
        K = P_p * H' * inv(H*P_p*H' + R);
        x = x_p + K*([h(i);ay(i)] - H*x_p);
        P = (eye(3) - K*H)*P_p;
        
        dstr_x = (x(1)-10):0.1:(x(1)+10); 
        dstr_p = normpdf(dstr_x,x(1),P(1,1));
        %plot(dstr_x,dstr_p,'b')
        axis([x(1)-5,x(1)+5,0,max(dstr_p)])

        t_prev = t;
        xs(:,i) = x;
        Ps(i,:,:) = P;
end


clf
hold on
plot(t_bmp,h)
plot(t_mma(1:FLT_LEN),xs(1,:));
plot(t_mma(1:FLT_LEN),xs(2,:));
plot(t_mma,ay);
plot((1:length(v_dr))/50,h_dr);
ylabel('m | m/s | m/s^2');
xlabel('Time (s)');
legend('Raw Pressure Altitude', 'Filtered Altitude', 'Filered Velocity', 'Acceleration')
title('Flight Profile from Skybass Altimeter')

%% 
h_diffs = xs(1,:) - h(1:length(xs))';
plot(xs(2,:), (h_diffs).^2, 'b*');
%%
clf
hold on
plot(t_mma(1:FLT_LEN),xs(2,:)/343);
plot((1:length(v_dr))/50,v_dr/343);
xlabel('Time (s)')
ylabel('Mach')
title('FAR 2017-5-20 Flight Velocity')



