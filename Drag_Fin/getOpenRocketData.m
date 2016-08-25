% getOpenRocketData
% Grabs the matching flight data from a previously simulation in OpenRocket
% Ian Gomez,  08/25/16

function [openRocket] = getOpenRocketData(motor)

openRocket.csv_name = strcat(motor.name,'.csv');

% Enter OpenRocket data folder (You need to start from the Drag_Fin folder 
% as your directory, or this will not work)
cd('OpenRocket_Comparison_Data');

if exist(openRocket.csv_name,'file')
    
    % get the headers + read in data
    fileID = fopen(openRocket.csv_name); 
    headers = fgets(fileID); headers = strsplit(headers,',');
    fclose(fileID);
    data = csvread(openRocket.csv_name,1,0); 
    cd ..
    
    % Store data in useful way
    openRocket.headers  = headers;
    openRocket.t        = data(:,1);  % t
    openRocket.h        = data(:,2);  % h
    openRocket.v_vert   = data(:,3);  % m/s
    openRocket.a_vert   = data(:,4);  % m/s^2
    openRocket.v_total  = data(:,5);  % m/s
    openRocket.a_total  = data(:,6);  % m/s
    openRocket.T        = data(:,29); % N
    openRocket.drag     = data(:,30); % N
    openRocket.Cd       = data(:,31);
    openRocket.P        = data(:,51); % mbar
    openRocket.sp_sound = data(:,52); % m/s    
else
    cd ..
    error(strcat('CSV file not found. Check directory for ',motor.name))
end

end

