% getPlots
% Plots everything
% Ian Gomez,  08/23/16

function getPlots(plot_options,time,t,t_powered,mach1,gravityloss,T,dragloss,...
    parachutedrag,droguedrag,h,u,a,altitude,motor,dragfin,g,openRocket)

linesize = 2;

plot_landing       = plot_options(1);
plot_thrust        = plot_options(2);
plot_h_u_a         = plot_options(3);
plot_combined_hu   = plot_options(4);
plot_h             = plot_options(5);
plot_forces        = plot_options(6);
plot_recovery_drag = plot_options(7);
plot_openRocket    = openRocket.retrieve;

% If an error occurs at t_land, you need to let the sim run longer
time.xlim = time.apogee;
if plot_landing == 1; time.xlim = time.land; end
xlimit = [0 time.xlim];  % plots up to the specified limits

% Makes apogee label
apogee_label_dim = [.4 .3 .6 .1];
apogee_label_str = strcat(strcat(strcat({'Matlab Apogee = '},num2str(max(h)))),...
    'm');
% Plot titles
if dragfin.deploy_t > 0
    h_title = strcat(strcat(strcat({'Altitude ('},motor.name),')',...
        {' -Fins Deployed'}));
    u_title = strcat(strcat(strcat({'Velocity ('},motor.name),')',...
        {' -Fins Deployed'}));
    a_title = strcat(strcat(strcat({'Acceleration ('},motor.name),')',...
        {' -Fins Deployed'}));
    force_title = strcat(strcat(strcat('In-Flight Forces (',motor.name,...
        ')',{' -Fins Deployed'})));  
else
    h_title = strcat(strcat(strcat({'Altitude ('},motor.name),')'));
    u_title = strcat(strcat(strcat({'Velocity ('},motor.name),')'));
    a_title = strcat(strcat(strcat({'Acceleration ('},motor.name),')'));
    force_title = strcat(strcat('In-Flight Forces (',motor.name,')'));
    
end


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
    if plot_openRocket
        plot(t,altitude.target.*ones(1,length(t)),'--',t,h,openRocket.t,...
            openRocket.h,'LineWidth',linesize)
        legend('3048m above launch site','SSI','OpenRocket','Location',...
            'Southwest')
    else
        plot(t,altitude.target.*ones(1,length(t)),'--',t,h,'LineWidth',...
            linesize)
        legend('3048m above launch site','Location','Southwest')
    end
    title(h_title)
    xlabel('Time (s)')
    ylabel('Height (m)')
    xlim(xlimit)
    grid on
    % label apogee
    annotation('textbox',apogee_label_dim,'String',...
        apogee_label_str,'FitBoxToText','on');
    
    figure
    hold on
    if plot_openRocket
        plot(t,mach1,'--',t,u,openRocket.t,openRocket.v_vert,'LineWidth',...
            linesize)
        legend('Mach 1','SSI','OpenRocket','Location','Southwest')
        
    else
        plot(t,mach1,'--',t,u,'LineWidth',linesize)
        legend('Mach 1','Location','Southwest')
        
    end
    title(u_title)
    xlabel('Time (s)')
    ylabel('Velocity (m/s)')
    xlim(xlimit)
    grid on
    
    figure
    if plot_openRocket
        plot(t(1:length(a)),a./g,openRocket.t,openRocket.a_vert./g,...
            'LineWidth',linesize)
        legend('SSI','OpenRocket','Location','Southwest')
    else
        plot(t(1:length(a)),a./g,'LineWidth',linesize)
    end
    title(a_title)
    xlabel('Time (s)')
    ylabel('g')
    xlim(xlimit)
    grid on
end

if plot_combined_hu == 1
    figure
    yyaxis right
    if plot_openRocket
        plot(t,h,openRocket.t,openRocket.h,'-.',t,altitude.target.*...
            ones(1,length(t)),'--','LineWidth',linesize)
        legend('SSI','OpenRocket','Location',...
            'Southeast')
    else
        plot(t,altitude.target.*ones(1,length(t)),'--',t,h,'LineWidth',...
            linesize)
    end
    title(strcat(strcat({'Altitude and Velocity ('},motor.name),')'))
    xlabel('Time (s)')
    ylabel('Height (m)')
    xlim(xlimit)
    grid on
    yyaxis left
    if plot_openRocket
        plot(t,u,openRocket.t,openRocket.v_vert,'-.',t,mach1,'--',...
            'LineWidth',linesize)
        legend('SSI','OpenRocket','Location','Southwest')
    else
        plot(t,mach1,'--',t,u,'LineWidth',linesize)
    end
    ylabel('Velocity (m/s)')
end

if plot_h == 1
    
    figure
    hold on
    if plot_openRocket
        plot(t,altitude.target.*ones(1,length(t)),'--',t,h,openRocket.t,...
            openRocket.h,'LineWidth',linesize)
        legend('3048m above launch site','SSI','OpenRocket','Location',...
            'Southwest')
    else
        plot(t,altitude.target.*ones(1,length(t)),'--',t,h,'LineWidth',...
            linesize)
        legend('3048m above launch site','Location','Southwest')
    end
    title(h_title)
    xlabel('Time (s)')
    ylabel('Height (m)')
    xlim(xlimit)
    
    grid on
    % label apogee
    annotation('textbox',apogee_label_dim,'String',...
        apogee_label_str,'FitBoxToText','on');
end

if plot_forces == 1
    figure
    plot(t,gravityloss,t_powered,T,t,dragloss,'LineWidth',linesize);
    legend('Gravity','Thrust','Drag + Drag Fins - Recovery Drag')
    hold on
    if dragfin.deploy_t > -1
        yl = get(gca,'YLim');
        plot([dragfin.deploy_t dragfin.deploy_t], yl,'black')
        txt = text(dragfin.deploy_t+0.1, yl(2)*0.9, 'Drag fin deployment');
        txt.Color = 'black';
    end
    title(force_title)
    xlabel('Time (s)')
    ylabel('Force (N)')
    xlim(xlimit)
    grid on
    hold off;
    
    if plot_openRocket
        figure
        plot(openRocket.t,openRocket.gravityloss,openRocket.t,...
            openRocket.T,openRocket.t,openRocket.drag,'LineWidth',linesize);
        legend('Gravity','Thrust','Drag')
        hold on
        title(strcat(strcat('OpenRocket In-Flight Forces (',motor.name,')')))
        xlabel('Time (s)')
        ylabel('Force (N)')
        xlim(xlimit)
        grid on
        hold off;
    end
end

if plot_recovery_drag == 1
    figure
    hold on
    plot(t,parachutedrag,'LineWidth',linesize)
    plot(t,droguedrag,'LineWidth',linesize);
    title(strcat(strcat('In-Flight Recovery Forces (',motor.name,')')))
    xlabel('Time (s)')
    ylabel('Force (N)')
    xlim(xlimit)
    grid on
    legend('Parachute','Drogue')
end

end