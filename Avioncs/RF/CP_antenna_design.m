function [eggbeater, QFH] = CP_antenna_design(frequency, velocity_factor)
% Logan Herrera
% Stanford Student Space Initiative - IREC
% 
% This MATLAB script designs two types of circularly polarized antennas
% designed for satellite communications. The gain pattern is roughly
% hemispherical. -3dB beamwidths are ~120 degrees and ~140 degrees for 
% eggbeater and QFH antennas respectively.
%
% Calculates parameters for ON6WG / F5VIF eggbeater antenna (eggbeater)
% Calculates parameters for W3KH Quadrafilar Helix Antenna (QFH)
%
% frequency is center frequency in MHz
% velocity factor is phasing coax velocity factor
% 
% eggbeater assumes 93 ohm impedance phasing line coax 
% QFH assumes 50 ohm impedance phasing line coax
%
% Returns two design structs with complete mechanical description
% 
% Units:    length = [ft] unless struct otherwise indicates
%           time = [s] seconds
%           wire = [in or AWG] wiretable contains AWG and inch table
%
% References:
% http://on6wg.pagesperso-orange.fr/Doc/Antenne%20Eggbeater-Engl-Part1-Full.pdf
% 
% http://www.rish.kyoto-u.ac.jp/digitalbeacon/information/Building_QFH_Antenna_Guide.pdf
% AUG 1996 QST - "The W3KH Quadrafilar Helix Antenna" - p30-34
% JUN 1999 QST - "Feedback" - p78
% SEP 1999 QST - "Feedback" - p80


c = 9.836e2; % megafeet / second
wavelength = c / frequency; % feet
wiretable = [1,0.2893; 2,0.2576; 3,0.2294; 4,0.2043; 5,0.1819; 6,0.162; 7,0.1443; 8,0.1285; 9,0.1144; 10,0.1019; 11,0.0907; 12,0.0808; 13,0.072; 14,0.0641; 15,0.0571; 16,0.0508; 17,0.0453; 18,0.0403; 19,0.0359; 20,0.032; 21,0.0285; 22,0.0253; 23,0.0226; 24,0.0201; 25,0.0179; 26,0.0159; 27,0.0142; 28,0.0126; 29,0.0113; 30,0.01; 31,0.0089; 32,0.008; 33,0.0071; 34,0.0063; 35,0.0056; 36,0.005; 37,0.0045; 38,0.004; 39,0.0035; 40,0.0031;];

eggbeater.wavelength = wavelength;
eggbeater.wirelength = 1005 / frequency;
eggbeater.phasinglength = 246 * velocity_factor / frequency;
eggbeater.groundplane.spacing = 1/8 * wavelength;
eggbeater.groundplane.radius = 1/4 * wavelength;
eggbeater.groundplane.maxopening = 1/20 * wavelength;           % feet; made up rule of thumb from me

QFH.wavelength = wavelength;
QFH.bigloop.height = 0.26 * wavelength;
QFH.bigloop.diameter = 0.173 * wavelength;
QFH.bigloop.length = 0.560 * wavelength;
QFH.smallloop.height = 0.238 * wavelength;
QFH.smalloop.diameter = 0.156 * wavelength;
QFH.smalloop.length = 0.508 * wavelength;
QFH.conductordiameter.inches = 0.0088 * wavelength * 12;
QFH.conductordiameter.millimeters = QFH.conductordiameter.inches * 25.4;

[~, index] = min(abs(wiretable(:,2) - QFH.conductordiameter.inches));
QFH.conductordiameter.AWG = wiretable(index, 1);
