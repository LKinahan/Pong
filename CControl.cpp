#include "stdafx.h"
#include "CControl.h"
#include <conio.h>
#include <sstream>
#include <string>
#include <vector>

#define ADC_MAX 1024
#define MIN_ANGLE 0
#define MAX_ANGLE 180
#define RECEIVED_VALUE 3
#define CHANNEL_RGBLED_RED 39
#define CHANNEL_SERVO_1 0


void CControl::init_com(int comport)
{
	// std::string strcomport = "COM" + std::to_string(comport);
	_com.open("COM3");
}


bool CControl::get_data(int type, int channel, int& result)
{


	if (_com.is_open())
	{
		std::string tx_str;
		std::string rx_str;
		std::string type_str, channel_str;


		tx_str = "G " + (type_str = std::to_string(type)) + " " + (channel_str = std::to_string(channel)) + "\n";

		// temporary storage
		char buff[2];

		do
		{
			// Send TX string
			_com.write(tx_str.c_str(), tx_str.length());
			Sleep(10); // wait for ADC conversion, etc. May not be needed?

			// start timeout count
			double start_time = cv::getTickCount();

			buff[0] = 0;
			// Read 1 byte and if an End Of Line then exit loop
			// Timeout after 1 second, if debugging step by step this will cause you to exit the loop
			while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
			{
				if (_com.read(buff, 1) > 0)
				{
					rx_str = rx_str + buff[0];
				}
			}
			std::vector <std::string> results;
			std::istringstream iss(rx_str);
			for (std::string rx_str; iss >> rx_str;)
				results.push_back(rx_str);

			result = std::stoi(results[RECEIVED_VALUE]);
			_com.flush();
		} while (!kbhit);
	}
}


bool CControl::set_data(int type, int channel, int val)
{
	std::string tx_str = "S " + std::to_string(type) + " " + std::to_string(channel) + " " + std::to_string(val) + "\n";
	std::string rx_str;

	// temporary storage
	char buff[2];

	// Send TX string
	_com.write(tx_str.c_str(), tx_str.length());
	Sleep(10); // wait for ADC conversion, etc. May not be needed?

	// start timeout count
	double start_time = cv::getTickCount();

	buff[0] = 0;
	// Read 1 byte and if an End Of Line then exit loop
	// Timeout after 1 second, if debugging step by step this will cause you to exit the loop
	while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
	{
		if (_com.read(buff, 1) > 0)
		{
			rx_str = rx_str + buff[0];
		}
	}
	_com.flush();
}


float CControl::get_analog(int channel)
{
	int analog_joystick;
	get_data(ANALOG, channel, analog_joystick);
	float percent_joystick = (float)analog_joystick / ADC_MAX;
	return percent_joystick;
}


void CControl::get_digital(int channel)
{
	while (!kbhit())
	{
		int digital_result;
		get_data(DIGITAL, channel, digital_result);
		set_data(DIGITAL, CHANNEL_RGBLED_RED, !digital_result);
		std::cout << "DIGITAL TEST: CH32 = " << digital_result << std::endl;
	}
}


bool CControl::get_button(int channel)
{
	int prev_digital_result, current_digital_result;
	get_data(DIGITAL, channel, prev_digital_result);

	double start_time = 0;
	if (!prev_digital_result)
		start_time = cv::getTickCount();

	while ((cv::getTickCount() - start_time) / cv::getTickFrequency() < 0.2)
	{
		get_data(DIGITAL, channel, current_digital_result);

		if (current_digital_result != prev_digital_result)
			break;
		return 1;
	}
	return 0;
}


void CControl::get_servo(void)
{
	while (!kbhit())
	{
		for (int angle = MIN_ANGLE; angle <= MAX_ANGLE; angle += 1)
		{
			set_data(SERVO, CHANNEL_SERVO_1, angle);
			std::cout << "SERVO TEST: angle = " << angle << std::endl;
		}
		for (int angle = MAX_ANGLE; angle >= MIN_ANGLE; angle -= 1)
		{
			set_data(SERVO, CHANNEL_SERVO_1, angle);
			std::cout << "SERVO TEST: CH0 POS = " << angle << std::endl;
		}
	}
}