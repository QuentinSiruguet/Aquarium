#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

class Journal
{
private:
	std::string fileName;
public:

	void showTime();
	void update(std::string senderName, std::string appName, std::string resultName);
};

