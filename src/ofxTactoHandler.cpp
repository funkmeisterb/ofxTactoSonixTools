#include "ofxTactoHandler.h"

void ofxTactoHandler::setup()
{
    tuiorcvr.setup(TUIOPORT);
}

void ofxTactoHandler::update()
{
	while(tuiorcvr.hasWaitingMessages())
	{
		// get the next message
		ofxOscMessage m;
		tuiorcvr.getNextMessage(&m);

		// check for mouse moved message
		if (m.getAddress() == "/tuio/2Dcur")
		{
		    string msg = m.getArgAsString(0);
		    if (msg == "set")
		    {
				float blobID = m.getArgAsInt32(1);
				float posX = m.getArgAsFloat(2);
				float posZ = m.getArgAsFloat(3);
				float velX = m.getArgAsFloat(4);
				float velZ = m.getArgAsFloat(5);
				float height = 0;
				float width = 0;
				float accel = 0;
				if(m.getNumArgs() > 6)
					m.getArgAsFloat(6);
				if(m.getNumArgs() > 7)
					width = m.getArgAsFloat(7);
				if(m.getNumArgs() > 8)
					accel = m.getArgAsFloat(8);

				if (! IsBlobRegistered(blobID))
				{
				    // Add the current blob
				    ofxTactoBlob newBlob = ofxTactoBlob(blobID, posX, posZ, velX, velZ, height, width, accel);
				    AppendBlob(newBlob);
				    touchDown(posX, posZ, blobID);
				}
				else
				{
				    // Exists, so update its info
                    list<ofxTactoBlob>::iterator it;
                    for (it = listBlobs.begin(); it != listBlobs.end(); ++it)
                    {
                        if ((*it).id == blobID)
                        {
                            (*it).x = posX;
                            (*it).y = posZ;
                            (*it).vx = velX;
                            (*it).vy = velZ;
                            (*it).height = height;
                            (*it).width = width;
                            (*it).accel = accel;
                            touchMoved(posX, posZ, blobID);
                        }
                    }
				}
		    }
		    if (msg == "alive")
		    {
		        int numArgs = m.getNumArgs();
		        if (numArgs == 1)
		        {
					// For each blob in the list, send TouchUp message
                    list<ofxTactoBlob>::iterator it;
                    for (it = listBlobs.begin(); it != listBlobs.end(); ++it)
                    {
						touchUp((*it).x, (*it).y, (*it).id);
                    }
		            // Clear the vector, there are no blobs!
		            listBlobs.clear();
		        }
		        else
		        {
		            ManageBlobs(m);
		        }
		    }
		}
	}
}

void ofxTactoHandler::drawBlobs()
{
    list<ofxTactoBlob>::iterator it;
    for (it = listBlobs.begin(); it != listBlobs.end(); ++it)
    {
        (*it).draw();
    }
}

/** \return The list of blobs in the touch system.
*/
list<ofxTactoBlob> ofxTactoHandler::getBlobs()
{
    return listBlobs;
}

/** \return The number of blobs currently in the system.
*/
int ofxTactoHandler::numBlobs()
{
    return listBlobs.size();
}

/** \param m The TUIO message that is parsed.
*/
void ofxTactoHandler::ManageBlobs(ofxOscMessage m)
{
    int numArgs = m.getNumArgs();
    int numAliveBlobs = numArgs - 1;

    // Create vector of alive blobs
    vector<int> aliveBlobIDs;
    if (numAliveBlobs > 0) // only if there are blobs
    {
        for (int i=0; i<numAliveBlobs; i++)
        {
            int currAliveBlob = m.getArgAsInt32(i + 1);
            aliveBlobIDs.push_back(currAliveBlob);
        }

        // Traverse list of maintained blobs and remove those that are not alive
        list<ofxTactoBlob>::iterator it = listBlobs.begin();
        while (it != listBlobs.end())
        {
            if (IsBlobIDInVector((*it).id, aliveBlobIDs))
            {
                // Leave blob as is
                ++it;
            }
            else
            {
                // Remove blob
                touchUp((*it).x, (*it).y, (*it).id);
                listBlobs.erase(it++);
            }
        }
    }
}

/** \param blobID The ID of the blob.
* \return Whether or not the queried blob ID is in the current list of blobs.
*/
bool ofxTactoHandler::IsBlobRegistered(int blobID)
{
    bool breturn = false;
    list<ofxTactoBlob>::const_iterator it;
    for (it = listBlobs.begin(); it != listBlobs.end(); ++it)
    {
        if ((*it).id == blobID)
        {
            breturn = true;
            break;
        }
    }
    return breturn;
}

/** \param blobID The ID of the blob.
* \param vectorIDs A vector of blob IDs.
* \return Whether or not the queried blob ID is in the queried vector of blobs.
*/
bool ofxTactoHandler::IsBlobIDInVector(int blobID, vector<int> vectorIDs)
{
    bool breturn = false;
    vector<int>::const_iterator it;
    for (it = vectorIDs.begin(); it != vectorIDs.end(); ++it)
    {
        if (*it == blobID)
        {
            breturn = true;
            break;
        }
    }
    return breturn;
}

/** \param newblob The blob to append to the list of blobs in the system.
*/
void ofxTactoHandler::AppendBlob(ofxTactoBlob newblob)
{
    listBlobs.push_back(newblob);
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/void ofxTactoHandler::touchDown(float x, float y, int touchId)
{
	ofTouchEventArgs touchArgs;
	touchArgs.x = x;
	touchArgs.y = y;
	touchArgs.id = touchId;
	ofNotifyEvent(ofEvents.touchDown, touchArgs);
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void ofxTactoHandler::touchMoved(float x, float y, int touchId)
{
	ofTouchEventArgs touchArgs;
	touchArgs.x = x;
	touchArgs.y = y;
	touchArgs.id = touchId;
	ofNotifyEvent(ofEvents.touchMoved, touchArgs);
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void ofxTactoHandler::touchUp(float x, float y, int touchId)
{
	ofTouchEventArgs touchArgs;
	touchArgs.x = x;
	touchArgs.y = y;
	touchArgs.id = touchId;
	ofNotifyEvent(ofEvents.touchUp, touchArgs);
}

/**
* \param x The x coordinate of the touch event.
* \param y The y coordinate of the touch event.
* \param x The ID of the touch point.
*/
void ofxTactoHandler::touchDoubleTap(float x, float y, int touchId)
{
	ofTouchEventArgs touchArgs;
	touchArgs.x = x;
	touchArgs.y = y;
	touchArgs.id = touchId;
	ofNotifyEvent(ofEvents.touchDoubleTap, touchArgs);
}