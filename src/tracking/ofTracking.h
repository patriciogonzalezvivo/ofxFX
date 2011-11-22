/*
 *  ofTracking.h
 *
 *  Created by Ramsin Khoshabeh on 5/4/08.
 *  Copyright 2008 risenparadigm. All rights reserved.
 *
 * Changelog:
 * 08/15/08 -- Fixed a major bug in the track algorithm
 */

#ifndef OFTRACKING
#define OFTRACKING

#include <list>
#include <map>

#include "ofContourFinder.h"

class ofBlobTracker {
public:
	ofBlobTracker();
	~ofBlobTracker();
	
	//assigns IDs to each blob in the contourFinder
	void track(ofContourFinder* newBlobs);

	int MOVEMENT_FILTERING;
	std::vector<ofBlob> getTrackedBlobs();
	std::vector<ofBlob> getTrackedFingers();

private:
	int trackKnn(ofContourFinder *newBlobs, ofBlob *track, int k, double thresh, bool fingers);
	int	IDCounter;	  //counter of last blob
	//int	fightMongrel;
	int numEnter,numLeave;
	int size;
	
	std::vector<ofBlob>		trackedBlobs; //tracked blobs
	std::vector<ofBlob>		trackedFingers; //tracked Fingers
};

#endif
