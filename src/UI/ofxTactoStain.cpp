#include "UI/ofxTactoStain.h"

//------------------------------------------------------------------
/** \param _nVertices The number of vertices in the stain.
* \param _name The name of the stain.
* \param _fRadius The radius of the stain to be initially created.
*/
void ofxTactoStain::setup(int _nVertices, string _name, float _fRadius)
{
	// From input
	m_fRadius = _fRadius;
	m_nNumVertices = _nVertices;

	// Default values
	m_nColor1 = 0x0A0A0A;
	m_nColor2 = 0xEEEEEE;
	m_nCurrentColor = m_nColor1;
	m_fOriX = 0.0f;
	m_fOriY = 0.0f;
	m_fInitialSize = 1;
	reset();
	m_fInitialSize = area();
	m_fVertexRadius = 0.04f;
	m_bActive = false;
	m_bInMotion = false;
	m_bMovable = true;
	m_bScalable = true;
	m_bShapable = true;
	m_nTimeFirstFinger = 0;
	m_fOriAngle = 0;

	m_nameText.init("fonts/arial.ttf", 20);
	m_sNameInfo = _name;
	m_nameText.setText(m_sNameInfo);
	m_nameText.wrapTextX(ofGetWidth() * 3/4);
}

/** \param _bActive The value to assign.
*/
void ofxTactoStain::setActive(bool _bActive)
{
	m_bActive = _bActive;
}

/** \param _bMovable The value to assign.
*/
void ofxTactoStain::setMovable(bool _bMovable)
{
	m_bMovable = _bMovable;
}

/** \param _bScalable The value to assign.
*/
void ofxTactoStain::setScalable(bool _bScalable)
{
	m_bScalable = _bScalable;
}

/** \param _bScalable The value to assign.
*/
void ofxTactoStain::setShapable(bool _bShapable)
{
	m_bShapable = _bShapable;
}

/** \param _nLowGradientColor The lower limit of the colour gradient.
* \param _nHighGradientColor The higher limit of the colour gradient.
*/
void ofxTactoStain::setColors(int _nLowGradientColor, int _nHighGradientColor)
{
	m_nColor1 = _nLowGradientColor;
	m_nColor2 = _nHighGradientColor;
}

/**
* \return True if and only if the stain is alive
*/
bool ofxTactoStain::isActive()
{
	return m_bActive;
}

/** \return True if and only if the stain is in motion
*/

bool ofxTactoStain::isInMotion()
{
	return m_bInMotion;
}

/**
* \note The stain's origin is not (necessarily) its centre of mass. It is just a reference point from which
* the positions of the vertices are computed.
* \param _fOriX The x coordinate of the stain's origin
* \param _fOriY The y coordinate of the stain's origin
*/
void ofxTactoStain::setOrigin(float _fOriX, float _fOriY)
{
	m_fOriX = _fOriX;
	m_fOriY = _fOriY;
}

/** \return The point of origin of the stain.
*/
ofPoint ofxTactoStain::getOrigin()
{
	return ofPoint(m_fOriX, m_fOriY, 0);
}

/** \param _fGradient A value in [0;1] in which 0 is mapped to m_nColor1 and 1 to m_nColor2
*/
void ofxTactoStain::setColorGradient(float _fGradient)
{
	int R1 = m_nColor1 & 0xFF0000;
	R1 /= pow(2.0, 16);
	int R2 = m_nColor2 & 0xFF0000;
	R2 /= pow(2.0, 16);

	int G1 = m_nColor1 & 0x00FF00;
	G1 /= pow(2.0, 8);
	int G2 = m_nColor2 & 0x00FF00;
	G2 /= pow(2.0, 8);

	int B1 = m_nColor1 & 0x0000FF;
	int B2 = m_nColor2 & 0x0000FF;

	int Rcurrent = (R2 - R1) * _fGradient + R1;
	Rcurrent *= pow(2.0, 16);
	int Gcurrent = (G2 - G1) * _fGradient + G1;
	Gcurrent *= pow(2.0, 8);
	int Bcurrent = (B2 - B1) * _fGradient + B1;
	m_nCurrentColor = Rcurrent | Gcurrent | Bcurrent;
	m_nCurrentColor = max(m_nCurrentColor, 0);
}

void ofxTactoStain::reset()
{
	// All positions are relative to the (oriX, oriY) point
	for (int i = 0; i < m_nNumVertices; i++)
	{
		// Spread out in an even circle
		float currentAngle = 2 * PI * i / m_nNumVertices;
		vertices[i].x = m_fRadius * cos(currentAngle);
		vertices[i].y = m_fRadius * sin(currentAngle);
	}
}

void ofxTactoStain::update()
{
	m_fSpikiness = spikiness();
	m_fArea = area();
	m_nCounter += 1;
	if (m_nCounter >= MAX_COUNT)
		m_nCounter = 0;
}

void ofxTactoStain::draw()
{
	if (m_bActive)
	{
		ofFill();
		ofSetHexColor(m_nCurrentColor);
		ofBeginShape();

		for (int i = 0; i < m_nNumVertices; i++)
		{
			// sorry about all the if/states here, but to do catmull rom curves
			// we need to duplicate the start and end points so the curve acutally
			// goes through them.

			// for i == 0, we just call the vertex twice
			// for i == nCurveVertexes-1 (last point) we call vertex 0 twice
			// otherwise just normal ofCurveVertex call
			if (i == 0)
			{
				ofCurveVertex((m_fOriX + vertices[0].x) * ofGetWidth(), (m_fOriY + vertices[0].y) * ofGetHeight()); // we need to duplicate 0 for the curve to start at point 0
				ofCurveVertex((m_fOriX + vertices[0].x) * ofGetWidth(), (m_fOriY + vertices[0].y) * ofGetHeight()); // we need to duplicate 0 for the curve to start at point 0
			}
			else if (i == m_nNumVertices - 1)
			{
				ofCurveVertex((m_fOriX + vertices[i].x) * ofGetWidth(), (m_fOriY + vertices[i].y) * ofGetHeight());
				ofCurveVertex((m_fOriX + vertices[0].x) * ofGetWidth(), (m_fOriY + vertices[0].y) * ofGetHeight());	// to draw a curve from pt m_nNumVertices-1 to pt 0
				ofCurveVertex((m_fOriX + vertices[0].x) * ofGetWidth(), (m_fOriY + vertices[0].y) * ofGetHeight());	// we duplicate the first point twice
			}
			else
			{
				ofCurveVertex((m_fOriX + vertices[i].x) * ofGetWidth(), (m_fOriY + vertices[i].y) * ofGetHeight());
			}
		}
		ofEndShape();
		// show a faint the non-curve version of the same polygon:
		ofNoFill();
		ofSetColor(0,0,128,40);
		ofBeginShape();
		for (int i = 0; i < m_nNumVertices; i++)
		{
			ofVertex((m_fOriX + vertices[i].x) * ofGetWidth(), (m_fOriY + vertices[i].y) * ofGetHeight());
		}
		ofEndShape(true);
		ofSetColor(0,0,0,80);
		for (int i = 0; i < m_nNumVertices; i++)
		{
			if (vertices[i].bBeingDragged == true) ofFill();
			else ofNoFill();
			ofCircle((m_fOriX + vertices[i].x) * ofGetWidth(), (m_fOriY + vertices[i].y) * ofGetHeight(), m_fVertexRadius);
		}

		// Draw the name of the stain
		std::string sToDisplay = m_sNameInfo + " [" + ofToString(m_fOriX) + "; " + ofToString(m_fOriY) + "] Spikiness=" + ofToString(m_fSpikiness) + " Area=" + ofToString(m_fArea);
		m_nameText.setText(sToDisplay);
		m_nameText.setColor(189, 189, 189, 255);
		m_nameText.drawCenter(m_fOriX * ofGetWidth(), m_fOriY * ofGetHeight());
	}
}

/**
* \return The area of the stain, relative to its initial area
*/
float ofxTactoStain::area()
{
	// based on http://www.alienryderflex.com/polygon_area/
	float area = 0.;
	int j = 0;

	for (int i = 0; i < m_nNumVertices; i++)
	{
		j++;
		if (j == m_nNumVertices)
			j=0;
		area += (vertices[i].x + vertices[j].x) * (vertices[i].y - vertices[j].y);
	}

	float fReturnValue = area * .5 / m_fInitialSize;
	return fReturnValue;
}

/**
* \return The "spikiness" of the stain.
*/
float ofxTactoStain::spikiness()
{
	return 1 - polygonInternalAngles() / ((m_nNumVertices - 2) * 180);
}

float ofxTactoStain::polygonInternalAngles()
{
	// a polygon with n sides has a sum of internal angles equal to (n-2) × 180°
	float spikiness = 0.;
	int j = 0;
	int k = m_nNumVertices - 2;

	for (int i = 0; i < m_nNumVertices; i++)
	{
		j++;
		k++;
		// Take care of bounds
		if (j == m_nNumVertices)
			j = 0;
		if (k == m_nNumVertices)
			k = 0;
		// Consider angle at point i, so vectors v1 from pt i to pt j and v2 from pt i to pt k
		// A * B = v1_x * v2_x + v1_y * v2_x
		float v1_x = vertices[j].x - vertices[i].x;
		float v1_y = vertices[j].y - vertices[i].y;
		float v1_len = sqrt (v1_x * v1_x + v1_y * v1_y);
		float v2_x = vertices[k].x - vertices[i].x;
		float v2_y = vertices[k].y - vertices[i].y;
		float v2_len = sqrt (v2_x * v2_x + v2_y * v2_y);
		float cosTheta = (v1_x * v2_x + v1_y * v2_y) / (v1_len * v2_len);
		float theta = 0;
		if (cosTheta == 1)
			theta = 0;
		else if (cosTheta == 0)
			theta = PI / 2;
		else
			theta = acos(cosTheta);
		float thetaDeg = theta * 180 / PI;

		spikiness += thetaDeg;
	}

	return spikiness;
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void ofxTactoStain::mouseDragged(int x, int y, int button)
{
	if (m_bActive)
	{
		for (int i = 0; i < m_nNumVertices; i++)
		{
			if (vertices[i].bBeingDragged == true)
			{
				vertices[i].x = (float)x/ofGetWidth() - m_fOriX;
				vertices[i].y = (float)y/ofGetHeight() - m_fOriY;
			}
		}

		if (m_bInMotion && m_bMovable)
		{
			m_fOriX = m_PtMotionOrigin.x + (float)x/ofGetWidth() - m_PtMotionStart.x;
			m_fOriY = m_PtMotionOrigin.y + (float)y/ofGetHeight() - m_PtMotionStart.y;
		}
	}
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void ofxTactoStain::mousePressed(int x, int y, int button)
{
	if (m_bActive)
	{
		bool bGrabbedVertex = false;
		// Move vertices
		for (int i = 0; i < m_nNumVertices; i++)
		{
			float diffx = (float)x/ofGetWidth() - (m_fOriX + vertices[i].x);
			float diffy = (float)y/ofGetHeight() - (m_fOriY + vertices[i].y);
			float dist = sqrt(diffx*diffx + diffy*diffy);
			if (dist < m_fVertexRadius)
			{
				vertices[i].bBeingDragged = true;
				bGrabbedVertex = true;
				break; // Do not allow several vertices to be grabbed at the same time
			}
			else
				vertices[i].bBeingDragged = false;
		}

		if (!bGrabbedVertex) // No vertex was moved, so see if we move/rotate/scale the stain
		{
			// Look for fingers inside the shape
			if (isPointInside(x, y, true))
			{
				m_bInMotion = true;
				m_PtMotionStart.x = (float)x/ofGetWidth();
				m_PtMotionStart.y = (float)y/ofGetHeight();
				m_PtMotionOrigin.x = m_fOriX;
				m_PtMotionOrigin.y = m_fOriY;
			}
		}
	}
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void ofxTactoStain::mouseReleased(int x, int y, int button)
{
	if (m_bActive)
	{
		for (int i = 0; i < m_nNumVertices; i++)
		{
			vertices[i].bBeingDragged = false;
		}
		if (m_bInMotion)
		{
			m_bInMotion = false;
		}
	}
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void ofxTactoStain::touchDown(float x, float y, int touchId)
{
	bool bGrabbedVertex = false;
	if (m_bActive)
	{
		if(m_bShapable)
		{
			// Lock the first item we find
			for (int i = 0; i < m_nNumVertices; i++)
			{
				float diffx = x - (m_fOriX + vertices[i].x);
				float diffy = y - (m_fOriY + vertices[i].y);
				float dist = sqrt(diffx*diffx + diffy*diffy);
				if (dist < m_fVertexRadius)
				{
					vertices[i].bBeingDragged = true;
					bGrabbedVertex = true;
					break; // Do not allow several vertices to be grabbed at the same time
				}
				else
					vertices[i].bBeingDragged = false;
			}
		}

		if (!bGrabbedVertex) // did not find a vertex to grab
		{
			// Look for fingers inside the shape
			if (isPointInside(x, y, false))
			{
				// The current position is inside the shape
				m_bInMotion = true;
				m_PtMotionStart.x = x;
				m_PtMotionStart.y = y;
				m_PtMotionOrigin.x = m_fOriX;
				m_PtMotionOrigin.y = m_fOriY;

				ofxTactoBlobMovementInfo toPush;
				toPush.ID = touchId;
				toPush.xOri = x;
				toPush.x = x;
				toPush.yOri = y;
				toPush.y = y;
				blobsInsideStain.push_back(toPush);

				if (blobsInsideStain.size() == 1)
				{
					m_nTimeFirstFinger = ofGetElapsedTimeMillis();
#ifdef _DEBUG
					std::cout << "First finger just landed!\n";
#endif
				}
				else if (blobsInsideStain.size() == 2)
				{
					// Compute initial angle
					std::list<ofxTactoBlobMovementInfo>::iterator firstPoint = blobsInsideStain.begin();
					std::list<ofxTactoBlobMovementInfo>::iterator secondPoint = firstPoint++;
					float deltaX = secondPoint->xOri - firstPoint->xOri;
					float deltaY = secondPoint->yOri - firstPoint->yOri;
					m_fOriAngle = atan2f(deltaX, deltaY); // radians
					//m_fOriAngle = angleBetweenVectors(firstPoint->xOri, firstPoint->yOri, secondPoint->xOri, secondPoint->yOri);
				}

				// We need to remember the vertex positions
				for (int i = 0; i < m_nNumVertices; i++)
				{
					vertices[i].xOriScale = vertices[i].x;
					vertices[i].yOriScale = vertices[i].y;
				}
			}
		}
	}
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void ofxTactoStain::touchMoved(float x, float y, int touchId)
{
	if (m_bActive)
	{
		for (int i = 0; i < m_nNumVertices; i++)
		{
			if (vertices[i].bBeingDragged == true){
				vertices[i].x = x - m_fOriX;
				vertices[i].y = y - m_fOriY;
			}
		}

		if (m_bInMotion && m_bMovable)
		{
			m_fOriX = m_PtMotionOrigin.x + x - m_PtMotionStart.x;
			m_fOriY = m_PtMotionOrigin.y + y - m_PtMotionStart.y;
		}
	}

	std::list<ofxTactoBlobMovementInfo>::iterator it = std::find(blobsInsideStain.begin(),
		blobsInsideStain.end(), touchId);
	if (it != blobsInsideStain.end())
	{
		if (touchId == it->ID)
		{
			// Update the position in this structure
			it->x = x;
			it->y = y;
		}
	}

	// The moving finger was inside the stain
	if (blobsInsideStain.size() == 2 && m_bScalable)
	{
		std::list<ofxTactoBlobMovementInfo>::iterator firstPoint = blobsInsideStain.begin();
		std::list<ofxTactoBlobMovementInfo>::iterator secondPoint = firstPoint++;

		// distance between the 2 points when they landed on the stain
		float initialDistance = sqrt(pow(secondPoint->xOri - firstPoint->xOri, 2) +
			pow(secondPoint->yOri - firstPoint->yOri, 2));
		// current distance between the 2 points
		float currentDistance = sqrt(pow(secondPoint->x - firstPoint->x, 2) +
			pow(secondPoint->y - firstPoint->y, 2));
		float scaleFactor = currentDistance / initialDistance;

		// We need normalized vectors
		float currentAngle = angleBetweenVectors(firstPoint->x, firstPoint->y, secondPoint->x, secondPoint->y);

		float deltaX = secondPoint->x - firstPoint->x;
		float deltaY = secondPoint->y - firstPoint->y;
		float angle2 = atan2f(deltaX, deltaY);
		float angleChange = angle2 - m_fOriAngle; // radians

		// Now scale and rotate the shape based on this factor
		for (int i = 0; i < m_nNumVertices; i++)
		{
			// Scale & rotate
			float x1 = vertices[i].xOriScale;
			float y1 = vertices[i].yOriScale;
			float radius = sqrtf(x1*x1 + y1*y1);
			float vertexOriAngle = atan2f(y1, x1);
			float vertexNewAngle = vertexOriAngle - angleChange;
			float x2 = radius * cosf(vertexNewAngle);
			float y2 = radius * sinf(vertexNewAngle);
			vertices[i].x = x2 * scaleFactor;
			vertices[i].y = y2 * scaleFactor;
		}
	}
}

/** \brief A method that returns the angle between two vectors.
* \param _ax The x position of the first vector.
* \param _ay The y position of the first vector.
* \param _bx The x position of the second vector.
* \param _by The y position of the second vector.
* \return The value in radians of the angle.
*/
float ofxTactoStain::angleBetweenVectors(float _ax, float _ay, float _bx, float _by)
{
   float dotproduct, lengtha, lengthb, result;

   dotproduct = _ax * _bx + _ay * _by;
   lengtha = sqrt(_ax * _ax + _ay * _ay);
   lengthb = sqrt(_bx * _bx + _by * _by);

   result = acos(dotproduct / (lengtha * lengthb));

   if(dotproduct < 0)
   {
      if(result > 0)
         result += PI;
      else
         result -= PI;
   }
   return result;
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void ofxTactoStain::touchUp(float x, float y, int touchId)
{
	if (m_bActive)
	{
		// Unlock corresponding vertex
		for (int i = 0; i < m_nNumVertices; i++)
		{
			float diffx = x - (m_fOriX + vertices[i].x);
			float diffy = y - (m_fOriY + vertices[i].y);
			float dist = sqrt(diffx*diffx + diffy*diffy);
			if (dist < m_fVertexRadius)
			{
				vertices[i].bBeingDragged = false;
				break;
			}
		}

		// Kill motion
		if (isPointInside(x, y, false))
		{
			// TODO: handle multiple fingers
			m_bInMotion = false;
		}
	}

	// clean up the list of blob IDs inside the shape
	std::list<ofxTactoBlobMovementInfo>::iterator it = std::find(blobsInsideStain.begin(),
		blobsInsideStain.end(), touchId);
	if (it != blobsInsideStain.end())
	{
		blobsInsideStain.erase(it);
	}
}

/** \note See http://paulbourke.net/geometry/insidepoly/ for reference.
* \param x The x coordinate of the point to evaluate
* \param y The y coordinate of the point to evaluate
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
*/
bool ofxTactoStain::isPointInside(float x, float y, bool fullRange)
{
	int i, j;
	bool inShape = false;
	float xWorld = x;
	float yWorld = y;

	if (fullRange)
	{
		xWorld /= ofGetWidth();
		yWorld /= ofGetHeight();
	}
	for (i = 0, j = m_nNumVertices-1; i < m_nNumVertices; j = i++) {
		if ((((m_fOriY + vertices[i].y <= yWorld) && (yWorld < m_fOriY + vertices[j].y)) ||
			((m_fOriY + vertices[j].y <= yWorld) && (yWorld < m_fOriY + vertices[i].y))) &&
			(xWorld < (vertices[j].x - vertices[i].x) * (yWorld - m_fOriY - vertices[i].y) / (vertices[j].y - vertices[i].y) + m_fOriX + vertices[i].x))
			inShape = !inShape;
	}
	return inShape;
}

/** \brief This function determines whether or not the queried position is close to the stain.
This is determined by figuring out whether or not the position is close to the vertices, or inside the shape.
* \param x The x coordinate of the point to evaluate
* \param y The y coordinate of the point to evaluate
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
*/
bool ofxTactoStain::isPointClose(float x, float y, bool fullRange)
{
	int i, j;
	bool closeToShape = false;
	float xWorld = x;
	float yWorld = y;

	if (fullRange) // the input values are [0;ofGetWidth()], not [0;1]
	{
		xWorld /= ofGetWidth();
		yWorld /= ofGetHeight();
	}
	for (i = 0; i < m_nNumVertices; i++)
	{
		float diffx = xWorld - (m_fOriX + vertices[i].x);
		float diffy = yWorld - (m_fOriY + vertices[i].y);
		float dist = sqrt(diffx*diffx + diffy*diffy);
		if (dist < m_fVertexRadius)
		{
			closeToShape = true;
			break;
		}
	}
	// If the point is inside the shape, it is close to it!
	return closeToShape || isPointInside(x, y, fullRange);
}

/** \return The number of blobs currently inside the stain.
*/
int	ofxTactoStain::getNumPointsInside()
{
	return blobsInsideStain.size();
}

/** \return A time value in milliseconds representing the time passed since the first finger hit the stain..
*/
int	ofxTactoStain::getTimeSinceFirstFinger()
{
	return ofGetElapsedTimeMillis() - m_nTimeFirstFinger;
}
