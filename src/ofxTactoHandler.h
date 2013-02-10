#ifndef TACTOHANDLER_H
#define TACTOHANDLER_H

/**
 * \class ofxTactoHandler
 *
 * \brief 
 *
 * \author Bruno Angeles (bruno.angeles@mail.mcgill.ca)
 *
 * \version 1.0
 *
 * \date 2012/08/14
 *
 */

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxTactoBlob.h"

#define TUIOPORT 3333

enum BLOB_METHOD { NAIVE, CLOSEST };

/// A class that provides support for the TUIO protocol.
class ofxTactoHandler : public ofBaseApp
{
    public:
		void                setup(); ///< Regular OpenFrameworks function.
		void                update(); ///< Regular OpenFrameworks function.
		void                drawBlobs(); ///< Draws the blobs.
		int                 numBlobs(); ///< Returns the number of blobs in the system.
		list<ofxTactoBlob>	getBlobs(); ///< Returns a list of the blobs in the system.

        void				touchDown(float x, float y, int touchId); ///< Regular OpenFrameworks function.
        void				touchMoved(float x, float y, int touchId); ///< Regular OpenFrameworks function.
        void				touchUp(float x, float y, int touchId); ///< Regular OpenFrameworks function.
        void				touchDoubleTap(float x, float y, int touchId); ///< Regular OpenFrameworks function.

    private:
        void                ManageBlobs(ofxOscMessage m); ///< Updates the list of active blobs.
        bool                IsBlobRegistered(int blobID); ///< Returns true if and only if the queried ID is in the current list of blobs.
        static bool         IsBlobIDInVector(int blobID, vector<int> vectorIDs); ///< Static function that returns true if and only if the queried ID is in the queried vector.
        void                AppendBlob(ofxTactoBlob newblob); ///< Appends a blob to the list of blobs.

        ofxOscReceiver      tuiorcvr; ///< The OSC packet listener that reads TUIO data.
        list<ofxTactoBlob>  listBlobs; ///< The list of blobs in the touch system.
};

#endif // TACTOHANDLER_H
