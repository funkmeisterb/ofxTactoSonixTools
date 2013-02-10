#ifndef TACTOBLOB_H
#define TACTOBLOB_H

/**
 * \class ofxTactoBlob
 *
 * \brief This is an implementation of a multi-touch blob.
 *
 * \author Bruno Angeles (bruno.angeles@mail.mcgill.ca)
 *
 * \version 1.0
 *
 * \date 2012/08/14
 *
 */

#include "ofMain.h"

/// A class that implements a multi-touch point of contact.
class ofxTactoBlob
{
    public:
        ofxTactoBlob(int _id, float _x, float _y, float _vx, float _vy, float _width, float _height, float _accel); ///< Constructor
		void            draw(); ///< Regular OpenFrameworks function.

        int             id; ///< The ID of the blob.
        float           x; ///< The x coordinate of the blob.
        float           y; ///< The y coordinate of the blob.
        float           vx; ///< The speed along axis x of the blob.
        float           vy; ///< The speed along axis y of the blob.
        float           width; ///< The width of the blob.
        float           height; ///< The height of the blob.
        float           accel; ///< The acceleration of the blob.
        ofColor         color; ///< The colour of the blob.
};

#endif // BLOB_H
