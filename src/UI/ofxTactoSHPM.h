#ifndef _OF_TACTO_SHPM
#define _OF_TACTO_SHPM

/**
 * \class ofxTactoSHPM
 *
 * \brief A class that implementats Stacked Half-Pie Menus (SHPMs).
 *
 * See http://dl.acm.org/citation.cfm?id=1731936 for the paper by Hesselmann, Floering, and Schmitt.
 *
 * \author Bruno Angeles (bruno.angeles@mail.mcgill.ca)
 *
 * \version 1.0
 *
 * \date 2012/08/14
 *
 */

#include "ofMain.h"
#include "UI/ofxTactoSHPMNode.h"
#include "UI/ofxTactoBeatNode.h"

/// A class that implements a Stacked Half-Pie Menu.
class ofxTactoSHPM : public ofBaseApp
{
public:
	void									setup(ofxTactoSHPMNode* _rootNode, ofPoint _ptCentre, int _nWidth); ///< Override of a regular OpenFrameworks function.
	void									draw(); ///< Regular OpenFrameworks function.
	bool									isPointInsideRing(int x, int y, int nDepth, bool fullRange); ///< Returns true if and only if the point is within the menu level.
	bool									isPointInsideActiveMenu(int x, int y, bool fullRange); ///< Returns true if and only if the point is within the menu's active levels.
	void									mouseDragged(int x, int y, int button); ///< Regular OpenFrameworks function.
	void									mousePressed(int x, int y, int button); ///< Regular OpenFrameworks function.
	void									windowResized(int w, int h); ///< Regular OpenFrameworks function.
	void									touchDown(float x, float y, int touchId); ///< Regular OpenFrameworks function.
	void									touchMoved(float x, float y, int touchId); ///< Regular OpenFrameworks function.
	ofxTactoSHPMNode*						getRoot(); ///< Returns the root node of the Stacked Half-Pie Menu.
	vector<ofxTactoBeatNode*>*				getDraggedNodes(); ///< Returns a vector of dragged nodes.
	bool									mouseTouchDown(float x, float y, bool fullRange, int button = 0, int touchId = 0); ///< A handler function for mouse and touch down events.
	bool									mouseTouchMoved(float x, float y, bool fullRange, int button = 0, int touchId = 0); ///< A handler function for mouse and touch moved events.

private:
	ofxTactoSHPMNode*						m_menuRoot; ///< The root of the menu.
	int										m_nMaxDepth; ///< The max depth of the menu.
	ofPoint									m_ptOrigin; ///< The origin of the menu.
	int										m_nWidth; ///< The width in pixels of each menu layer.
	vector<ofxTactoBeatNode*>				m_draggedNodes; ///< The nodes being dragged.
	
	void									deactivateNodes(ofxTactoSHPMNode* _ptNode); ///< Deactivates a node and its children nodes.
	ofxTactoSHPMNode*						dragNodes(ofxTactoSHPMNode* _ptNode, int _x, int _y); ///< Drag the nodes.
	ofxTactoSHPMNode*						getClickedNode(ofxTactoSHPMNode* _ptNode, float _x, float _y, bool _fullRange); ///< Get the node at the queried position.
	int										countActiveMenuLevels(ofxTactoSHPMNode* _ptNode, int _currentDepth); ///< Returns the number of active levels of the menu.
	void									drawNodeLevels(ofxTactoSHPMNode* _ptNode, int _currentDepth); ///< Draws the levels of the menu.
	void									drawNodes(ofxTactoSHPMNode* _ptNode, int _currentDepth); ///< Draws the nodes of the menu.
	void									placeNodes(ofxTactoSHPMNode* _ptNode, int _currentDepth); ///< Places the nodes of the menu during initialization.
	static int								maxDepth(ofxTactoSHPMNode* _ptNode); ///< Static function that returns the depth of a menu.
	bool									isPointInsideMenuRings(ofxTactoSHPMNode* _ptNode, int _x, int _y, int _currentDepth); ///< Returns true if and only if the queried point is inside the rings of the menu.
	void									reset(); ///< Reconfigures the menu when something changes.
};

#endif
