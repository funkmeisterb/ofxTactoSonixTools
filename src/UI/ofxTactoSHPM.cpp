#include "UI/ofxTactoSHPM.h"
#include "TactosonixHelpers.h"
#include "assert.h"

using namespace TactoHelpers;

/**
* \param _rootNode The root node of the menu.
* \param _ptCentre The origin of the menu.
* \param _nWidth The width in pixels of each menu layer.
*/
void ofxTactoSHPM::setup(ofxTactoSHPMNode* _rootNode, ofPoint _ptCentre, int _nWidth)
{
	m_menuRoot = _rootNode;
	m_nMaxDepth = maxDepth(m_menuRoot);
	m_nWidth = _nWidth;
	m_ptOrigin = _ptCentre;

	reset();
}

/** \brief This is a recursive function.
* \param _ptNode The node at which to start the deactivation.
*/
void ofxTactoSHPM::deactivateNodes(ofxTactoSHPMNode* _ptNode)
{
	if (_ptNode->getChildren().size() == 0)
		return;
	else
	{
		_ptNode->setActive(false);
		// The current node is active, let's handle its children
		vector<ofxTactoSHPMNode*>::iterator It;
		vector<ofxTactoSHPMNode*> children = _ptNode->getChildren();
		for (It = children.begin(); It != children.end(); ++It)
		{
			deactivateNodes(*It);
		}
	}
}

/** \brief This is a recursive function.
* \param _ptNode The node at which to start the drag motion.
* \param _x The x coordinate of the motion.
* \param _y The y coordinate of the motion.
*/
ofxTactoSHPMNode* ofxTactoSHPM::dragNodes(ofxTactoSHPMNode* _ptNode, int _x, int _y)
{
	// Point will always be in world coordinates
	ofPoint ptCompare(_x, _y);
    ptCompare.x /= ofGetWidth();
    ptCompare.y /= ofGetHeight();

	if (_ptNode->isPointInside(ptCompare) && _ptNode->isActive() && _ptNode->getChildren().size() == 0)
	{
		// Return this node if the point dragged is inside,
		// the node is a leaf node (it has no children),
		// and it is active.
		return _ptNode;
	}
	else
	{
		vector<ofxTactoSHPMNode*>::iterator It;
		vector<ofxTactoSHPMNode*> children = _ptNode->getChildren();
		for (It = children.begin(); It != children.end(); ++It)
		{
			ofxTactoSHPMNode* draggedChildNode = dragNodes(*It, _x, _y);
			if (draggedChildNode)
			{
				return draggedChildNode;
			}
		}
		return NULL;
	}
}

/** \brief This is a recursive function.
* \param _ptNode The node at which to start the deactivation.
* \param _x The x coordinate of the motion.
* \param _y The y coordinate of the motion.
* \param _fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
*/
ofxTactoSHPMNode* ofxTactoSHPM::getClickedNode(ofxTactoSHPMNode* _ptNode, float _x, float _y, bool _fullRange)
{
	ofPoint ptCompare(_x, _y);
    if (_fullRange)
    {
        ptCompare.x /= ofGetWidth();
        ptCompare.y /= ofGetHeight();
    }

	if (_ptNode->isPointInside(ptCompare))
	{
		// The current node was clicked
		_ptNode->setActive(true);
		return _ptNode;
	}
	else
	{
		// The cursor is not over the current node
		if (_ptNode->isActive())
		{
			bool bActivatedAChild = false;
			ofxTactoSHPMNode* returnedNode = 0;
			// The current node is active, let's handle its children
			vector<ofxTactoSHPMNode*>::iterator It;
			vector<ofxTactoSHPMNode*> children = _ptNode->getChildren();
			for (It = children.begin(); It != children.end(); ++It)
			{
				if (!bActivatedAChild)
				{
					// No child node has yet been activated
					returnedNode = getClickedNode(*It, _x, _y, _fullRange);
					if (returnedNode)
					{
						// This child node was activated, the other ones must be deactivated!
						bActivatedAChild = true;
					}
				}
			}
			if (bActivatedAChild)
			{
				// We have activated a child node, so let's disable the other ones at the same level
				for (It = children.begin(); It != children.end(); ++It)
				{
					if (*It != returnedNode)
					{
						(*It)->setActive(false);
					}
				}
			}
			return returnedNode;
		}
		else
		{
			return 0;
		}
	}
}

/** \brief This is a recursive function.
* \param _ptNode The node at which to start counting.
* \param _currentDepth The depth of the menu level.
* \return The number of active levels of the menu.
*/
int ofxTactoSHPM::countActiveMenuLevels(ofxTactoSHPMNode* _ptNode, int _currentDepth)
{
	if (!_ptNode->isActive())
	{
		return _currentDepth;
	}
	else
	{
		// If the current node is active and has children, we draw the level
		vector<ofxTactoSHPMNode*>::iterator It;
		vector<ofxTactoSHPMNode*> children = _ptNode->getChildren();
		int nSubCount = 1;

		for (It = children.begin(); It != children.end(); ++It)
		{
			ofxTactoSHPMNode* currentNode = *It;
			int nChildCount = countActiveMenuLevels(currentNode, _currentDepth + 1);
			nSubCount = max (nSubCount, nChildCount);
		}

		return nSubCount;
	}
}

/** \brief This is a recursive function.
* \param _ptNode The node at which to start drawing.
* \param _currentDepth The depth of the menu level.
*/
void ofxTactoSHPM::drawNodeLevels(ofxTactoSHPMNode* _ptNode, int _currentDepth)
{
	if (_ptNode->isActive() && _ptNode->getChildren().size() != 0)
	{
		// If the current node is active and has children, we draw the level
		vector<ofxTactoSHPMNode*>::iterator It;
		vector<ofxTactoSHPMNode*> children = _ptNode->getChildren();
		// Linear values for alpha gradient
		int alpha = 255 / m_nMaxDepth;
		ofSetColor(m_menuRoot->getColor(), alpha);
		// Draw donut before drawing nodes
		ofCircle(m_ptOrigin, m_nWidth*(_currentDepth+1));

		for (It = children.begin(); It != children.end(); ++It)
		{
			ofxTactoSHPMNode* currentNode = *It;
			drawNodeLevels(currentNode, _currentDepth + 1);
		}
	}
}

/** \brief This is a recursive function.
* \param _ptNode The node at which to start drawing.
* \param _currentDepth The depth of the menu level.
*/
void ofxTactoSHPM::drawNodes(ofxTactoSHPMNode* _ptNode, int _currentDepth)
{
	if (_ptNode->getChildren().size() == 0)
	{
		// This is a child node
		_ptNode->draw();
		return;
	}
	else
	{
		if (_ptNode->isActive())
		{
			// If the current node is active, we draw its children
			vector<ofxTactoSHPMNode*>::iterator It;
			vector<ofxTactoSHPMNode*> children = _ptNode->getChildren();
			// Linear values for alpha gradient
			int alpha = 255 / m_nMaxDepth;
			ofSetColor(m_menuRoot->getColor(), alpha);
			_ptNode->draw();

			for (It = children.begin(); It != children.end(); ++It)
			{
				ofxTactoSHPMNode* currentNode = *It;
				drawNodes(currentNode, _currentDepth + 1);
			}
		}
		else
		{
			// Otherwise we draw the node itself
			_ptNode->draw();
		}
	}
}

/** \brief This method traverses the node's children and places them based on their depth and
 * the number of children at each level.
* \param _ptNode The root node of the menu.
* \param _currentDepth The depth of the menu level.
*/
void ofxTactoSHPM::placeNodes(ofxTactoSHPMNode* _ptNode, int _currentDepth)
{
	if (_ptNode->getChildren().size() == 0)
		return;
	else
	{
		vector<ofxTactoSHPMNode*>::iterator It;
		vector<ofxTactoSHPMNode*> children = _ptNode->getChildren();
		int nNumChildren = children.size();
		float fSpacingRads = PI / (nNumChildren + 2 - 1);
		int nCount = 0;
		for (It = children.begin(); It != children.end(); ++It)
		{
			ofxTactoSHPMNode* currentNode = *It;

			cartesianCoords relCoords = polToCar(m_nWidth*_currentDepth + m_nWidth/2,
				fSpacingRads * (nCount + 1));
			cartesianCoords absCoords;
			absCoords.x = m_ptOrigin.x + relCoords.x;
			absCoords.y = m_ptOrigin.y - relCoords.y;
			currentNode->setOrigin(ofPoint(absCoords.x, absCoords.y), true);
			currentNode->setRadius(m_nWidth * 0.4f);
			nCount++;
			placeNodes(currentNode, _currentDepth + 1);
		}
	}
}

/** \param _ptNode The root node of the menu.
* \return The depth of the menu.
*/
int	ofxTactoSHPM::maxDepth(ofxTactoSHPMNode* _ptNode)
{
	if (_ptNode->getChildren().size() == 0)
		return 0;
	else
	{
		vector<ofxTactoSHPMNode*>::iterator It;
		vector<ofxTactoSHPMNode*> children = _ptNode->getChildren();
		int nMaxDepth = 0;
		ofxTactoSHPMNode* ptNodeToExplore = *children.begin();
		// Figure out max depth at current level
		for (It = children.begin(); It != children.end(); ++It)
		{
			ofxTactoSHPMNode* currentNode = *It;
			int nCurrentDepth = maxDepth(currentNode);
			if (nCurrentDepth > nMaxDepth)
			{
				nMaxDepth = nCurrentDepth;
				ptNodeToExplore = currentNode;
			}
		}
		return maxDepth(ptNodeToExplore) + 1;
	}
}

void ofxTactoSHPM::draw()
{
	ofFill();
	// First, draw the parent nodes
	drawNodeLevels(m_menuRoot, 1);
	// Then, draw the children nodes
	drawNodes(m_menuRoot, 1);

	// Draw the nodes being dragged
	vector<ofxTactoBeatNode*>::iterator It;
	for (It = m_draggedNodes.begin(); It != m_draggedNodes.end(); ++It)
	{
		(*It)->draw();
	}
}

/**
* \param x The x coordinate of the queried point.
* \param y The y coordinate of the queried point.
* \param nDepth The depth of the queried menu level.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \return Whether or not the queried point is within the menu level.
*/
bool ofxTactoSHPM::isPointInsideRing(int x, int y, int nDepth, bool fullRange)
{
	bool bReturnVal = false;
	ofPoint ptCentre = m_menuRoot->getOrigin();
	float distX = abs(ptCentre.x - (float)x);
	float distY = abs(ptCentre.y - (float)y);
	// The point is inside the ring if its distance to the menu's origin is in a particular range
	float distCentre = sqrt(distX*distX + distY*distY);
	int nInnerRadius = nDepth * m_nWidth;
	int nOuterRadius = (nDepth + 1)* m_nWidth;
	if (distCentre > nInnerRadius && distCentre < nOuterRadius)
		bReturnVal = true;
	return bReturnVal;
}

/**
* \brief This is a recursive function.
*
* \param _ptNode The node to query.
* \param _x The x coordinate of the queried point.
* \param _y The y coordinate of the queried point.
* \param _currentDepth The depth of the menu level.
* \return Whether or not the queried point is within the menu's rings.
*/
bool ofxTactoSHPM::isPointInsideMenuRings(ofxTactoSHPMNode* _ptNode, int _x, int _y, int _currentDepth)
{
	// Point will always be in world coordinates
	ofPoint ptCompare(_x, _y);
    ptCompare.x /= ofGetWidth();
    ptCompare.y /= ofGetHeight();

	if (_ptNode->isPointInside(ptCompare))
	{
		// The current node was clicked
		return true;
	}
	else
	{
		// The cursor is not over the current node
		if (_ptNode->getChildren().size() > 0)
		{
			// The current node is active, was the click on its level?
			if (isPointInsideRing(_x, _y, _currentDepth, true))
			{
				return true;
			}
			else
			{
				bool bInsideChildRing = false;
				vector<ofxTactoSHPMNode*>::iterator It;
				vector<ofxTactoSHPMNode*> children = _ptNode->getChildren();
				for (It = children.begin(); It != children.end(); ++It)
				{
					// No child node has yet been activated
					if (isPointInsideMenuRings(*It, _x, _y, _currentDepth+1))
					{
						// This child node was activated, the other ones must be deactivated!
						bInsideChildRing = true;
						if (bInsideChildRing)
						{
							break;
						}
					}
					else
					{
						(*It)->setActive(false);
					}
				}
				return bInsideChildRing;
			}
		}
		else
		{
			return false;
		}
	}
}

/**
* \param x The x coordinate of the queried point.
* \param y The y coordinate of the queried point.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \return Whether or not the queried point is within the menu's active levels.
*/
bool ofxTactoSHPM::isPointInsideActiveMenu(int x, int y, bool fullRange)
{
	int nNumActiveMenus = countActiveMenuLevels(m_menuRoot, 0);

	for (int i=0; i<nNumActiveMenus + 1; i++)
	{
		if (isPointInsideRing(x, y, i, fullRange))
			return true;
	}
	return false;
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void ofxTactoSHPM::mouseDragged(int x, int y, int button)
{
	mouseTouchMoved(x, y, true, button);
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param button The ID of the mouse button.
*/
void ofxTactoSHPM::mousePressed(int x, int y, int button)
{
	mouseTouchDown(x, y, true, button);
}

/**
* \param w The new width in pixels.
* \param h The new height in pixels.
*/
void ofxTactoSHPM::windowResized(int w, int h)
{
	// Update the centre point
	reset();
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void ofxTactoSHPM::touchDown(float x, float y, int touchId)
{
	mouseTouchDown(x, y, false, 0, touchId);
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void ofxTactoSHPM::touchMoved(float x, float y, int touchId)
{
	mouseTouchMoved(x, y, false, 0, touchId);
}

/** \return The root node of the menu.
*/
ofxTactoSHPMNode* ofxTactoSHPM::getRoot()
{
	return m_menuRoot;
}

void ofxTactoSHPM::reset()
{
	ofPoint ptCentre = ofPoint(ofGetWidth()/2, ofGetHeight());
	m_ptOrigin = ptCentre;
	m_menuRoot->setOrigin(ptCentre, true);
	m_menuRoot->setOriginInit(ptCentre, true);
	m_menuRoot->setRadius(m_nWidth);
	// Update node positions
	placeNodes(m_menuRoot, 1);
}

/** \return The vector of dragged nodes.
*/
vector<ofxTactoBeatNode*>* ofxTactoSHPM::getDraggedNodes()
{
	return &m_draggedNodes;
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \param button The ID of the mouse button, in the case of mouse input.
* \param touchId The ID of the touch event, in the case of touch input.
*/
bool ofxTactoSHPM::mouseTouchDown(float x, float y, bool fullRange, int button, int touchId)
{
    bool bTouchIsInsideMenu = isPointInsideActiveMenu(x, y, fullRange);

	ofxTactoSHPMNode* clickedNode = getClickedNode(m_menuRoot, x, y, fullRange);
	if (!clickedNode)
	{
		if (!isPointInsideActiveMenu(x, y, fullRange))
			deactivateNodes(m_menuRoot);
	}
	else
	{
		if (clickedNode->getChildren().size() == 0)
		{
			// It is a child node
			ofxTactoBeatNode* newDraggedNode = new ofxTactoBeatNode(*(ofxTactoBeatNode*)clickedNode);
			newDraggedNode->setOrigin(ofPoint(x, y), fullRange);
			m_draggedNodes.push_back(newDraggedNode);
		}
	}

	return bTouchIsInsideMenu;
}

/**
* \param x The x coordinate of the point.
* \param y The y coordinate of the point.
* \param fullRange Whether or not the coordinates of the queried point are in pixels (false means [0-1]).
* \param button The ID of the mouse button, in the case of mouse input.
* \param touchId The ID of the touch event, in the case of touch input.
*/
bool ofxTactoSHPM::mouseTouchMoved(float x, float y, bool fullRange, int button, int touchId)
{
    bool bTouchIsInsideMenu = isPointInsideActiveMenu(x, y, fullRange);

    bool bMovedNode = false; // true if we move something

	vector<ofxTactoBeatNode*>::iterator It;
	for (It = m_draggedNodes.begin(); It != m_draggedNodes.end(); ++It)
	{
		(*It)->setOrigin(ofPoint(x, y), fullRange);
		bMovedNode = true;
		break;
	}

	return bMovedNode | bTouchIsInsideMenu;
}
