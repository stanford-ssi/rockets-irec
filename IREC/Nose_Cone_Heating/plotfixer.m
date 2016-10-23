%Plot Fixer
%Written by: Matt Svrcek  12/05/2001 Edited by: William Greenbaum 10/22/2013

%Run this script after generating the raw plots.  It will find
%all open figures and adjust line sizes and text properties.

%Change the following values to suit your preferences.  The variable
%names and comments that follow explain what each does and their options.

plotlsize = 1.2; %thickness of plotted lines, in points
axislsize = 1; %thickness of tick marks and borders, in points
markersize = 10;  %size of line markers, default is 6

%font names below must exactly match your system's font names
%check the list in the figure pull down menu under Tools->Text Properties
%note, the script editor does not have all the fonts, so use the figure menu

axisfont = 'Helvetica'; %changes appearance of axis numbers
axisfontsize = 12;            %in points
axisfontweight = 'normal';    %options are 'light' 'normal' 'demi' 'bold'
axisfontitalics = 'normal';   %options are 'normal' 'italic' 'oblique'

legendfont = 'Helvetica'; %changes text in the legend
legendfontsize = 12;
legendfontweight = 'normal';
legendfontitalics = 'normal';

labelfont = 'Helvetica';  %changes x, y, and z axis labels
labelfontsize = 14;
labelfontweight = 'normal';
labelfontitalics = 'normal';

titlefont = 'Helvetica';  %changes title
titlefontsize = 14;
titlefontweight = 'normal';
titlefontitalics = 'normal';

textfont = 'Helvetica';   %changes text
textfontsize = 12;
textfontweight = 'normal';
textfontitalics = 'normal';


%stop changing things below this line
%----------------------------------------------------
figureh = findobj('Type', 'figure');
axesh = findobj('Type', 'axes'); 
lineh = findobj(axesh, 'Type', 'line');
axestexth = findobj(axesh, 'Type', 'text');

set(lineh, 'LineWidth', plotlsize)
set(lineh, 'MarkerSize', markersize)
set(axesh, 'LineWidth', axislsize)
set(axesh, 'FontName', axisfont)
set(axesh, 'FontSize', axisfontsize)
set(axesh, 'FontWeight', axisfontweight)
set(axesh, 'FontAngle', axisfontitalics)
set(axestexth, 'FontName', textfont)
set(axestexth, 'FontSize', textfontsize)
set(axestexth, 'FontWeight', textfontweight)

% Edit Labels
for(i = 1:1:size(axesh))

    curAx = axesh(i);
%     (strcmpi(get(axesh(i), 'tag'), 'legend') == false);
    if (strcmpi(get(axesh(i), 'tag'), 'legend') == false)
        set(get(curAx,'XLabel'), 'FontName', labelfont)
        set(get(curAx,'XLabel'), 'FontSize', labelfontsize)
        set(get(curAx,'XLabel'), 'FontWeight', labelfontweight)
        set(get(curAx,'XLabel'), 'FontAngle', labelfontitalics)
        set(get(curAx,'YLabel'), 'FontName', labelfont)
        set(get(curAx,'YLabel'), 'FontSize', labelfontsize)
        set(get(curAx,'YLabel'), 'FontWeight', labelfontweight)
        set(get(curAx,'YLabel'), 'FontAngle', labelfontitalics)
        set(get(curAx,'ZLabel'), 'FontName', labelfont)
        set(get(curAx,'ZLabel'), 'FontSize', labelfontsize)
        set(get(curAx,'ZLabel'), 'FontWeight', labelfontweight)
        set(get(curAx,'ZLabel'), 'FontAngle', labelfontitalics)
        set(get(curAx,'Title'), 'FontName', titlefont)
        set(get(curAx,'Title'), 'FontSize', titlefontsize)
        set(get(curAx,'Title'), 'FontWeight', titlefontweight)
        set(get(curAx,'Title'), 'FontAngle', titlefontitalics)
    else
        set(curAx, 'FontName', labelfont)
        set(curAx, 'FontSize', labelfontsize)
        set(curAx, 'FontWeight', labelfontweight)
        set(curAx, 'FontAngle', labelfontitalics)
    end
 
end
