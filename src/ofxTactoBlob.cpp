#include "ofxTactoBlob.h"

/** \param _id The ID of the blob.
* \param _x The x coordinate of the blob.
* \param _y The y coordinate of the blob.
* \param _vx The speed along axis x of the blob.
* \param _vy The speed along axis y of the blob.
* \param _width The width of the blob.
* \param _height The height of the blob.
* \param _accel The acceleration of the blob.
*/
ofxTactoBlob::ofxTactoBlob(int _id, float _x, float _y, float _vx, float _vy, float _width, float _height, float _accel)
{
    id = _id;
    x = _x;
    y = _y;
    vx = _vx;
    vy = _vy;
    width = _width;
    height = _height;
    accel = _accel;

    // Init to random color
    color.a = 255;
    color.r = ofRandomuf() * 255;
    color.g = ofRandomuf() * 255;
    color.b = ofRandomuf() * 255;
}

void ofxTactoBlob::draw()
{
    ofSetColor(color.r, color.g, color.b, color.a);
    int blobX = x * ofGetWidth();
    int blobY = y * ofGetHeight();
    ofCircle(blobX, blobY, 20);
}
