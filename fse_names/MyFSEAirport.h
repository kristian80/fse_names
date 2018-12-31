#pragma once
#include <string>
#include <vector>
class MyFSEAirport
{
public:
	std::vector<std::string> fse_icao;
	std::vector<double> fse_lat;
	std::vector<double> fse_long;


	MyFSEAirport();
	~MyFSEAirport();
};

