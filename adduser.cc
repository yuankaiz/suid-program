#include <iostream>
#include <botan-1.10/botan/botan.h>
#include <botan-1.10/botan/auto_rng.h>
#include <botan-1.10/botan/bcrypt.h>
#include <memory>
#include <string>
#include <stdio.h>
#include <fstream>
#include <ios>
#include <termios.h>
#include <unistd.h>

#define CONFIG_FILE "/home/USER99D59/USER99D59.cfg"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using Botan::LibraryInitializer;
using Botan::generate_bcrypt;
using Botan::AutoSeeded_RNG;

void TurnOffEcho();
void TurnOnEcho();

int main()
{
	LibraryInitializer init;
	AutoSeeded_RNG rng;
	string username;
	cout << "Please enter username: ";
	cin >> username;
	cout << "Please enter password for " << username << ": ";
	TurnOffEcho();
	string password1, password2;
	cin >> password1;
	TurnOnEcho();
	cout << endl;
	cout << "Please confirm password for " << username << ": ";
	TurnOffEcho();
	cin >> password2;
	TurnOnEcho();
	cout << endl;
	if (password1 != password2)
	{
		cout << "Passwords don't match..." << endl;
		return 0;
	}
	string hash = generate_bcrypt(password1, rng);
	cout << "hash is: " << hash << endl;
	ofstream cfgfile(CONFIG_FILE, std::ios::app);
	if (cfgfile.is_open())
	{
		cfgfile << username << " " << hash << endl;
		cfgfile.close();
	}
	else
		cout << "Unable to open config file." << endl;
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