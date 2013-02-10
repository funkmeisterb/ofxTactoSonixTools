#ifndef _OF_TACTO_BUTTONONOFF
#define _OF_TACTO_BUTTONONOFF

/**
 * \class ofxTactoButtonOnOff
 *
 * \brief This is an on/off touch- and mouse-enabled button (a toggle).
 *
 * \author Bruno Angeles (bruno.angeles@mail.mcgill.ca)
 *
 * \version 1.0
 *
 * \date 2012/08/14
 *
 */

#include "ofxTextSuite.h"

/// A class that implements an on/off button.
class ofxTactoButtonOnOff : public ofBaseApp
{
public:
	void				setup(int _nColorOn, int _nColorOff); ///< Override of a regular OpenFrameworks function.
	void				draw(); ///< Regular OpenFrameworks function.
	void				windowResized(int w, int h); ///< Regular OpenFrameworks function.
	void				mousePressed(int x, int y, int button); ///< Regular OpenFrameworks function.
	void				touchDown(float x, float y, int touchId); ///< Regular OpenFrameworks function.

	void				setState(bool _inEnabled); ///< Sets the state (pushed or not) of the button.
	bool				getState(); ///< Returns the state (pushed or not) of the button.
	void				setPosition(float _x, float _y); ///< Sets the position of the button.
	bool				isPointInside(float _x, float _y, bool fullRange); ///< Returns true if and only if the specified coordinates are within the button.
	bool				isActive(); ///< Returns true if and only if the button is active.
	void				setActive(bool _bActive); ///< Makes the button active or not.

private:
	int					m_nColorOn; ///< The HEX value of the color when the button is on
	int					m_nColorOff; ///< The HEX value of the color when the button is off
	float				m_fRadius; ///< The radius of the button
	bool				m_bEnabled; ///< Whether or not the button is on
	bool				m_bActive; ///< ///< Is the button active? (do not draw or interact with the shape until it is)
	ofPoint				m_ptOrigin; ///< The origin of the button
	ofxTextBlock		m_labelText; ///< A text block to display the button's label
};

#endif


