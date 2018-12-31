fse_names

This application modifies your apt.dat to append the closest fse airport in brackets to the name of the x-plane airport. Published under the GPL V3 license. 

I do not take any reponsibility for damages caused by bugs in this software, so back-up your files before using it!

How to use this program:

1. Copy the contents of this zip file (fse_names.exe + icaodata.csv) into the directory of the apt.dat

2. Create a backup by renaming your apt.dat to apt.old

3. Run fse_names.exe

Windows only. 
Linux and Mac users will have to compile this software manually. I do not use any additional libs, so it should be straight forward.
You can download the source code here: https://github.com/kristian80/fse_names

X-plane has multiple apt.dat files.

Original apt.dat files:
"X-Plane 11\Resources\default scenery\default apt dat\Earth nav data"
"X-Plane 11\Custom Scenery\Global Airports\Earth nav data"

Custom Scenery apt.dat files:
"X-Plane 11\Custom Scenery\YOUR-SCENERY\Earth nav data"

Not all scenery folders contain an apt.dat file. Only those containing additional airports or heliports, such as the prefab libraries, contain one.