#ifndef _OF_TACTO_SHPMNODE
#define _OF_TACTO_SHPMNODE

/**
 * \class ofxTactoSHPMNode
 *
 * \brief A class that implementats a node within a Stacked Half-Pie Menu (\link ofxTactoSHPM).
 *
 * \author Bruno Angeles (bruno.angeles@mail.mcgill.ca)
 *
 * \version 1.0
 *
 * \date 2012/08/14
 *
 */

#include "ofMain.h"
#include "TactosonixHelpers.h"
using namespace TactoHelpers;

/// A class that implements nodes in a Stacked Half-Pie Menu.
class ofxTactoSHPMNode : public ofBaseApp
{
public:
	ofxTactoSHPMNode(ofColor _color, TACTO_LOOPTYPE _type = TACTO_LOOPTYPE_NONE); ///< Constructor
	ofxTactoSHPMNode() :
		m_nColor(0), m_bActive(false), m_nType(TACTO_LOOPTYPE_DRUMS) {}; ///< Default constructor
	void									addChild(ofxTactoSHPMNode* _pChild); ///< Adds a child node.
	ofColor									getColor() { return m_nColor; } ///< Returns the colour of the node. \return The colour of the node.
	vector<ofxTactoSHPMNode*>				getChildren(); ///< Returns the vector of children of the node.
	bool									isActive(); ///< Returns true if and only if the node is active.
	void									setActive(bool _active); ///< Makes the node active or not. \param _active Whether or not the node should be active.
	bool									isPointInside(ofPoint pt); ///< Returns true if and only if the specified coordinates are within the button.
	void									draw(); ///< Draws the node.
	ofPoint									getOrigin(); ///< Returns the point of origin of the node, including the dragged offset..
	void									setOrigin(ofPoint _origin, bool fullRange); ///< Sets the point of origin of the node, including the dragged offset..
	ofPoint									getOriginInit(); ///< Returns the point of origin of the node, without the dragged offset.
	void									setOriginInit(ofPoint _origin, bool fullRange); ///< Sets the point of origin of the node, without the dragged offset.
	int										getRadius(); ///< Returns the radius in pixels of the node.
	void									setRadius(int _nRadius); ///< Sets the radius in pixels of the node.
	
private:
	TACTO_LOOPTYPE							m_nType; ///< The type of loop that the node represents.
	ofColor									m_nColor; ///< The colour of the menu node.
	vector<ofxTactoSHPMNode*>				m_children; ///< The children nodes below this node.
	bool									m_bActive; ///< Whether or not the node is activated.
	ofPoint									m_ptOrigin; ///< The point of origin of the node, including dragging motion.
	ofPoint									m_ptOriginalPosition; ///< The original position of the node.
	int										m_nRadius; ///< The radius of the node.
	vector<ofPoint*>						m_vertices; ///< The vertex coordinates of the shape, relative to the origin.
};

#endif