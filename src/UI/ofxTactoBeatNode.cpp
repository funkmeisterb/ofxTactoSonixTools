#include "UI/ofxTactoBeatNode.h"
#include "UI/ofxTactoSHPM.h"
#include "assert.h"

/**
* \param _color The colour of the node.
* \param _fullFilePath The full path of the file.
* \param _lifeTime The lifetime in ms of the beat.
* \param _type The type of loop.
* \param _lengthBeats The length of the loop (in number of beats)
*/
ofxTactoBeatNode::ofxTactoBeatNode(ofColor _color, string _fullFilePath, int _lifeTime, TACTO_LOOPTYPE _type, int _lengthBeats) :
	ofxTactoSHPMNode(_color, _type), m_sFullFilePath(_fullFilePath), m_nLifeTimeMs(_lifeTime), 
		m_nLoopType(_type), m_nLengthBeats(_lengthBeats)
{
}

/**
* \brief The default node has a beat length of 4, an infinite lifetime, and is green. */
ofxTactoBeatNode::ofxTactoBeatNode() :
	ofxTactoSHPMNode(0x00FF00, TACTO_LOOPTYPE_NONE), m_sFullFilePath(""), 
		m_nLifeTimeMs(-1), m_nLoopType(TACTO_LOOPTYPE_NONE), m_nLengthBeats(4)
{
}
