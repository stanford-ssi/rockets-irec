%% LOADING FLIGHT DATA

flight_name = 'FAR_4-15-17';
rvn_fname = strcat(flight_name, '_Raven.csv');
sto_fname = strcat(flight_name, '_Strato.csv');
skb_fname = strcat(flight_name, '_Skybass\');

rvn_dat = readtable(rvn_fname);
sto_dat = csvread(sto_fname,1,0);

%ravn was .31 secs ahead
rvn_t = rvn_dat{:,{'Time'}} + .31;
rvn_h = rvn_dat{:,{'x_Altitude_Baro_Ft_AGL__'}};
rvn_h_dr = rvn_dat{:,{'x_Altitude_Accel_Ft__'}};
rvn_v = rvn_dat{:,{'x_Velocity_Accel_Ft_Sec__'}};
rvn_T = rvn_dat{:,{'Temperature_F_'}};
rvn_ay = rvn_dat{:,{'AxialAccel_Gs_'}};
rvn_vbat = rvn_dat{:,{'VoltsBattery_V_'}};

sto_t = sto_dat(:,1);
sto_h = sto_dat(:,2);
sto_v = sto_dat(:,3);
sto_T = sto_dat(:,4);
sto_vbat = sto_dat(:,5);

%% SKYBASS DATA

if ~(exist('bmp') && exist('mma'))
    bmp = csvread(strcat(skb_fname,'bmp.csv'));
    mma = csvread(strcat(skb_fname,'mma.csv'));
end

START = 714000;
LEN = 5700;
APO = 892;

skb_p = (bmp(START:START+LEN,2) + bmp(START:START+LEN,3))/2;
skb_h = (1-((skb_p)./101350).^0.190284) .* 145366.45;  %%in ft
skb_h = skb_h - skb_h(1);
skb_t_bmp = bmp(START:START+LEN,1)/1000000;
skb_t_bmp = skb_t_bmp - skb_t_bmp(1) - 0.17;


%% Acceleration analysis

skb_t_mma = mma(START:START+LEN,1);
skb_t_mma = (skb_t_mma - skb_t_mma(1))/1000000 - 0.23;
skb_ax = mma(START:START+LEN,2);
skb_ay = -mma(START:START+LEN,3);

%clean up bad samples
for i = 1:LEN
    if (skb_ax(i) < -400) || (skb_ay(i) < -400)
        skb_ax(i) = (skb_ax(i-1)+skb_ax(i+1))/2;
        skb_ay(i) = (skb_ay(i-1)+skb_ay(i+1))/2;
    end
end

skb_v_dr = zeros(APO,1);
skb_h_dr = zeros(APO,1);
for i = 2:APO
    skb_v_dr(i) = skb_v_dr(i-1) + (skb_ay(i)+skb_ay(i+1))/2 * (skb_t_mma(i+1) - skb_t_mma(i));
    skb_h_dr(i) = skb_h_dr(i-1) + (skb_v_dr(i-1)+skb_v_dr(i))/2 * (skb_t_mma(i+1) - skb_t_mma(i));
end
skb_h_dr = skb_h_dr * 3.28084;  %% convert to ft
skb_v_dr = skb_v_dr * 3.28084;  %% convert to ft/s
skb_ay = skb_ay / 9.81;   %% convert to g's  


%% Get APPO
rvn_apo = max(rvn_dat{:,'x_Altitude_Baro_Ft_AGL__'});
rvn_apo_dr = max(rvn_h_dr);
sto_apo = max(sto_dat(:,2));
rvn_apo_loc = find(rvn_h == rvn_apo);
rvn_apo_loc = rvn_apo_loc(1);
rvn_apo_t = rvn_t(rvn_apo_loc);
rvn_apo_loc_dr = find(rvn_h_dr == rvn_apo_dr);
rvn_apo_loc_dr = rvn_apo_loc_dr(1);
rvn_apo_t_dr = rvn_t(rvn_apo_loc_dr);

%% Plot h vs t forall altimeters
clf
hold on
plot(rvn_t, rvn_h);
plot(sto_t, sto_h);
plot(skb_t_bmp, skb_h);
xlim([0,120]);
ylim([-100,5000]);
xlabel('Time (s)')
ylabel('Height (ft)')
legend('Raven', 'Strato', 'Skybass')

%% Plot accel for raven vs skybass
clf
hold on
plot(rvn_t, rvn_ay);
plot(skb_t_mma, skb_ay);


%% Plot Temperature
clf
plot(rvn_t,smooth(rvn_T,300));
hold on
plot(sto_t,smooth(sto_T,10), 'r--');
vline(rvn_apo_t,'bl','apogee');
legend('Raven', 'Stratologger');
xlabel('Time(s)')
ylabel('Temperature (^oF)')
title('4-15-17 FAR launch temperature in AV Bay');
%% Raven Skybass Dead Reakonning
clf
hold on
plot(rvn_t(1:rvn_apo_loc_dr), rvn_h_dr(1:rvn_apo_loc_dr), 'b');
plot(skb_t_mma(1:APO), skb_h_dr, 'r--');
plot(rvn_t, rvn_h, 'black');
plot(skb_t_bmp, skb_h, 'black');
legend('Raven', 'Skybass', 'Barro Data');
ylabel('Height (ft)');
ylim([-100,6000])
xlim([0,20])
xlabel('Time (s)');
title('Raven Dead Reckoning vs Barrometer')

%% Rvn_VBatvsAccel
clf
subplot(2,1,1)
plot(rvn_time,rvn_dat{:,'VoltsBattery_V_'})
ylabel('Raven Voltage (V)')
xlabel('Time (s)')
subplot(2,1,2)
plot(rvn_time,rvn_dat{:,'AxialAccel_Gs_'})
ylabel('Axial Acceleration (g)')
xlabel('Time (s)')
print('Rvn_VBatvsAccel.png', '-dpng')
