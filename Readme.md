# PlanetSystem

Det är gjort med hjälp av C/C++ programmering, windows programmering (med "message queue") och opengl. 
Koden med .cpp, .h och .dlg filerna finns att titta på i mappen "Projekt\PlanetSystem\PlanetSystem". 
".dlg" filerna är dialogrutorna med inmatningsinformation för "nya planeter", "ta bort planeter" och "nytt planetsystem" (en form av "resurser"). 

I planetsystems projektet har det inte implementerats extra information, några krockar eller månar (eller "skarpt solljus"). 

Programmet simulerar skapandet av planetsystem. Här har jag använt mig av mina  fysikkunskaper, där jag antagit att accelerationen på grund av gravitationskrafter är konstant under ett kort tidsintervall dt. Gravitationskraften på en planet eller sol X fås sedan ur dragningskraften från alla solar och planeter utom X. Delar man sedan med massan för planeten eller solen X, så får man accelerationen och sedan kan hastigheten och positionen beräknas. Alla solar och planeter får utslumpade hastigheter och positioner i början av simuleringen.

Jag har gjort programmeringen i Visualstudio 6 och sedan Visualstudio 2008. 

## För att testköra

Testkör programmet från mappen (kör "PlanetSystem.exe" - ska fungera i till exempel windows 10):
"Projekt\PlanetSystem\PlanetSystem\Debug\PlanetSystemDebug"
eller
"Projekt\PlanetSystem\PlanetSystem\Release\PlanetSystemRelease"