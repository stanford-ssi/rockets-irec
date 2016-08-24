% getPlots
% Plots everything
% Ian Gomez,  08/23/16

function getPlots(plot_options,time,t,t_powered,mach1,gravityloss,T,dragloss,...
    parachutedrag,droguedrag,h,u,a,altitude,motor,dragfin,g)

linesize = 2; 
plot_landing       = plot_options(1); 
plot_thrust        = plot_options(2); 
plot_h_u_a         = plot_options(3); 
plot_combined_hu   = plot_options(4); 
plot_h             = plot_options(5);
plot_forces        = plot_options(6); 
plot_recovery_drag = plot_options(7); 

% If an error occurs at t_land, you need to let the sim run longer
time.xlim = time.apogee;
if plot_landing == 1; time.xlim = time.land; end
xlimit = [0 time.xlim];  % plots up to the specified limits

% Makes apogee label
apogee_label_dim = [.4 .3 .6 .1];
apogee_label_str = strcat(strcat(strcat({'Apogee = '},num2str(max(h)))),'m');

% Plot the thrust data
if plot_thrust == 1
    figure
    plot(t_powered,T,'-mo','LineWidth',linesize)
    xlim([0 t_powered(end)])
    thrust_title = strcat({'Thrust Curve of '},motor.name);
    title(thrust_title)
    xlabel('Time (s)')
    ylabel('Thrust (N)')
    grid on
end

if plot_h_u_a == 1
    figure
    hold on
    plot(t,altitude.target.*ones(1,length(t)),'--','LineWidth',linesize)
    plot(t,h,'LineWidth',linesize)
    title(strcat(strcat({'Altitude ('},motor.name),')'))
    xlabel('Time (s)')
    ylabel('Height (m)')
    xlim(xlimit)
    grid on
    legend('3048m above launch site','Location','Southwest')
    % label apogee
    annotation('textbox',apogee_label_dim,'String',...
        apogee_label_str,'FitBoxToText','on');
    
    figure
    hold on
    plot(t,mach1,'--','LineWidth',linesize)
    plot(t,u,'LineWidth',linesize)
    title(strcat(strcat({'Velocity ('},motor.name),')'))
    xlabel('Time (s)')
    ylabel('Velocity (m/s)')
    xlim(xlimit)
    grid on
    legend('Mach 1','Location','Southwest')
    
    figure
    plot(t(1:length(a)),a./g,'LineWidth',linesize)
    title(strcat(strcat({'Acceleration ('},motor.name),')'))
    xlabel('Time (s)')
    ylabel('g')
    xlim(xlimit)
    grid on
end

if plot_combined_hu == 1
    figure
    yyaxis right
    plot(t,h,t,altitude.target.*ones(1,length(t)),'LineWidth',linesize)
    title(strcat(strcat({'Altitude and Velocity ('},motor.name),')'))
    xlabel('Time (s)')
    ylabel('Height (m)')
    xlim(xlimit)
    grid on
    yyaxis left
    plot(t,u,t(1:length(mach1)),mach1,'LineWidth',linesize)
    ylabel('Velocity (m/s)')
end

if plot_h == 1
    
    figure
    hold on
    y1=get(gca,'ylim');
    plot(t,altitude.target.*ones(1,length(t)),'--','LineWidth',linesize)
    plot(t,h,'LineWidth',linesize)
    plot([t_powered(end) t_powered(end)],y1)
    title(strcat(strcat({'Altitude ('},motor.name),')'))
    xlabel('Time (s)')
    ylabel('Height (m)')
    xlim(xlimit)
    
    grid on
    legend('3048m above launch site','Location','Southwest')
    % label apogee
    annotation('textbox',apogee_label_dim,'String',...
        apogee_label_str,'FitBoxToText','on');
end

if plot_forces == 1
    figure
    plot(t,gravityloss,t_powered,T,t,dragloss,'LineWidth',linesize);
    legend('Gravity','Thrust','Drag + Drag Fins')
    hold on
    if dragfin.deploy_t > -1
        yl = get(gca,'YLim');
        plot([dragfin.deploy_t dragfin.deploy_t], yl,'black')
        txt = text(dragfin.deploy_t+0.1, yl(2)*0.9, 'Drag fin deployment');
        txt.Color = 'black';
    end
    title('In-Flight Forces')
    xlabel('Time (s)')
    ylabel('Force (N)')
    xlim(xlimit)
    grid on
    hold off;
end

if plot_recovery_drag == 1
    figure
    hold on
    plot(t,parachutedrag,'LineWidth',linesize)
    plot(t,droguedrag,'LineWidth',linesize);
    title('In-Flight Forces')
    xlabel('Time (s)')
    ylabel('Force (N)')
    xlim(xlimit)
    grid on
    legend('Parachute','Drogue')
end

end