////////////////////////////////////////////////////////////////
// CPong.cpp
// Member function definition for CPong.h
// This program runs a simple Pong Game with input from user
// Created Feb 20, 2021 by Laurel Kinahan 
////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "CPong.h"
#include <opencv2/imgproc/imgproc_c.h>
#include <chrono>
#include <conio.h>
#include <thread>
#include "CBase4618.h"
#include <mutex>
#include <iostream>
#include <string>
#include "time.h"

using namespace std;

CPong::CPong(int comport)
{
	mCControl.init_com(comport);
	gameboard();
	reset();
	_thread_exit = false;
}

CPong::~CPong()
{

}

void CPong::gameboard()
{
	mCanvas = cv::Mat::zeros(windowY, windowX, CV_8UC3);
	P1Pos = cv::Rect(windowX / 99, windowY / 2 - windowY / 20, windowX / 54, windowY / 7);
	P2Pos = cv::Rect(windowX - windowX / 40, windowY / 2 - windowY / 20, windowX / 54, windowY / 7);
	ball = cv::Point(windowX / 2, windowY / 2);
	ball_move_default = cv::Point(-windowX / 5, windowY / 5);
	ball_move = ball_move_default;
	canvas_size = cv::Size(windowX, windowY);
}

void CPong::reset()
{
	string score1Str = to_string(score1);
	string score2Str = to_string(score2);
	score1 = 0;
	score1Str = "Player 1: " + to_string(score1);
	score2 = 0;
	score2Str = "Player 2: " + to_string(score2);
	ball = cv::Point(500, 400);
	ball_prev_time = cv::getTickCount();
	ball_move = ball_move_default;
	P1Pos = cv::Rect(windowX / 99, windowY / 2 - windowY / 20, windowX / 54, windowY / 7);
}

void CPong::update()
{
	if (mCControl.get_button(BUTTON))
		reset();

	// ball to start in center and change speeds according to player 1 paddle response
	ball_current_time = cv::getTickCount();
	ball_delta_time = (ball_current_time - ball_prev_time) / (cv::getTickFrequency());
	ball_prev_time = ball_current_time;
	ball = cv::Point(ball.x + ball_move.x * ball_delta_time, ball.y + ball_move.y * ball_delta_time);

	// if ball reaches player 1 paddle, ball bounces off and reverse direction
	if (ball.x - ball_radius <= P1Pos.x + P1Pos.width && ball.x + ball_radius > P1Pos.x + P1Pos.width)
	{
		if ((ball.y + ball_radius > P1Pos.y) && (ball.y - ball_radius < P1Pos.y + P2Pos.height))
		{
			ball_move.x *= -1.3;
		}
	}

	// if ball reaches player 2 paddle, ball bounces off and reverse direction
	else if (ball.x + ball_radius >= P2Pos.x && ball.x - ball_radius < P2Pos.x)
	{
		if ((ball.y + ball_radius > P2Pos.y) && (ball.y - ball_radius < P2Pos.y + P2Pos.height))
		{
			ball_move.x *= -1.3;
		}
	}

	// if ball passes player 1 paddle, player 2 scores and board reset
	else if (ball.x + ball_radius > canvas_size.width)
	{
		score(1);
	}

	// if ball passes player 2 paddle, player 1 scores and board reset
	else if (ball.x - ball_radius < 0)
	{
		score(2);
	}
	else
		ball.x = ball.x;

	// ball bounces off to pand bottom of play window
	if (ball.y - ball_radius < 0 || ball.y + ball_radius > canvas_size.height)
		ball_move.y *= -1;
	else
		ball.y = ball.y;

	// player 1 from MSP Joystick
	safe.lock();
	P1Pos_move = mCControl.get_analog(JOYSTICK);

	if (P1Pos_move > UPPER_LIMIT_Y)
	{
		P1Pos.y -= canvas_size.height / 25 * (P1Pos_move - UPPER_LIMIT_Y);
		if (P1Pos.y < 0)
			P1Pos.y = 0;
	}
	else if (P1Pos_move < LOWER_LIMIT_Y)
	{
		P1Pos.y += canvas_size.height / 25 * (LOWER_LIMIT_Y - P1Pos_move);
		if (P1Pos.y > canvas_size.height - P1Pos.height)
			P1Pos.y = canvas_size.height - P1Pos.height;
	}
	else
		P1Pos.y = P1Pos.y;
	safe.unlock();

	//Player 2
	if (P2Pos.y + P2Pos.height / 2 < ball.y)
	{
		P2Pos.y += canvas_size.height / 90;
		if (P2Pos.y > canvas_size.height - P2Pos.height)
			P2Pos.y = canvas_size.height - P2Pos.height;
	}
	else if (P2Pos.y + P2Pos.height / 2 > ball.y)
	{
		P2Pos.y -= canvas_size.height / 90;
		if (P2Pos.y < 0)
			P2Pos.y = 0;
	}
	else
		P2Pos.y = P2Pos.y;

	//reset board if someone wins
	if (score1 == 5 || score2 == 5)
		ball_move = cv::Point(0, 0);
}


void CPong::draw()
{
	string score1Str = to_string(score1);
	string score2Str = to_string(score2);

	frequency = cv::getTickFrequency();
	while ((cv::getTickCount() - startTick) / frequency < 0.033333)
	{

	}
	frameRate = 1 / ((cv::getTickCount() - startTick) / frequency);
	startTick = cv::getTickCount();

	mCanvas = cv::Mat::zeros(canvas_size.height, canvas_size.width, CV_8UC3);
	line(mCanvas, cv::Size(500, 0), cv::Size(500, 800), cv::Scalar(0, 0, 255), 2, CV_AA, 0);
	line(mCanvas, cv::Size(0, 0), cv::Size(1000, 0), cv::Scalar(2500, 255, 255), 2, CV_AA, 0);
	line(mCanvas, cv::Size(0, 800), cv::Size(1000, 800), cv::Scalar(2500, 255, 255), 2, CV_AA, 0);
	circle(mCanvas, ball, ball_radius, cv::Scalar(150, 150, 150), CV_FILLED, -1);
	rectangle(mCanvas, P1Pos, cv::Scalar(255, 0, 0), CV_FILLED, CV_AA, 0);
	rectangle(mCanvas, P2Pos, cv::Scalar(0, 255, 255), CV_FILLED, CV_AA, 0);
	putText(mCanvas, "Player 1", cv::Size(100, 50), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(100, 255, 100), 2);
	putText(mCanvas, "Player 2", cv::Size(700, 50), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(100, 255, 100), 2);
	putText(mCanvas, score1Str, cv::Size(450, 50), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(100, 255, 100), 2);
	putText(mCanvas, score2Str, cv::Size(530, 50), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(100, 255, 100), 2);
	putText(mCanvas, "FPS : " + to_string(frameRate), cv::Size(700, 770), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 255, 255), 2);

	if (score1 == 5 || score2 == 5)
	{
		mCanvas = cv::Mat::zeros(canvas_size.height, canvas_size.width, CV_8UC3);
		if (score1 == 5)
		{
			cv::putText(mCanvas, "YOU WIN!", cv::Size(300, 400), CV_FONT_HERSHEY_PLAIN, 4, cv::Scalar(100, 255, 100), 2);
		}

		else
		{
			cv::putText(mCanvas, "YOU LOST!", cv::Size(300, 400), CV_FONT_HERSHEY_PLAIN, 4, cv::Scalar(100, 255, 100), 2);
		}
	}
	cv::imshow("ELEX4618 - Lab04 - PongGame", mCanvas);
}

//////////// Multithreading Functions 

void CPong::update_thread(CPong* ptr)
{
	while (ptr->_thread_exit == false)
	{
		ptr->update();
		if (cv::waitKey(1) == 'q')
			ptr->_thread_exit = true;
	}
}


void CPong::draw_thread(CPong* ptr)
{
	while (ptr->_thread_exit == false)
	{
		ptr->draw();
		if (cv::waitKey(1) == 'q')
			ptr->_thread_exit = true;
	}
}


void CPong::run()
{
	thread thread1(&CPong::update_thread, this);
	thread thread2(&CPong::draw_thread, this);
	thread1.detach();
	thread2.join();
}

void CPong::score(int player)
{
	string score1Str = to_string(score1);
	string score2Str = to_string(score2);
	safe.lock();
	if (player == 1)
	{
		score1++;
		score1Str = "Player 1: " + to_string(score1);
	}
	else
	{
		score2++;
		score2Str = "Player 2: " + to_string(score2);
	}
	ball = cv::Point(500, 400);
	ball_prev_time = cv::getTickCount();
	safe.unlock();
}
