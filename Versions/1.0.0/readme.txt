fse_names, by Kristian80

FSEconomy is an MMO that is based on flight simulation. As FSEconomy is a static world and the airports included are based on FS9, it does not contain all airports that are part of x-plane today. 
Therefore, I wrote this program that modifies your apt.dat to append the closest FSEconomy airports in brackets to your x-plane airport name. 
This way, you can see which airport FSEconomy will consider before actually loading the airports and starting the FSEconomy client.
You can also search within x-plane for an FSEconomy airport and instantly see which x-plane airports are feasible. 

Published under the GPL V3 license. This does not guarantee a bug free software and you yourself are fully responsible to backup your data before using this software.

How it works:

For each airport/heliport/seeport in the apt.dat file, I search for the location of runways, heliports and seeports. If I find one, I use the last one as the airport coordinates. 
Then, I calculate the distance for all FSE Airports, which have their integer coordinates within +/- 2 degrees of the latitude and longitude of the x-plane airports.
I sort the FSE airports according to their integer coordinates first, which results in a pretty good performance and the conversion of the main apt.dat file (230MB) took less than 5min on my computer.

How to use this program:

1. Copy the contents of this zip file (fse_names.exe + icaodata.csv) into the directory of the apt.dat

2. Create a backup by renaming your apt.dat to apt.old

3. Run fse_names.exe

Windows only. 
Linux and Mac users will have to compile this software manually. I do not use any additional libs, so it should be straight forward.
You can download the source code here: https://github.com/kristian80/fse_names

You need to run this software in each directory that contains an apt.dat file. Be aware that x-plane contains multiple apt.dat files.

Original apt.dat files:
"X-Plane 11\Resources\default scenery\default apt dat\Earth nav data"
"X-Plane 11\Custom Scenery\Global Airports\Earth nav data"

Custom Scenery apt.dat files:
"X-Plane 11\Custom Scenery\YOUR-SCENERY\Earth nav data"

Not all scenery folders contain an apt.dat file. Only those containing additional airports or heliports, such as the prefab libraries, contain one.
