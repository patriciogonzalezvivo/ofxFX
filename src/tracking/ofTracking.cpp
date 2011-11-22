/*
*  Tracking.cpp
*
*  Created by Ramsin Khoshabeh on 5/4/08.
*  Copyright 2008 risenparadigm. All rights reserved.
*
*  Changelog:
*  08/15/08 -- Fixed a major bug in the track algorithm
*
*
*/

#include "ofTracking.h"

ofBlobTracker::ofBlobTracker(){
	IDCounter = 0; //200;
	numEnter = 0;
	numLeave = 0;
	size = 0; 
}

ofBlobTracker::~ofBlobTracker(){
}

//assigns IDs to each blob in the contourFinder
void ofBlobTracker::track(ofContourFinder* newBlobs){	
	numEnter = 0;
	numLeave = 0;
	//printf("%d\n",trackedBlobs.size());
	
/****************************************************************************
	 //Blob tracking
****************************************************************************/
	//initialize ID's of all blobs
	for(int i=0; i<newBlobs->nBlobs; i++)
			newBlobs->blobs[i].id=-1;
	
	size = trackedBlobs.size();

	// STEP 1: Blob matching 
	//
	//go through all tracked blobs to compute nearest new point
	for(int i = 0; i < trackedBlobs.size(); i++) {
		/******************************************************************
		 * *****************TRACKING FUNCTION TO BE USED*******************
		 * Replace 'trackKnn(...)' with any function that will take the
		 * current track and find the corresponding track in the newBlobs
		 * 'winner' should contain the index of the found blob or '-1' if
		 * there was no corresponding blob
		 *****************************************************************/
		int winner = trackKnn(newBlobs, &(trackedBlobs[i]), 3, 0, false);

		if(winner == -1) { //track has died, mark it for deletion
			trackedBlobs[i].id = -1;
		} else { //still alive, have to update
			//if winning new blob was labeled winner by another track\
			//then compare with this track to see which is closer
			if(newBlobs->blobs[winner].id!=-1){
				//find the currently assigned blob
				int j; //j will be the index of it
				for(j=0; j<trackedBlobs.size(); j++){
					if(trackedBlobs[j].id==newBlobs->blobs[winner].id)
						break;
				}

				if(j==trackedBlobs.size()){//got to end without finding it
					newBlobs->blobs[winner].id = trackedBlobs[i].id;
					newBlobs->blobs[winner].age = trackedBlobs[i].age;
					newBlobs->blobs[winner].sitting = trackedBlobs[i].sitting;
					newBlobs->blobs[winner].downTime = trackedBlobs[i].downTime;
					newBlobs->blobs[winner].color = trackedBlobs[i].color;
					newBlobs->blobs[winner].lastTimeTimeWasChecked = trackedBlobs[i].lastTimeTimeWasChecked;
					trackedBlobs[i] = newBlobs->blobs[winner];
				} else { //found it, compare with current blob
					double x = newBlobs->blobs[winner].centroid.x;
					double y = newBlobs->blobs[winner].centroid.y;
					double xOld = trackedBlobs[j].centroid.x;
					double yOld = trackedBlobs[j].centroid.y;
					double xNew = trackedBlobs[i].centroid.x;
					double yNew = trackedBlobs[i].centroid.y;
					double distOld = (x-xOld)*(x-xOld)+(y-yOld)*(y-yOld);
					double distNew = (x-xNew)*(x-xNew)+(y-yNew)*(y-yNew);

					//if this track is closer, update the ID of the blob
					//otherwise delete this track.. it's dead
					if( distNew < distOld){ //update
						newBlobs->blobs[winner].id = trackedBlobs[i].id;
						newBlobs->blobs[winner].age = trackedBlobs[i].age;
						newBlobs->blobs[winner].sitting = trackedBlobs[i].sitting;
						newBlobs->blobs[winner].downTime = trackedBlobs[i].downTime;
						newBlobs->blobs[winner].color = trackedBlobs[i].color;
						newBlobs->blobs[winner].lastTimeTimeWasChecked = trackedBlobs[i].lastTimeTimeWasChecked;
						//TODO-----------------------------------------------
						//now the old winning blob has lost the win.
						//I should also probably go through all the newBlobs
						//at the end of this loop and if there are ones without
						//any winning matches, check if they are close to this
						//one. Right now I'm not doing that to prevent a
						//recursive mess. It'll just be a new track.

						//erase calibrated blob from map
						//calibratedBlobs.erase(trackedBlobs[j].id);
						//mark the blob for deletion
						trackedBlobs[j].id = -1;
						//-----------------------------------------------------
					} else {	//delete
						//erase calibrated blob from map
						//calibratedBlobs.erase(trackedBlobs[i].id);
						//mark the blob for deletion
						trackedBlobs[i].id = -1;
					}
				}
			} else {//no conflicts, so simply update
				newBlobs->blobs[winner].id = trackedBlobs[i].id;
				newBlobs->blobs[winner].age = trackedBlobs[i].age;
				newBlobs->blobs[winner].sitting = trackedBlobs[i].sitting;
				newBlobs->blobs[winner].downTime = trackedBlobs[i].downTime;
				newBlobs->blobs[winner].color = trackedBlobs[i].color;
				newBlobs->blobs[winner].lastTimeTimeWasChecked = trackedBlobs[i].lastTimeTimeWasChecked;
			}
		}
	}

	// AlexP
	// save the current time since we will be using it a lot
	int now = ofGetElapsedTimeMillis();

	// STEP 2: Blob update
	//
	//--Update All Current Tracks
	//remove every track labeled as dead (ID='-1')
	//find every track that's alive and copy it's data from newBlobs
	for(int i = 0; i < trackedBlobs.size(); i++) {
		if(trackedBlobs[i].id == -1) { //dead
			numLeave++;
			//erase track
			trackedBlobs.erase(trackedBlobs.begin()+i, trackedBlobs.begin()+i+1);
			i--; //decrement one since we removed an element
		} else {//living, so update it's data
			for(int j = 0; j < newBlobs->nBlobs; j++) {
				if(trackedBlobs[i].id == newBlobs->blobs[j].id) {
					//update track
					ofPoint tempLastCentroid = trackedBlobs[i].centroid; // assign the new centroid to the old
					trackedBlobs[i] = newBlobs->blobs[j];
					trackedBlobs[i].lastCentroid = tempLastCentroid;

					ofPoint tD;
					//get the Differences in position
					tD.set(trackedBlobs[i].centroid.x - trackedBlobs[i].lastCentroid.x, 
							trackedBlobs[i].centroid.y - trackedBlobs[i].lastCentroid.y);
					//calculate the acceleration
					float posDelta = sqrtf((tD.x*tD.x)+(tD.y*tD.y));

					// AlexP
					// now, filter the blob position based on MOVEMENT_FILTERING value
					// the MOVEMENT_FILTERING ranges [0,15] so we will have that many filtering steps
					// Here we have a weighted low-pass filter
					// adaptively adjust the blob position filtering strength based on blob movement
					// http://www.wolframalpha.com/input/?i=plot+1/exp(x/15)+and+1/exp(x/10)+and+1/exp(x/5)+from+0+to+100
					float a = 1.0f - 1.0f / expf(posDelta / (1.0f + (float)MOVEMENT_FILTERING*10));
					trackedBlobs[i].centroid.x = a * trackedBlobs[i].centroid.x + (1-a) * trackedBlobs[i].lastCentroid.x;
					trackedBlobs[i].centroid.y = a * trackedBlobs[i].centroid.y + (1-a) * trackedBlobs[i].lastCentroid.y;

					//get the Differences in position
					trackedBlobs[i].D.set(trackedBlobs[i].centroid.x - trackedBlobs[i].lastCentroid.x, 
											trackedBlobs[i].centroid.y - trackedBlobs[i].lastCentroid.y);

					//if( abs((int)trackedBlobs[i].D.x) > 1 || abs((int)trackedBlobs[i].D.y) > 1) {
//						printf("\nUNUSUAL BLOB @ %f\n-----------------------\ntrackedBlobs[%i]\nD = (%f, %f)\nXY= (%f, %f)\nlastTimeTimeWasChecked = %f\nsitting = %f\n",
//							   ofGetElapsedTimeMillis(),
//							   i,
//							   trackedBlobs[i].D.x,  trackedBlobs[i].D.y,
//							   trackedBlobs[i].centroid.x, trackedBlobs[i].centroid.y,
//							   trackedBlobs[i].lastTimeTimeWasChecked,
//							   trackedBlobs[i].downTime,
//							   trackedBlobs[i].sitting
//						);
//					}

					//calculate the acceleration again
					tD = trackedBlobs[i].D;
					trackedBlobs[i].maccel = sqrtf((tD.x* tD.x)+(tD.y*tD.y)) / (now - trackedBlobs[i].lastTimeTimeWasChecked);

					//calculate the age
					trackedBlobs[i].age = ofGetElapsedTimef() - trackedBlobs[i].downTime;

					//set sitting (held length)
                    if(trackedBlobs[i].maccel < 7) {	//1 more frame of sitting
						if(trackedBlobs[i].sitting != -1)
							trackedBlobs[i].sitting = ofGetElapsedTimef() - trackedBlobs[i].downTime;           
					} else
						trackedBlobs[i].sitting = -1;

					//printf("time: %f\n", ofGetElapsedTimeMillis());
					//printf("%i age: %f, downTimed at: %f\n", i, trackedBlobs[i].age, trackedBlobs[i].downTime);

					//if blob has been 'holding/sitting' for 1 second send a held event
					if(trackedBlobs[i].sitting > 1.0f){
                        //held event only happens once so set to -1
                        trackedBlobs[i].sitting = -1;
					}
					
					// AlexP
					// The last lastTimeTimeWasChecked is updated at the end after all acceleration values are calculated
					trackedBlobs[i].lastTimeTimeWasChecked = now;
				}
			}
		}
	}

	// STEP 3: add tracked blobs to TouchEvents
	//--Add New Living Tracks
	//now every new blob should be either labeled with a tracked ID or\
	//have ID of -1... if the ID is -1... we need to make a new track
	for(int i = 0; i < newBlobs->nBlobs; i++){
		if(newBlobs->blobs[i].id==-1){
			//add new track
			newBlobs->blobs[i].id = IDCounter++;
			IDCounter++;
			newBlobs->blobs[i].downTime = ofGetElapsedTimef();
			//newBlobs->blobs[i].lastTimeTimeWasChecked = ofGetElapsedTimeMillis();

			//random color for blob. Could be useful?
			int r = ofRandom(0, 255);
            int g = ofRandom(0, 255);
            int b = ofRandom(0, 255);
            //Convert to hex
            int rgbNum = ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
            //Set color
            newBlobs->blobs[i].color = rgbNum;
			trackedBlobs.push_back(newBlobs->blobs[i]);
			numEnter++;
			if (numEnter > 20){
				printf("something wrong!\n");
			}
		}
	}
	
/****************************************************************************
	 //Finger tracking
****************************************************************************/
	//initialize ID's of all blobs
	for(int i=0; i<newBlobs->nFingers; i++)
		newBlobs->fingers[i].id=-1;
	
	// STEP 1: Finger matching
	//
	//go through all tracked blobs to compute nearest new point
	for(int i = 0; i < trackedFingers.size(); i++) {
		/******************************************************************
		 * *****************TRACKING FUNCTION TO BE USED*******************
		 * Replace 'trackKnn(...)' with any function that will take the
		 * current track and find the corresponding track in the newBlobs
		 * 'winner' should contain the index of the found blob or '-1' if
		 * there was no corresponding blob
		 *****************************************************************/
		int winner = trackKnn(newBlobs, &(trackedFingers[i]), 3, 0, true);
		
		if(winner == -1) { //track has died, mark it for deletion
			trackedFingers[i].id = -1;
		} else { //still alive, have to update
			//if winning new blob was labeled winner by another track\
			//then compare with this track to see which is closer
			if(newBlobs->fingers[winner].id!=-1) {
				//find the currently assigned finger
				int j; //j will be the index of it
				for(j=0; j<trackedFingers.size(); j++){
					if(trackedFingers[j].id==newBlobs->fingers[winner].id)
						break;
				}
				
				if(j==trackedFingers.size()){ //got to end without finding it
					newBlobs->fingers[winner].id = trackedFingers[i].id;
					newBlobs->fingers[winner].age = trackedFingers[i].age;
					newBlobs->fingers[winner].sitting = trackedFingers[i].sitting;
					newBlobs->fingers[winner].downTime = trackedFingers[i].downTime;
					newBlobs->fingers[winner].color = trackedFingers[i].color;
					newBlobs->fingers[winner].lastTimeTimeWasChecked = trackedFingers[i].lastTimeTimeWasChecked;
					
					trackedFingers[i] = newBlobs->fingers[winner];
				} else {//found it, compare with current blob
					double x = newBlobs->fingers[winner].centroid.x;
					double y = newBlobs->fingers[winner].centroid.y;
					double xOld = trackedFingers[j].centroid.x;
					double yOld = trackedFingers[j].centroid.y;
					double xNew = trackedFingers[i].centroid.x;
					double yNew = trackedFingers[i].centroid.y;
					double distOld = (x-xOld)*(x-xOld)+(y-yOld)*(y-yOld);
					double distNew = (x-xNew)*(x-xNew)+(y-yNew)*(y-yNew);
					
					//if this track is closer, update the ID of the blob
					//otherwise delete this track.. it's dead
					if(distNew<distOld) { //update
						newBlobs->fingers[winner].id = trackedFingers[i].id;
						newBlobs->fingers[winner].age = trackedFingers[i].age;
						newBlobs->fingers[winner].sitting = trackedFingers[i].sitting;
						newBlobs->fingers[winner].downTime = trackedFingers[i].downTime;
						newBlobs->fingers[winner].color = trackedFingers[i].color;
						newBlobs->fingers[winner].lastTimeTimeWasChecked = trackedFingers[i].lastTimeTimeWasChecked;
						
						//TODO--------------------------------------------------------------------------
						//now the old winning blob has lost the win.
						//I should also probably go through all the newBlobs
						//at the end of this loop and if there are ones without
						//any winning matches, check if they are close to this
						//one. Right now I'm not doing that to prevent a
						//recursive mess. It'll just be a new track.
						
						//erase calibrated blob from map
						//calibratedBlobs.erase(trackedBlobs[j].id);
						//mark the blob for deletion
						trackedFingers[j].id = -1;
						//------------------------------------------------------------------------------
					} else { //delete
						//erase calibrated blob from map
						//calibratedBlobs.erase(trackedBlobs[i].id);
						//mark the blob for deletion
						trackedFingers[i].id = -1;
					}
				}
			} else { //no conflicts, so simply update
				newBlobs->fingers[winner].id = trackedFingers[i].id;
				newBlobs->fingers[winner].age = trackedFingers[i].age;
				newBlobs->fingers[winner].sitting = trackedFingers[i].sitting;
				newBlobs->fingers[winner].downTime = trackedFingers[i].downTime;
				newBlobs->fingers[winner].color = trackedFingers[i].color;
				newBlobs->fingers[winner].lastTimeTimeWasChecked = trackedFingers[i].lastTimeTimeWasChecked;
			}
		}
	}
	
	// AlexP
	// save the current time since we will be using it a lot
	now = ofGetElapsedTimeMillis();
	
	// STEP 2: Blob update
	//
	//--Update All Current Tracks
	//remove every track labeled as dead (ID='-1')
	//find every track that's alive and copy it's data from newBlobs
	for(int i = 0; i < trackedFingers.size(); i++){
		if(trackedFingers[i].id == -1) { //dead
			trackedFingers.erase(trackedFingers.begin()+i, trackedFingers.begin()+i+1); //erase track
			i--; //decrement one since we removed an element
		} else { //living, so update it's data
			for(int j = 0; j < newBlobs->nFingers; j++){
				if(trackedFingers[i].id == newBlobs->fingers[j].id) {
					//update track
					ofPoint tempLastCentroid = trackedFingers[i].centroid; // assign the new centroid to the old
					trackedFingers[i] = newBlobs->fingers[j];
					trackedFingers[i].lastCentroid = tempLastCentroid;
					
					ofPoint tD;
					//get the Differences in position
					tD.set(trackedFingers[i].centroid.x - trackedFingers[i].lastCentroid.x, 
						   trackedFingers[i].centroid.y - trackedFingers[i].lastCentroid.y);
					//calculate the acceleration
					float posDelta = sqrtf((tD.x*tD.x)+(tD.y*tD.y));
					
					// AlexP
					// now, filter the blob position based on MOVEMENT_FILTERING value
					// the MOVEMENT_FILTERING ranges [0,15] so we will have that many filtering steps
					// Here we have a weighted low-pass filter
					// adaptively adjust the blob position filtering strength based on blob movement
					// http://www.wolframalpha.com/input/?i=plot+1/exp(x/15)+and+1/exp(x/10)+and+1/exp(x/5)+from+0+to+100
					float a = 1.0f - 1.0f / expf(posDelta / (1.0f + (float)MOVEMENT_FILTERING*10));
					trackedFingers[i].centroid.x = a * trackedFingers[i].centroid.x + (1-a) * trackedFingers[i].lastCentroid.x;
					trackedFingers[i].centroid.y = a * trackedFingers[i].centroid.y + (1-a) * trackedFingers[i].lastCentroid.y;
					
					//get the Differences in position
					trackedFingers[i].D.set(	trackedFingers[i].centroid.x - trackedFingers[i].lastCentroid.x, 
												trackedFingers[i].centroid.y - trackedFingers[i].lastCentroid.y);
					
					//if( abs((int)trackedBlobs[i].D.x) > 1 || abs((int)trackedBlobs[i].D.y) > 1) {
					//						printf("\nUNUSUAL BLOB @ %f\n-----------------------\ntrackedBlobs[%i]\nD = (%f, %f)\nXY= (%f, %f)\nlastTimeTimeWasChecked = %f\nsitting = %f\n",
					//							   ofGetElapsedTimeMillis(),
					//							   i,
					//							   trackedBlobs[i].D.x,  trackedBlobs[i].D.y,
					//							   trackedBlobs[i].centroid.x, trackedBlobs[i].centroid.y,
					//							   trackedBlobs[i].lastTimeTimeWasChecked,
					//							   trackedBlobs[i].downTime,
					//							   trackedBlobs[i].sitting
					//						);
					//					}
					
					//calculate the acceleration again
					tD = trackedFingers[i].D;
					trackedFingers[i].maccel = sqrtf((tD.x* tD.x)+(tD.y*tD.y)) / (now - trackedFingers[i].lastTimeTimeWasChecked);
					
					//calculate the age
					trackedFingers[i].age = ofGetElapsedTimef() - trackedFingers[i].downTime;
					
					//set sitting (held length)
                    if(trackedFingers[i].maccel < 7) {	//1 more frame of sitting
						if(trackedFingers[i].sitting != -1)
							trackedFingers[i].sitting = ofGetElapsedTimef() - trackedFingers[i].downTime;           
					} else
						trackedFingers[i].sitting = -1;
					
					//printf("time: %f\n", ofGetElapsedTimeMillis());
					//printf("%i age: %f, downTimed at: %f\n", i, trackedBlobs[i].age, trackedBlobs[i].downTime);
					
					//if blob has been 'holding/sitting' for 1 second send a held event
					if(trackedFingers[i].sitting > 1.0f){
                        //held event only happens once so set to -1
                        trackedFingers[i].sitting = -1;
						
						//TouchEvents.notifyTouchHeld(NULL);
					} 
					// AlexP
					// The last lastTimeTimeWasChecked is updated at the end after all acceleration values are calculated
					trackedFingers[i].lastTimeTimeWasChecked = now;
				}
			}
		}
	}
	
	// STEP 3: add tracked blobs to TouchEvents
	//--Add New Living Tracks
	//now every new blob should be either labeled with a tracked ID or\
	//have ID of -1... if the ID is -1... we need to make a new track
	for(int i = 0; i < newBlobs->nFingers; i++) {
		if(newBlobs->fingers[i].id==-1){
			//add new track
			newBlobs->fingers[i].id = IDCounter++;
			newBlobs->fingers[i].downTime = ofGetElapsedTimef();
			//newBlobs->fingers[i].lastTimeTimeWasChecked = ofGetElapsedTimeMillis();
			
			//random color for blob. Could be useful?
			int r = ofRandom(0, 255);
            int g = ofRandom(0, 255);
            int b = ofRandom(0, 255);
            //Convert to hex
            int rgbNum = ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
            //Set color
            newBlobs->fingers[i].color = rgbNum;
			trackedFingers.push_back(newBlobs->fingers[i]);
		}
	}
	
	
}

std::vector<ofBlob> ofBlobTracker::getTrackedBlobs(){
    return trackedBlobs;
}

std::vector<ofBlob> ofBlobTracker::getTrackedFingers(){
	return trackedFingers;
}


/*************************************************************************
* Finds the blob in 'newBlobs' that is closest to the trackedBlob with index
* 'ind' according to the KNN algorithm and returns the index of the winner
* newBlobs	= list of blobs detected in the latest frame
* track		= current tracked blob being tested
* k			= number of nearest neighbors to consider\
*			  1,3,or 5 are common numbers..\
*			  must always be an odd number to avoid tying
* thresh	= threshold for optimization
**************************************************************************/
//int ofBlobTracker::trackKnn(ContourFinder *newBlobs, Blob *track, int k, double thresh = 0){
int ofBlobTracker::trackKnn(ofContourFinder *newBlobs, ofBlob *track, int k, double thresh , bool fing){
	int winner = -1; //initially label track as '-1'=dead
	if((k%2)==0) k++; //if k is not an odd number, add 1 to it

	//if it exists, square the threshold to use as square distance
	if(thresh>0)
		thresh *= thresh;

	//list of neighbor point index and respective distances
	std::list<std::pair<int,double> > nbors;
	std::list<std::pair<int,double> >::iterator iter;

	//find 'k' closest neighbors of testpoint
	double x, y, xT, yT, dist;
	
	if (fing){	// SEARCHING FOR FINGERS
		for(int i=0; i < newBlobs->nFingers; i++){
			x = newBlobs->fingers[i].centroid.x;
			y = newBlobs->fingers[i].centroid.y;

			xT = track->centroid.x;
			yT = track->centroid.y;
			dist = (x-xT)*(x-xT)+(y-yT)*(y-yT);

			if(dist<=thresh) { //it's good, apply label if no label yet and return
				winner = i;
				return winner;
			}

			/****************************************************************
			 * check if this blob is closer to the point than what we've seen
			 *so far and add it to the index/distance list if positive
			 ****************************************************************/

			//search the list for the first point with a longer distance
			for(iter=nbors.begin(); iter!=nbors.end()
				&& dist>=iter->second; iter++);

			if((iter!=nbors.end())||(nbors.size()<k)) { //it's valid, insert it
				nbors.insert(iter, 1, std::pair<int, double>(i, dist));
				//too many items in list, get rid of farthest neighbor
				if(nbors.size()>k)
					nbors.pop_back();
			}
		}
	} else {	// SEARCHING FOR BLOBS
		for(int i=0; i < newBlobs->nBlobs; i++){
			x = newBlobs->blobs[i].centroid.x;
			y = newBlobs->blobs[i].centroid.y;
			
			xT = track->centroid.x;
			yT = track->centroid.y;
			dist = (x-xT)*(x-xT)+(y-yT)*(y-yT);
			
			if(dist<=thresh) { //it's good, apply label if no label yet and return
				winner = i;
				return winner;
			}
			
			/****************************************************************
			 * check if this blob is closer to the point than what we've seen
			 *so far and add it to the index/distance list if positive
			 ****************************************************************/
			
			//search the list for the first point with a longer distance
			for(iter=nbors.begin(); iter!=nbors.end()
				&& dist>=iter->second; iter++);
			
			if((iter!=nbors.end())||(nbors.size()<k)) { //it's valid, insert it
				nbors.insert(iter, 1, std::pair<int, double>(i, dist));
				//too many items in list, get rid of farthest neighbor
				if(nbors.size()>k)
					nbors.pop_back();
			}
		}
	}

	/********************************************************************
	* we now have k nearest neighbors who cast a vote, and the majority
	* wins. we use each class average distance to the target to break any
	* possible ties.
	*********************************************************************/

	// a mapping from labels (IDs) to count/distance
	std::map<int, std::pair<int, double> > votes;

	//remember:
	//iter->first = index of newBlob
	//iter->second = distance of newBlob to current tracked blob
	for(iter=nbors.begin(); iter!=nbors.end(); iter++){
		//add up how many counts each neighbor got
		int count = ++(votes[iter->first].first);
		double dist = (votes[iter->first].second+=iter->second);

		/* check for a possible tie and break with distance */
		if(count>votes[winner].first || count==votes[winner].first
			&& dist<votes[winner].second)
		{
			winner = iter->first;
		}
	}
	return winner;
}
