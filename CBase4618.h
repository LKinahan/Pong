#pragma once

#include "stdafx.h"
#include "CControl.h"
#include <opencv2/imgproc/imgproc_c.h>

/** 
* @brief Base Class for Update, Draw, and Run functions
* This class communcates between CControl and mini games such as Pong 
* @author Laurel Kinahan
*/

class CBase4618
{
private:

public:	
	
	CControl mCControl; ///< CControl gamepad object
	cv::Mat mCanvas; ///< Matrix canvas to draw on

	/**
	* @brief This is the class constructor
	* @param None
	* @return None
	*/
	CBase4618();

	/**
	* @brief This is the class destructor
	* @param None
	* @return None
	*/
	~CBase4618();

	/**
	* @brief Updates internal variables
	* @param nothing to receive
	* @return nothing to return
	*/
	virtual void update() = 0;

	/**
	* @brief Draws on the created image
	* @param nothing to receive
	* @return nothing to return
	*/
	virtual void draw() = 0;

	/**
	* @brief Initializes the COM port
	* @param nothing to receive
	* @return nothing to return
	*/
	virtual void run();

	/**
	* @brief Terminates the calling thread
	* @param nothing to receive
	* @return nothing to return
	*/
	bool _thread_exit;
};

