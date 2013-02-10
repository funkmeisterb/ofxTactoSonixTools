#ifndef _OF_TACTO_BEATNODE
#define _OF_TACTO_BEATNODE

/**
 * \class ofxTactoBeatNode
 *
 * \brief This is an implementation of a touch-enabled musical node within a Stacked Half-Pie Menu (\link ofxTactoSHPM).
 *
 * \author Bruno Angeles (bruno.angeles@mail.mcgill.ca)
 *
 * \version 1.0
 *
 * \date 2012/08/14
 *
 */

#include "UI/ofxTactoSHPMNode.h"

/// A class that implements musical nodes in a stacked half-pie menu (\link ofxTactoSHPM).
class ofxTactoBeatNode : public ofxTactoSHPMNode
{
public:
	ofxTactoBeatNode(ofColor _color, string _fullFilePath, int _lifeTime, TACTO_LOOPTYPE _type, int _lengthBeats); ///< Constructor
	ofxTactoBeatNode(); ///< Default constructor

	string									getFullFilePath() { return m_sFullFilePath; } ///< Returns the full path of the loop.
	int										getLifeTime() { return m_nLifeTimeMs; } ///< Returns the lifetime in ms of the loop.
	TACTO_LOOPTYPE							getLoopType() { return m_nLoopType; } ///< Returns the type of loop.
	int										getLoopLength() { return m_nLengthBeats; } ///< Returns the length of the loop, in beats.

private:
	string									m_sFullFilePath; ///< The full path of the file.
	int										m_nLifeTimeMs; ///< The lifetime in ms of the beat.
	int										m_nLengthBeats; ///< The length of the loop, in beats.
	TACTO_LOOPTYPE							m_nLoopType; ///< The type of loop that the node represents.
};

#endif