#include "ofxTactoButtonOnOff.h"

/**
* \param _nColorOn The colour of the button when it is on.
* \param _nColorOff The colour of the button when it is off.
*/
void ofxTactoButtonOnOff::setup(int _nColorOn, int _nColorOff)
{
	m_nColorOn = _nColorOn;
	m_nColorOff = _nColorOff;
	m_bEnabled = false;
	m_bActive = false;
	m_labelText.init("fonts/arial.ttf", 20);
	m_labelText.setText("");
	m_labelText.wrapTextX(ofGetWidth() * 3/4);
	m_fRadius = 61.0f; // TODO: percentage of minimum window size?
}

void ofxTactoButtonOnOff::draw()
{
	if (m_bActive)
	{
		int currentColor = 0;
		m_bEnabled == true ? currentColor = m_nColorOn : currentColor = m_nColorOff;

		// Draw button
		ofSetHexColor(currentColor);
		ofFill();
		ofCircle(m_ptOrigin.x, m_ptOrigin.y, m_fRadius);

		// Draw label
		m_labelText.setColor(0, 0, 0, 255);
		string labelStr = "";
		m_bEnabled == true ? labelStr = "On" : labelStr = "Off";
		m_labelText.setText(labelStr);
		float textHeight = m_labelText.getHeight();
		m_labelText.drawCenter(m_ptOrigin.x, m_ptOrigin.y - textHeight/2);
	}
}

/**
* \param w The new width in pixels.
* \param h The new height in pixels.
*/
void ofxTactoButtonOnOff::windowResized(int w, int h)
{
	float minMeasure = min(ofGetHeight(), ofGetWidth());
	m_fRadius = minMeasure * 0.08f; // percentage of minimum window size
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void ofxTactoButtonOnOff::mousePressed(int x, int y, int button)
{
	m_bEnabled = !m_bEnabled;
}

/**
* \param _inEnabled Whether or not the button is on or off.
*/
void ofxTactoButtonOnOff::setState(bool _inEnabled)
{
	m_bEnabled = _inEnabled;
}

/**
* \return Whether or not the button is pushed.
*/
bool ofxTactoButtonOnOff::getState()
{
	return m_bEnabled;
}

/**
* \param _x The x coordinate of the button's origin.
* \param _y The y coordinate of the button's origin.
*/
void ofxTactoButtonOnOff::setPosition(float _x, float _y)
{
	m_ptOrigin = ofPoint(_x, _y);
}

/**
* \param _x The x coordinate of the queried point.
* \param _y The y coordinate of the queried point.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \return Whether or not the button is pushed.
*/
bool ofxTactoButtonOnOff::isPointInside(float _x, float _y, bool fullRange)
{
	bool inShape = false;
	float xWorld = _x;
	float yWorld = _y;

	if (!fullRange) // the input values are [0;ofGetWidth()], not [0;1]
	{
		xWorld *= ofGetWidth();
		yWorld *= ofGetHeight();
	}

	// The point is inside if its distance to the button's origin is less than the button's radius
	float distance = sqrtf(pow(m_ptOrigin.x - xWorld, 2) + pow(m_ptOrigin.y - yWorld, 2));
	if (distance < m_fRadius)
		inShape = true;

	return inShape;
}

/**
* \return Whether or not the button is active (pushed down).
*/
bool ofxTactoButtonOnOff::isActive()
{
	return m_bActive;
}

/**
* \param _bActive Whether or not the button is active (pushed down).
*/
void ofxTactoButtonOnOff::setActive(bool _bActive)
{
	m_bActive = _bActive;
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void ofxTactoButtonOnOff::touchDown(float x, float y, int touchId)
{
	m_bEnabled = !m_bEnabled;
}
