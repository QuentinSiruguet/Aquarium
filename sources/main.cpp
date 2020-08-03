#include "TCPServer.h"
#include "Parser.h"
#include "Journal.h"

void rewriteStaticIp(std::string old_ip, std::string new_ip)
{
	/*Define variables*/
	std::ifstream file("/etc/dhcpcd.conf");
	std::ofstream result("/etc/newdhcpcd.conf");
	std::string staticip = "static ip_address=";
	staticip += old_ip;
	staticip += "/24";
	char buffer[100];
	std::string buf;
	/*find static_ip_address and write all file into new one*/
	do
	{
		file.getline(buffer, 100);
		buf = buffer;
		if (buf != staticip)
			result << buffer << std::endl;
	} while (buf != staticip);
	/*change static ip*/
	result << "static ip_address=" << new_ip << "/24";
	/*close files and replace them*/
	file.close();
	result.close();

	std::remove("/etc/dhcpcd.conf");
	std::rename("/etc/newdhcpcd.conf", "/etc/dhcpcd.conf");
}

void createWifiConnection(std::string SSID, std::string PASS, std::string old_ip, std::string new_ip)
{
	std::ofstream Wifi("/etc/wpa_supplicant/wpa_supplicant.conf");

	Wifi << "ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev" << std::endl
		<< "update_config=1" << std::endl
		<< "country=FR" << std::endl << std::endl;

	Wifi << "network={" << std::endl
		<< "ssid=\"" << SSID << "\"" << std::endl
		<< "psk=\"" << PASS << "\"" << std::endl
		<< "}";

	Wifi.close();
	rewriteStaticIp(old_ip, new_ip);
	system("sudo reboot");
}

int main()
{
	Parser parser;
	TCPServer Server(8888);
	Journal journal;

	while (true)
	{
		std::string read;
		read = Server.recvString();
		parser.callParse(read);
		journal.showTime();
		switch (parser.getSenderCode())
		{
		case MikelServer:
			Server.setLastIPName("MIKELSERVER");
			switch (parser.getApplicationCode())
			{
			case 1:
				std::cout << "MIKELSERVER::TEMP::" << parser.getTemperature() << "°C" << std::endl;
				journal.update("MIKELSERVER", "TEMPERATURE", std::to_string(parser.getTemperature()));
				break;
			case 2:
			{
				std::cout << "MIKELSERVER::REQUEST_DATA::" << std::endl;
				std::stringstream ss;
				ss << "<D>" << parser.getWantedTemperature() << "<F>";
				journal.update("MIKELSERVER", "REQUEST_DATA", ss.str());
				//ss << "\n";
				Server.returnString(ss.str());
			}
			break;
			default:
				journal.update("MIKELSERVER", "UNKNOWN", read);
			}
			break;
		case Enzo:
			Server.setLastIPName("ENZO");
			switch (parser.getApplicationCode())
			{
			case 1:
			{
				std::cout << "ENZO::CHANGEWIFI::" << parser.getSSID() << " " << parser.getPASS() << " " << parser.getNEWIP() << std::endl;
				std::stringstream ss1;
				ss1 << parser.getSSID() << ";" << parser.getPASS() << ";" << parser.getNEWIP();
				journal.update("ENZO", "CHANGEWIFI", ss1.str());
				createWifiConnection(parser.getSSID(), parser.getPASS(), parser.getOLDIP(), parser.getNEWIP());
			}
			break;
			case 2:
				std::cout << "ENZO::DEFAULT_WIFI::" << std::endl;
				journal.update("ENZO", "CHANGEWIFI", "DEFAULT");
				createWifiConnection("", "", parser.getOLDIP(), "10.0.0.5");
				break;
			case 3:
			{
				std::cout << "ENZO::REQUEST_DATA::" << std::endl;
				std::stringstream ss;
				ss << "<D>" << parser.getTemperature() << ";" << parser.getWaterCode() << "<F>";
				journal.update("ENZO", "REQUEST_DATA", ss.str());
				ss << "\n";
				Server.returnString(ss.str());
			}
			break;
			case 4:
			{
				std::cout << "ENZO::WANTED_TEMP :: ";
				std::stringstream ss;
				ss << parser.getWantedTemperature();
				std::cout << ss.str() << std::endl;
				journal.update("ENZO", "WANTED_TEMP", ss.str());
			}
			break;
			default:
				journal.update("ENZO", "UNKNOWN", read);
			}
			break;
		case MikelWater:
			Server.setLastIPName("MIKELWATER");
			switch (parser.getApplicationCode())
			{
			case 1:
				std::cout << "MIKELWATER::WATER::" << parser.getWaterCode() << std::endl;
				journal.update("MIKELWATER", "WATER", std::to_string(parser.getWaterCode()));
				break;
			default:
				journal.update("MIKELWATER", "UNKNOWN", read);
			}
			break;
		default:
			journal.update("UNKNOWN", "UNKNOWN", read);
		}
		Server.showIP();
	}
	return 0;
}