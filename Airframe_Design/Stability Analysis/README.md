# Damping / Dynamic Stability Analysis
## Instructions
1. Export OpenRocket simulation data to '.csv' format. (See example: 
'SSI-IREC-2017_OpenRocket.csv') Be sure to use in, lb, s
2. Create the aerodynamic data file. (See example: 
SSI-IREC-2017_AeroData.xlsx)
a. Over a range of Mach numbers, copy the aerodynamic data using the 
'Component Analysis' feature of OpenRocket. Follow the exact column format of 
the example file. Be sure to follow the specified units. 
3. Update the nozzle exit plane location value (see 'OpenRocket Inputs') in
the 'DampingCalcs.m' file. Be sure to follow the specified units. 
4. Run 'DampingCalcs.m' and follow the prompts to select the two required 
files from steps 1-2. 