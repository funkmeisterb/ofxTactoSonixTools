#ifndef OFXTACTOSTAIN_H
#define OFXTACTOSTAIN_H

/**
* \class ofxTactoStain
*
* \brief A class that implements a connected list of points representing a stain.
*
* \author Bruno Angeles (bruno.angeles@mail.mcgill.ca)
 *
 * \version 1.0
 *
 * \date 2012/08/14
*
*/

#define VERTICES_MAX 64
#define MAX_COUNT 65536

#include "ofMain.h"
#include "ofxTextSuite.h"

/** \brief A class that represents an individual vertex, many of which make up a stain.
*/
class ofxTactoVertex
{
public:
	float 	    x; ///< Current X position
	float   	y; ///< Current Y position
	bool    	bBeingDragged; ///< Is the vertex being dragged?
	float 	    xOriScale; ///< X position when scaling started
	float   	yOriScale; ///< Y position when scaling started
};

/** \brief A class that contains information about the motion of a blob.
*/
class ofxTactoBlobMovementInfo
{
public:
	int			ID; ///< The ID of the blob
	float 	    x; ///< The X position of the blob
	float   	y; ///< The Y position of the blob
	float 	    xOri; ///< The X position of the blob at the time the motion was started
	float   	yOri; ///< The Y position of the blob at the time the motion was started
	// Default constructor.
	ofxTactoBlobMovementInfo()
		: ID(0), x(0), y(0), xOri(0), yOri(0) {}

	/** \brief We override the comparison operator to consider blobs as identical
	* if and only if their IDs are identical.
	*/
	bool operator==(const int &_ID) const {
		return _ID == ID;
	}
};

class ofxTactoStain
{
public:
	void                					setup(int _nVertices, string _name = "", float _fRadius = 0.2f); ///< Configures the stain.
	void                					update(); ///< Regular OpenFrameworks function.
	void                					draw(); ///< Regular OpenFrameworks function.
	void                					reset(); ///< Resets the initial position of all points but the origin.
	float              						area(); ///< Returns the area of the stain, where 1 is the initial value and negative values represent a stain's opposite side.
	float               					spikiness(); ///< Returns the 'spikiness' of the stain, where 1 is the initial value. The "spikiness" of a stain is a function of the sum of the internal angles of the stain.
	bool									isActive(); ///< Returns true if the stain is active.
	void									setActive(bool _bActive); ///< Makes the stain active or not. When a stain is active, it is displayed and can be modified.
	bool									isInMotion(); ///< Returns true if the stain is moving.
	void									setMovable(bool _bMovable); ///< Makes the stain movable or not.
	void									setScalable(bool _bScalable); ///< Makes the stain scaleable or not.
	void									setShapable(bool _bShapable); ///< Makes the stain's shape modifiable or not.
	void									setOrigin(float _fOriX, float _fOriY); ///< Sets the origin of the stain.
	ofPoint									getOrigin(); ///< Returns the origin of the stain.
	void									setColors(int _nLowGradientColor, int _nHighGradientColor); ///< Sets the two colours that determine a colour gradient for the stain.
	void									setColorGradient(float _fGradient); ///< Sets the stain's colour in the range of the gradient.
	bool									isPointInside(float x, float y, bool fullRange); ///< Returns true if and only if the queried point is within the stain.
	bool									isPointClose(float x, float y, bool fullRange); ///< Returns true if and only if the queried point is close to the stain.
	int										getNumPointsInside(); ///< Returns the number of points within the stain.
	int										getTimeSinceFirstFinger(); ///< Returns the time passed since the first finger hit the stain.
	void									mouseDragged(int x, int y, int button); ///< Regular OpenFrameworks function.
	void									mousePressed(int x, int y, int button); ///< Regular OpenFrameworks function.
	void									mouseReleased(int x, int y, int button); ///< Regular OpenFrameworks function.
	void									touchDown(float x, float y, int touchId); ///< Regular OpenFrameworks function.
	void									touchMoved(float x, float y, int touchId); ///< Regular OpenFrameworks function.
	void									touchUp(float x, float y, int touchId); ///< Regular OpenFrameworks function.

private:
	unsigned long int						m_nCounter; ///< A counter for dynamic features
	ofxTextBlock							m_nameText; ///< A text holder for name of the stain

	// Parameters to be used in mapping
	float              						m_fOriX; ///< The x position of the stain's origin. Note that the origin is not necessarily the centre of mass.
	float									m_fOriY; ///< The y position of the stain's origin. Note that the origin is not necessarily the centre of mass.
	float	           						m_fSpikiness; ///< The spikiness of the shape
	float	           						m_fArea; ///< The spikiness of the shape

	float									m_fRadius; ///< The initial radius of the stain (all vertices are placed along this radius).
	float									m_fVertexRadius; ///< The radius of each vertex (used to manipulate the shaped)
	float	           						m_fInitialSize; ///< The initial size of the shape
	float									m_fOriAngle; ///< Angle (in rad) of rotation when scaling started

	int                						m_nColor1; ///< The start gradient color of the shape (see setColorGradient)
	int										m_nColor2; ///< The end gradient color of the shape (see setColorGradient)
	int										m_nCurrentColor; ///< The current color of the shape
	int                						m_nNumVertices; ///< The number of vertices in the stain
	int										m_nTimeFirstFinger; ///< Time (ms) at which the first finger landed on the stain

	bool									m_bActive; ///< Is the shape active? (do not draw or interact with the shape until it is)
	bool									m_bInMotion; ///< Whether or not the stain is being displaced
	bool									m_bMovable; ///< Whether or not the stain can be moved
	bool									m_bScalable; ///< Whether or not the stain can be scaled and rotated
	bool									m_bShapable; ///< Whether or not the stain can be shaped

	ofxTactoVertex							vertices[VERTICES_MAX]; ///< An ofxTactoStain is nothing but an array of vertices!
	ofPoint									m_PtMotionStart; ///< Point at which the motion was started
	ofPoint									m_PtMotionOrigin; ///< Origin point when the motion was started
	std::list<ofxTactoBlobMovementInfo>		blobsInsideStain; ///< List of touch IDs inside the shape at any time (for manipulation)
	std::string								m_sNameInfo; ///< The string to be displayed on the stain

	float									polygonInternalAngles(); ///< Returns the sum of all internal angles of the stain, in degrees.
	static float							angleBetweenVectors(float _ax, float _ay, float _bx, float _by); ///< Static function that returns the angle between two vectors.
};

#endif // OFXTACTOSTAIN_H
