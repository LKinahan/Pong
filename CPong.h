#pragma once

#include "stdafx.h"
#include "CBase4618.h"
#include <opencv2/imgproc/imgproc_c.h>
#include <mutex>
#include "time.h"
 
using namespace std;

#define JOYSTICK 26 ///< input for player 1 from MSP joystick Y
#define UPPER_LIMIT_Y 0.75  ///< defines upper limits of window space for paddles
#define LOWER_LIMIT_Y 0.2 ///< defines lower limits of window space for paddles
#define BUTTON 32 ///< Button2 for reset

static int score1 = 0; ///< int to hold score
static int score2 = 0; ///< int to hold score
static int windowX = 1000; ///< horizontal dimensions of play window
static int windowY = 800; ///< vertical dimensions of play window

/**
* @brief A mini game of Pong 
* This class inherits from CBase4618 to draw a Pong game using open cv
* @author Laurel Kinahan
*/
class CPong :public CBase4618 {
private:
cv::Size canvas_size; ///< dimensions of play window
double frameRate; ///< holds freames per second
double frequency; ///< frequency of computer
double startTick; ///< used to get initial tick counts

//ball dimensions and speed variables
#define ball_radius 16 ///< ball radius
cv::Point ball; ///< point to hold all dimensions of ball
cv::Point ball_move; ///< track movement of ball
cv::Point ball_move_default; ///< return to default upon reset
double ball_current_time; ///< current speed of ball
double ball_prev_time; ///< hold previous speed of ball
double ball_delta_time; ///< change in ball speed

float rand = std::rand() % 40 - 20; ///< 
float rand2 = std::rand() % 40 - 20; ///< 

cv::Rect P2Pos; ///< paddle player 1 = user
cv::Rect P1Pos; ///< paddle player 2
float P1Pos_move;///< tracks movement of paddle float

mutex safe; ///< variable for mutex so input from joystick is safe
mutex hold; ///< variable for mutex so important functions defined in both update and draw can be accessed

public:

	/**
	* @brief constructor for CPong - initializes com port and canvas
	* @param None
	* @return None
	*/
	CPong(int comport = 3);

	/**
	* @brief destructor for CPong- destroys object when program is closed
	* @param None
	* @return None
	*/
	~CPong();

	/**
	* @brief Update will do all the assignings and instantiating for the Pong game
	* user input from joysitck, ball position, score handling etc
	* @param none
	* @return none
	*/
	void update();

	/**
	* @brief Draw is in charge of rendering all objects that need to be displayed
	* paddles, ball, text etc
	* @param none
	* @return none
	*/
	void draw();

	/**
	*@breif
	*@param none
	*@return int
	*/
	void score(int player);

	/**
	* @brief Reset is charge of resetting to the intial conditions
	* @param none
	* @return none
	*/
	void reset();
	
	/**
	*@breif Start the Update() and draw() threads and loop them
	*@param none
	*@return none
	*/
	void run();

	/**
	*@breif Initializes gameboard for start of play
	*@param none
	*@return none
	*/
	void gameboard();

	/**
	*@breif This function will be used to start the Update() thread
	*@param none
	*@return none
	*/
	static void update_thread(CPong* ptr);

	/**
	*@breif This function will be used to start the draw() thread
	*@param none
	*@return none
	*/
	static void draw_thread(CPong* ptr);
	
};