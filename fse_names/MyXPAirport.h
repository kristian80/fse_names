#pragma once
#include <string>
#include <vector>
class MyXPAirport
{
	std::vector<std::string> xp_icao;
	std::vector<std::string> fse_icao;
	std::vector<double> xp_lat;
	std::vector<double> xp_long;
public:
	MyXPAirport();
	~MyXPAirport();
};

