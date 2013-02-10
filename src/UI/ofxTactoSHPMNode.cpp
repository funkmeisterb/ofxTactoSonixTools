#include "UI/ofxTactoSHPMNode.h"
#include "UI/ofxTactoSHPM.h"
#include "assert.h"

using namespace TactoHelpers;

/** \param _color The colour of the node.
* \param _type The type of musical loop that the node represents.
*/
ofxTactoSHPMNode::ofxTactoSHPMNode(ofColor _color, TACTO_LOOPTYPE _type) :
m_nType(_type), m_bActive(false), m_nColor(_color), m_children(0), m_ptOrigin(ofPoint(0, 0)), m_nRadius(20)
{
	int numVertices = 0;
	switch (m_nType)
	{
		case TACTO_LOOPTYPE_DRUMS:
			numVertices = 4;
			break;
		case TACTO_LOOPTYPE_BASS:
			numVertices = 5;
			break;
		case TACTO_LOOPTYPE_LEAD:
			numVertices = 6;
			break;
		default:
			break;
	}
	
	float fSpacingRads = TWO_PI / numVertices;
	for (int i=0; i<numVertices; i++)
	{
		cartesianCoords relCoords = polToCar(getRadius(), fSpacingRads * (i + 1));
		ofPoint currVertexPos(relCoords.x, relCoords.y);
		m_vertices.push_back(&currVertexPos);
	}
}

/** \param _pChild The child node to add.
*/
void ofxTactoSHPMNode::addChild(ofxTactoSHPMNode* _pChild)
{
	_pChild->setRadius(m_nRadius);
	m_children.push_back(_pChild);
}

/** \return The vector of children nodes.
*/
vector<ofxTactoSHPMNode*> ofxTactoSHPMNode::getChildren()
{
	return m_children;
}

/**
* \param _active Whether or not the node is active (deployed).
*/
void ofxTactoSHPMNode::setActive(bool _active)
{
	m_bActive = _active;
}

/**
* \return Whether or not the button is active (pushed down).
*/
bool ofxTactoSHPMNode::isActive()
{
	return m_bActive;
}

/**
* \param pt The point to query.
*/
bool ofxTactoSHPMNode::isPointInside(ofPoint pt)
{
	bool returnValue = false;

	// pt will always be in [0;1], while the origin and most importantly radius will be in world coordinates
	pt.x *= ofGetWidth();
	pt.y *= ofGetHeight();

	if (m_nType == TACTO_LOOPTYPE_NONE)
	{
		// Simple circle
		float fDistance = sqrt(pow(pt.x - m_ptOrigin.x, 2.0f) + pow(pt.y - m_ptOrigin.y, 2.0f));
		if (fDistance <= m_nRadius)
		{
			returnValue = true;
		}
	}
	else
	{
		int i, j;
		// Vertices
		for (i = 0, j = m_vertices.size()-1; i < m_vertices.size(); j = i++) {
			if ((((m_ptOrigin.y + m_vertices[i]->y <= pt.y) && (pt.y < m_ptOrigin.y + m_vertices[j]->y)) ||
				((m_ptOrigin.y + m_vertices[j]->y <= pt.y) && (pt.y < m_ptOrigin.y + m_vertices[i]->y))) &&
				(pt.x < (m_vertices[j]->x - m_vertices[i]->x) * (pt.y - m_ptOrigin.y - m_vertices[i]->y) / (m_vertices[j]->y - m_vertices[i]->y) + m_ptOrigin.x + m_vertices[i]->x))
				returnValue = !returnValue;
		}
	}

	return returnValue;
}

void ofxTactoSHPMNode::draw()
{
	int alpha = 75;
	//if (m_bActive)
	//	alpha = 255;
	ofSetColor(m_nColor, alpha);
	ofSetLineWidth(0.0f);
	
	// Two cases: not a loop, and is a loop
	if (m_nType == TACTO_LOOPTYPE_NONE)
	{
		ofCircle(getOrigin(), getRadius());
	}
	else
	{
		if (m_vertices.size() > 0)
		{
			ofBeginShape();
			for (int i=0; i<m_vertices.size(); i++)
			{
				ofVertex(m_vertices[i]->x + getOrigin().x, m_vertices[i]->y + getOrigin().y);
			}
			ofEndShape();
		}
	}
}

/**
* \return The point of origin of the node.
*/
ofPoint ofxTactoSHPMNode::getOrigin()
{
	return m_ptOrigin;
}

/**
* \param _origin The point of origin of the node.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
*/
void ofxTactoSHPMNode::setOrigin(ofPoint _origin, bool fullRange)
{
	if (fullRange)
	{
		m_ptOrigin = _origin;
	}
	else
	{
		m_ptOrigin = ofPoint(_origin.x * ofGetWidth(), _origin.y * ofGetHeight());
	}
	
	// Refresh the vertices
	int numVertices = m_vertices.size();
	m_vertices.clear();
	float fSpacingRads = TWO_PI / numVertices;
	for (int i=0; i<numVertices; i++)
	{
		cartesianCoords relCoords = polToCar(getRadius(), fSpacingRads * (i + 1));
		ofPoint* currVertexPos = new ofPoint(relCoords.x, relCoords.y);
		m_vertices.push_back(currVertexPos);
	}
}

/**
* \return The radius in pixels of the node.
*/
int ofxTactoSHPMNode::getRadius()
{
	return m_nRadius;
}

/**
* \param _nRadius The radius of the node, in pixels.
*/
void ofxTactoSHPMNode::setRadius(int _nRadius)
{
	m_nRadius = _nRadius;
}

/**
* \return The point of origin of the node when it was created.
*/
ofPoint ofxTactoSHPMNode::getOriginInit()
{
	return m_ptOriginalPosition;
}

/**
* \param _origin The point of origin of the node.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
*/
void ofxTactoSHPMNode::setOriginInit(ofPoint _origin, bool fullRange)
{
	if (!fullRange)
	{
		_origin.x *= ofGetWidth();
		_origin.y *= ofGetHeight();
	}

	m_ptOriginalPosition = _origin;
}