#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

enum senderName { Enzo = 7, MikelServer = 6, MikelWater = 5};

class Parser
{
private:
	bool parseComplete;

	int senderCode;
	int applicationCode;
	int temperature;
	int waterCode;
	int wantedTemperature;

	std::string SSID;
	std::string PASS;
	std::string OLDIP;
	std::string NEWIP;


	void CheckTrame(std::string trame);
	void Parse(std::string trame);
public:
	Parser();

	void callParse(std::string trame);

	int getSenderCode();
	int getApplicationCode();
	int getTemperature();
	int getWaterCode();
	int getWantedTemperature();

	std::string getSSID();
	std::string getPASS();
	std::string getOLDIP();
	std::string getNEWIP();

};

