#include "Journal.h"

void Journal::showTime()
{
	std::time_t	currentDate = std::time(nullptr);

	/*DEFINE FILENAME*/
	char str[100];
	strftime(str, 100, "%T", std::localtime(&currentDate));
	std::cout << str << " - ";
}

void Journal::update(std::string senderName, std::string appName, std::string resultName)
{
	std::time_t	currentDate = std::time(nullptr);

	/*DEFINE FILENAME*/
	char str[100];
	strftime(str, 100, "%F", std::localtime(&currentDate));
	this->fileName = str;
	this->fileName += ".csv";

	/*OPENING FILENAME*/
	std::ofstream Journal(this->fileName, std::ios::out | std::ios::app);

	/*WRITING DATA IN FILE*/
	strftime(str, 100, "%T", std::localtime(&currentDate));
	Journal << str << ";" << senderName << ";" << appName << ";" << resultName << std::endl;


	/*CLOSING FILE*/
	Journal.close();
}


