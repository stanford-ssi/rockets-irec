Hi! You may be asking yourself, "How do?" or "Why do I exist?", so I'll point you towards the rest of this document for those answers.


Make sure your openRocket simulation is up to date before freaking out if matlab and openrocket don't match (mass numbers should be consistent across simulations)!!

Adding a motor
------------------
If you would like to add a motor (.eng file from thrustcurve.org), please make sure the beginning of time = 0.

Example:

; AeroTech M1939W
; converted from TMT test stand data 1998 (www.tripoli.org)
; provided by ThrustCurve.org (www.thrustcurve.org)
M1939W 98 732 0 5.656 8.98822 AT
   0     0
   0.134 1905.185
etc

Adding an OpenRocket sim
------------------
Export the csv with all the variables selected and turn off "Include simulation description" and "Include flight events".
Name the csv the same as the matching motor file from earlier.

Example csv name:     M2500.csv


Comments
------------------
If you're confused or want to help, hit Ian Gomez up on slack or email at iangomez@stanford.edu
And yes I wrote that stupidly long temperature ifelse statement. It sucked but mach number works now, so whatever.