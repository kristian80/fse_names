// fse_names.cpp : 
// Developer: Kristian80
// License: GPL V3

// This is just a quick and dirty implementation, so do not expect nice code

#include "pch.h"
#include "MyFSEAirport.h"
#include "MyXPAirport.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

// This value defines how many long/lat patches are considered in NOSW directions
// 2 (eqals 25 patches) gives only one invalid airport for prefab africa, so it should be good enough

#define MAX_APT_SEARCH 2
#define MAX_APT_ARRAY (((2*MAX_APT_SEARCH)+1) * ((2*MAX_APT_SEARCH)+1))

// Calculate the distance between two coordinates.
// Still implemented to hold all temporary values, not nice, but it does not affect performance

double calc_distance(double lat1, double long1, double lat2, double long2)
{
	lat1 = lat1 * M_PI / 180;
	long1 = long1 * M_PI / 180;
	lat2 = lat2 * M_PI / 180;
	long2 = long2 * M_PI / 180;

	double rEarth = 6372.797;

	double dlat = lat2 - lat1;
	double dlong = long2 - long1;

	double x1 = sin(dlat / 2);
	double x2 = cos(lat1);
	double x3 = cos(lat2);
	double x4 = sin(dlong / 2);

	double x5 = x1 * x1;
	double x6 = x2 * x3 * x4 * x4;

	double temp1 = x5 + x6;

	double y1 = sqrt(temp1);
	double y2 = sqrt(1.0 - temp1);

	double temp2 = 2 * atan2(y1, y2);

	double rangeKm = temp2 * rEarth;

	double CalcRange = rangeKm * 0.539957;

	return CalcRange;
}

// Find the closest fse airport 

std::string find_fse_airport(MyFSEAirport *fse_array, double apt_lat, double apt_long)
{
	int pos_lat = 90 + ((int)apt_lat);
	int pos_long = 180 + ((int)apt_long);

	std::string store_icao[MAX_APT_ARRAY];
	double store_distance[MAX_APT_ARRAY];

	for (int index = 0; index < MAX_APT_ARRAY; index++)
	{
		store_icao[index] = "NONE";
		store_distance[index] = 50000;
	}

	int act_lat = pos_lat;
	int act_long = pos_long;
	int store_pos = 0;

	for (int lat_index = -MAX_APT_SEARCH; lat_index <= MAX_APT_SEARCH; lat_index++)
	{
		int act_lat = pos_lat + lat_index;
		if (act_lat < 0) act_lat += 180;


		for (int long_index = -MAX_APT_SEARCH; long_index <= MAX_APT_SEARCH; long_index++)
		{
			int act_long = pos_long + long_index;
			if (act_long < 0) act_long += 360;

			for (int index = 0; index < fse_array[act_lat * 360 + act_long].fse_icao.size(); index++)
			{
				double act_distance = calc_distance(apt_lat, apt_long, fse_array[act_lat * 360 + act_long].fse_lat[index], fse_array[act_lat * 360 + act_long].fse_long[index]);

				if (act_distance < store_distance[store_pos])
				{
					store_distance[store_pos] = act_distance;
					store_icao[store_pos] = fse_array[act_lat * 360 + act_long].fse_icao[index];
				}
			}

			store_pos++;

		}
	}

	for (int index = 1; index < MAX_APT_ARRAY; index++)
	{
		if (store_distance[index] < store_distance[0])
		{
			store_distance[0] = store_distance[index];
			store_icao[0] = store_icao[index];
		}
	}

	return store_icao[0];

}

// remove entry from apt string

std::string remove_entry(std::string line_string, std::string delimiter)
{
	int pos;
	
	if ((pos = line_string.find(delimiter)) != std::string::npos)
		line_string.erase(0, pos + delimiter.length());

	size_t str_start = line_string.find_first_not_of(" \t");
	if (str_start != std::string::npos)
		line_string = line_string.substr(str_start);

	return line_string;
}

// get first entry from apt string

std::string get_entry(std::string line_string, std::string delimiter)
{
	int pos;

	std::string entry = "";

	if ((pos = line_string.find(delimiter)) != std::string::npos)
		entry = line_string.substr(0, pos);

	size_t str_start = line_string.find_first_not_of(" \t");
	if (str_start != std::string::npos)
		line_string = line_string.substr(str_start);

	return entry;
}

// main: do all the stuff

int main()
{
	std::string fse_line;
	std::ifstream fse_file("icaodata.csv");
	std::string fse_delimiter = ",";
	size_t pos = 0;
	
	// Airports are sorted according to coordinates
	// Typically searching is the slowest part of a job like this one
	// 130k entries is nothing for todays memory

	// Uh, and when starting the implementation I forgot that lat is -90 to 90 (not fixed yet, this is not the kind of project where I care about quality).
	//                                         lat, long
	MyFSEAirport *fse_array = new MyFSEAirport[360*360];

	//MyXPAirport *xp_array = new MyXPAirport[255*255];

	if (fse_file.is_open())
	{
		std::cout << "icaodata.csv opened successfully\n";



		while (!fse_file.eof())
		{
			getline(fse_file, fse_line);

			
			std::string apt_icao = "";
			std::string apt_lat = "";
			std::string apt_long = "";
			double apt_longitude = 0;
			double apt_latitude = 0;
			int pos_long = 0;
			int pos_lat = 0;

			if ((pos = fse_line.find(fse_delimiter)) != std::string::npos)
			{ 
				apt_icao = fse_line.substr(0, pos);
				fse_line.erase(0, pos + fse_delimiter.length());
			}
			
			// Check for header line
			if ((apt_icao.compare("icao") != 0) && (pos != std::string::npos))
			{
				if ((pos = fse_line.find(fse_delimiter)) != std::string::npos)
				{
					apt_lat = fse_line.substr(0, pos);
					apt_latitude = std::stof(apt_lat);
					pos_lat = 90 + ((int) apt_latitude);
					fse_line.erase(0, pos + fse_delimiter.length());
				}

				if ((pos = fse_line.find(fse_delimiter)) != std::string::npos)
				{
					apt_long = fse_line.substr(0, pos);
					apt_longitude = std::stof(apt_long);
					pos_long = 180 + ((int) apt_longitude);
					fse_line.erase(0, pos + fse_delimiter.length());
				}
				 
				fse_array[pos_lat * 360 + pos_long].fse_icao.push_back(apt_icao);
				fse_array[pos_lat * 360 + pos_long].fse_lat.push_back(apt_latitude);
				fse_array[pos_lat * 360 + pos_long].fse_long.push_back(apt_longitude);


				//std::cout << apt_icao << ";" << apt_latitude << ";" << apt_longitude << "\n";
				//std::cin.ignore();
			}

			
		}
		std::cout << "Finished loading icaodata.csv\n";
		fse_file.close();


		std::string icao_act = "FYWK";
		double lat_act = -22.43130000;
		double long_act = 14.46670000;
		
		std::string closest_icao = "XXXX";
		//closest_icao = find_fse_airport(fse_array, lat_act, long_act);
		//std::cout << "Closest FSE ICAO: " << closest_icao << "\n";




		std::ifstream apt_file_in("apt.old");
		std::ofstream apt_file_out;
		
		std::string apt_delimiter = " ";
		std::string apt_line;

		if (apt_file_in.is_open())
		{
			std::cout << "apt.old opened successfully\n";

			std::vector<std::string> apt_in;
			std::vector<std::string> apt_out;
			
			while (!apt_file_in.eof())
			{
				getline(apt_file_in, apt_line);
				apt_in.push_back(apt_line);
			}
			apt_file_in.close();



			std::string icao_act = "X-NONE-X";
			double lat_act = 0;
			double long_act = 0;
			int out_index = 0;
			std::string line_code;
			std::string line_out;
			std::string closest_fse_icao = "NONE";
			int in_index = 0;

			for (in_index = 0; in_index < apt_in.size(); in_index ++)
			{

				apt_line = apt_in[in_index];
				
				line_code = get_entry(apt_line, apt_delimiter);
				apt_line = remove_entry(apt_line, apt_delimiter);

				if ((line_code.compare("1") == 0) || (line_code.compare("16") == 0) || (line_code.compare("17") == 0))
				{
					// Remove 3 entries
					apt_line = remove_entry(apt_line, apt_delimiter);
					apt_line = remove_entry(apt_line, apt_delimiter);
					apt_line = remove_entry(apt_line, apt_delimiter);


					std::string icao_new = get_entry(apt_line, apt_delimiter);

					if (icao_act.compare("X-NONE-X") != 0)
					{
						std::string closest_icao = "NA";
											
						if ((lat_act != 0) && (long_act != 0))
							closest_icao = find_fse_airport(fse_array, lat_act, long_act);

						line_out = apt_in[out_index++];
						line_out += "[";
						line_out += closest_icao;
						line_out += "]";

						apt_out.push_back(line_out);
					}

					while (out_index < in_index) 
					{
						line_out = apt_in[out_index++];
						apt_out.push_back(line_out);
					}

					icao_act = icao_new;
					lat_act = 0;
					long_act = 0;

					// Find closest airport here


				}

				// Runway, Waterway, Helipad, Startup location
				if ((line_code.compare("100") == 0) || (line_code.compare("101") == 0) || (line_code.compare("102") == 0) || (line_code.compare("15") == 0))
				{
					int remove_entries = 0;

					if (line_code.compare("100") == 0) 
						remove_entries = 8;
					if (line_code.compare("101") == 0) 
						remove_entries = 3;
					if (line_code.compare("102") == 0) 
						remove_entries = 1;
					if (line_code.compare("15") == 0) 
						remove_entries = 0;

					for (; remove_entries > 0; remove_entries--)
						apt_line = remove_entry(apt_line, apt_delimiter);


					std::string lat_s = get_entry(apt_line, apt_delimiter);
					apt_line = remove_entry(apt_line, apt_delimiter);

					std::string long_s = get_entry(apt_line, apt_delimiter);
					apt_line = remove_entry(apt_line, apt_delimiter);

					lat_act = std::stof(lat_s);
					long_act = std::stof(long_s);
				}

				
			}

			// Get last ICAO
			if (icao_act.compare("X-NONE-X") != 0)
			{
				std::string closest_icao = "NA";

				if ((lat_act != 0) && (long_act != 0))
					closest_icao = find_fse_airport(fse_array, lat_act, long_act);

				line_out = apt_in[out_index++];
				line_out += "[";
				line_out += closest_icao;
				line_out += "]";

				apt_out.push_back(line_out);
			}
			
			// Get last lines
			while (out_index < in_index)
			{
				line_out = apt_in[out_index++];
				apt_out.push_back(line_out);
			}

			// Write output file
			apt_file_out.open("apt.dat");
			
			for (out_index = 0; out_index < apt_out.size(); out_index++)
			{
				apt_file_out << apt_out[out_index] << "\n";
			}

			apt_file_out.close();

			std::cout << "Computation Finished\n";

		}
		else
		{
			std::cout << "Unable to open apt.old. Rename your apt.dat to apt.old first.\n";
		}

	}
	else std::cout << "Unable to open icaodata.csv\n";

	std::cin.ignore();

}

