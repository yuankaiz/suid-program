#include <iostream>
#include <botan-1.10/botan/botan.h>
#include <botan-1.10/botan/auto_rng.h>
#include <botan-1.10/botan/bcrypt.h>
#include <memory>
#include <string>
#include <stdio.h>
#include <fstream>
#include <map>
#include <termios.h>
#include <unistd.h>
#include <ios>
#include <ctime>
#include <stdlib.h>

#define CONFIG_FILE "/home/USER99D59/USER99D59.cfg"
#define LOG_FILE "/home/USER99D59/USER99D59.log"
#define SECRET_FILE "/home/USER99D59/USER99D59.txt"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::map;
using Botan::LibraryInitializer;
using Botan::generate_bcrypt;
using Botan::check_bcrypt;
using std::time_t;
using std::time;
using std::ctime;

void TurnOffEcho();
void TurnOnEcho();


int main()
{
	try
	{
		LibraryInitializer init;
		string username, password, hash;
		cout << "Please enter username: ";
		cin >> username;
		cout << "please enter password: ";
		TurnOffEcho();
		cin >> password;
		TurnOnEcho();
		cout << endl;
		ifstream cfgfile(CONFIG_FILE);
		ofstream logfile(LOG_FILE, std::ios::app);
		string user, pass;
		map<string,string> credential;
		if (!cfgfile.is_open())
		{
			cout << "Unable to open config file. Exiting..." << endl;
			return 1;
		}
		while (cfgfile >> user >> pass)
		{
			credential[user] = pass;
		}
		cfgfile.close();
		if (credential.find(username) == credential.end())
		{
			cout << "User " << username << " does not exist." << endl;
			cout << "This activity will be logged" << endl;			
			if (!logfile.is_open())
			{
				cout << "Unable to open log file." << endl;
				return 1;
			}
			time_t now = time(NULL);
			string tstr = ctime(&now);
			logfile << "<" << tstr.substr(0,tstr.size()-1) << "> Nonexistent user failed to access secret" << endl;
			logfile.close();
			return 0;
		}

		hash = credential[username];
		const bool ok = check_bcrypt(password, hash);
		if (ok)
		{
			//print secret
			// cout << "secret" << endl;
			ifstream secretfile(SECRET_FILE);
			string line;
			if (secretfile.is_open())
			{
				while (getline(secretfile,line))
					cout << line << endl;
				secretfile.close();
			}
			else
				cout << "Unable to open secret file." << endl;
			//log
			cout << "This activity will be logged" << endl;
			if (!logfile.is_open())
			{
				cout << "Unable to open log file." << endl;
				return 1;
			}
			time_t now = time(NULL);
			string tstr = ctime(&now);
			logfile << "<" << tstr.substr(0,tstr.size()-1) << "> " << username << " accessed secret" << endl;
			logfile.close();
			return 0;
		}
		else
		{
			cout << "Password invalid" << endl;
			cout << "This activity will be logged" << endl;
			if (!logfile.is_open())
			{
				cout << "Unable to open log file." << endl;
				return 1;
			}
			time_t now = time(NULL);
			string tstr = ctime(&now);
			logfile << "<" << tstr.substr(0,tstr.size()-1) << "> " << username << " failed to access secret: password invalid" << endl;
			logfile.close();
			sleep(1);
			return 0;
		}
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << endl;
		return 1;
	}
	return 0;
}

//turn off/on stdin echo
//learned from online resources
void TurnOffEcho()
{
	termios tty;
	tcgetattr(STDIN_FILENO, &tty);
	tty.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void TurnOnEcho()
{
	termios tty;
	tcgetattr(STDIN_FILENO, &tty);
	tty.c_lflag |= ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}