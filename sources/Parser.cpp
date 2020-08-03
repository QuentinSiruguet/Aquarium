#include "Parser.h"

void Parser::CheckTrame(std::string trame)
{
	this->parseComplete = false;
	if (trame.find("\n") != std::string::npos)
		trame.replace(trame.find("\n"), sizeof("\n") - 1, "");
	int size = trame.size();
	if (size > 6)
	{
		if (trame.at(0) == '<' && trame.at(1) == 'D' && trame.at(2) == '>')
		{
			if (trame.at(size - 3) == '<' && trame.at(size - 2) == 'F' && trame.at(size - 1) == '>')
			{
				if (trame.find("<D>") != std::string::npos)
					trame.replace(trame.find("<D>"), sizeof("<D>") - 1, "");

				if (trame.find("<F>") != std::string::npos)
					trame.replace(trame.find("<F>"), sizeof("<F>") - 1, "");
				this->Parse(trame);
			}
			else
				std::cout << "ERROR::PARSER::TRAME::END_MISSING" << std::endl;
		}
		else
			std::cout << "ERROR::PARSER::TRAME::START_MISSING" << std::endl;
	}
	else
		std::cout << "ERROR::PARSER::TRAME_SIZE < 6" << std::endl;
}

void Parser::Parse(std::string trame)
{
	std::stringstream ss(trame);
	std::string parsed_part;
	std::vector<std::string> parsed_parts;

	while (std::getline(ss, parsed_part, ';'))
		parsed_parts.push_back(parsed_part);
	/*
	parsed_parts[0] = SenderCode;
	parsed_parts[1] = Application Code;
	*/
	if (parsed_parts.size() > 1)
	{
		this->senderCode = stoi(parsed_parts[0]);
		this->applicationCode = stoi(parsed_parts[1]);

		switch (this->senderCode)
		{
		case MikelServer:
			switch (this->applicationCode)
			{
			case 1:
				if (parsed_parts.size() == 3)
				{
					this->temperature = stoi(parsed_parts[2]);
					this->parseComplete = true;
				}
				else
					std::cout << "ERROR::PARSER::TRAME::NOT_CORRESPONDING" << std::endl;
				break;
			case 2:
				if (parsed_parts.size() == 2)
					this->parseComplete = true;
				else
					std::cout << "ERROR::PARSER::TRAME::NOT_CORRESPONDING" << std::endl;
				break;
			default:
				std::cout << "ERROR:PARSER::TRAME::UNKNOWN_APPCODE" << std::endl;
			}
			break;
		case Enzo:
			switch (this->applicationCode)
			{
			case 1:
				if (parsed_parts.size() == 6)
				{
					this->SSID = parsed_parts[2];
					this->PASS = parsed_parts[3];
					this->OLDIP = parsed_parts[4];
					this->NEWIP = parsed_parts[5];
					this->parseComplete = true;
				}
				else
					std::cout << "ERROR::PARSER::TRAME::NOT_CORRESPONDING" << std::endl;
				break;
			case 2:
				if (parsed_parts.size() == 3)
				{
					this->SSID = "";
					this->PASS = "";
					this->OLDIP = parsed_parts[2];
					this->parseComplete = true;
				}
				else
					std::cout << "ERROR::PARSER::TRAME::NOT_CORRESPONDING" << std::endl;
				break;
			case 3:
				if (parsed_parts.size() == 2)
					this->parseComplete = true;
				else
					std::cout << "ERROR::PARSER::TRAME::NOT_CORRESPONDING" << std::endl;
				break;
			case 4:
				if (parsed_parts.size() == 3)
				{
					this->wantedTemperature = stoi(parsed_parts[2]);
					this->parseComplete = true;
				}
				else
					std::cout << "ERROR::PARSER::TRAME::NOT_CORRESPONDING" << std::endl;
				break;
			default:
				std::cout << "ERROR:PARSER::TRAME::UNKNOWN_APPCODE" << std::endl;
			}
			break;
		case MikelWater:
			switch (this->applicationCode)
			{

			case 1:
				if (parsed_parts.size() == 3)
				{
					this->waterCode = stoi(parsed_parts[2]);
					this->parseComplete = true;
				}
				else
					std::cout << "ERROR::PARSER::TRAME::NOT_CORRESPONDING" << std::endl;
				break;
			default:
				std::cout << "ERROR:PARSER::TRAME::UNKNOWN_APPCODE" << std::endl;
			}
			break;
		default:
			std::cout << "ERROR::PARSER::TRAME::UNKNOWN_SENDERCODE" << std::endl;
		}
	}
	else
		std::cout << "ERROR::PARSER::TRAME::INCOMPLETE" << std::endl;
}

Parser::Parser()
{
	this->parseComplete = false;
	this->senderCode = -1;
	this->applicationCode = -1;
	this->temperature = -1;
	this->waterCode = -1;
	this->wantedTemperature = -1;
}

void Parser::callParse(std::string trame)
{
	this->CheckTrame(trame);
}

int Parser::getSenderCode()
{
	if (this->parseComplete)
		return this->senderCode;
	else
		return -1;
}

int Parser::getApplicationCode()
{
	if (this->parseComplete)
		return this->applicationCode;
	else
		return -1;
}

int Parser::getTemperature()
{
	if (this->parseComplete)
		return this->temperature;
	else
		return -1;
}

int Parser::getWaterCode()
{
	if (this->parseComplete)
		return this->waterCode;
	else
		return -1;
}

int Parser::getWantedTemperature()
{
	return this->wantedTemperature;
}

std::string Parser::getSSID()
{
	if (this->parseComplete)
		return this->SSID;
	else
		return "ERROR";
}

std::string Parser::getPASS()
{
	if (this->parseComplete)
		return this->PASS;
	else
		return "ERROR";
}

std::string Parser::getOLDIP()
{
	if (this->parseComplete)
		return this->OLDIP;
	else
		return "ERROR";
}

std::string Parser::getNEWIP()
{
	if (this->parseComplete)
		return this->NEWIP;
	else
		return "ERROR";
}