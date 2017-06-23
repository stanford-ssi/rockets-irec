fname = 'gps_test_30kft.log';
F = fopen(fname);

nmea_str = fgetl(F);
keyword = '$GNGGA';
data = cell(1);
i = 0;

while(ischar(nmea_str))
    if(~isempty(strfind(nmea_str,keyword)))
        A = strsplit(nmea_str,',','CollapseDelimiters', false);
        i = 1+i;
        data{i} = A;
    end
    nmea_str = fgetl(F);
end

lock_info = zeros(i,1);
alts = zeros(i,1);
errors = zeros(i,1);
for j = 1:i
   dat = data{j};
   lock_info(j) = str2double(dat{7});
   if(lock_info(j))
       alts(j) = str2double(dat{10});
       errors(j) = str2double(dat{9});
   end
end

plot(alts)
xlabel('Time (s)')
ylabel('Altitude (m)')
title('GPS altidude and lock info for 30kft test')
