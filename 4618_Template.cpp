////////////////////////////////////////////////////////////////
// ELEX 4618 Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated December 30, 2020
////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <thread>
#include <stdlib.h>

#include <conio.h>

#include "Server.h"
#include "Serial.h" // Must include Windows.h after Winsock2.h, so Serial must include after Client/Server

// OpenCV Include
#include <opencv2/imgproc/imgproc_c.h>

#include "Cbase4618.h"
#include "CControl.h"
//#include "CSketch.h"
#include "CPong.h"


using namespace std;

int main(int argc, char* argv[])
{
	char inputSelect;

	do {

		cout << "**************************************** \n"
			<< "              ELEX4618 \n"
			<< "       Embedded System Control \n"
			<< "**************************************** \n\n"
			<< "  (S)ketch \n"
			<< "  (P)ong \n"
			//<< "  (A)stroids \n"
			<< "  (Q)uit \n"
			<< "\n    Cmd:";

		cin >> inputSelect;

		switch (inputSelect) {

		case 'A':
		case 'a': {
			//CAstroids aGame(3);
			//aGame.run();
			break;
		}

		case 'S':
		case 's': {
			//CSketch etch(800, 3);
			//etch.run();
			break;
		}

		case 'P':
		case 'p': {
			CPong pGame;
			pGame.run();
			break;
		}

		case 'Q':
		case 'q': {
			cout << "Quiting" << endl;
			exit(0);
			break;
		}
		default: {
			cout << "Command not found\n";
			inputSelect = 0;
			break;
		}
		}

	} while (inputSelect == 'Y' || inputSelect == 'y' || inputSelect == 0);
	return 0;

}

