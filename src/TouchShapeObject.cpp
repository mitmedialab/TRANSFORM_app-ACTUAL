//
//  TouchShapeObject.cpp
//  basicExample
//
//  Created by Ken Nakagaki on 6/16/15.
//
//

#include "TouchShapeObject.h"


TouchShapeObject::TouchShapeObject()
{
    //mKinectHeightImage.allocate(KINECT_X, KINECT_Y);
    mOutputShapeImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
    allPixels = new unsigned char[RELIEF_SIZE];
};

void TouchShapeObject::setup()
{
    
}

//----------------------------------------------------

void TouchShapeObject::update(float dt)
{
    unsigned char * pixels;
    //pixels = mKinectTracker->depthThreshedPixels();
    
    //mKinectHeightImage.setFromPixels(pixels, KINECT_X, KINECT_Y);
    //mImageWarper->warpIntoImage(mKinectHeightImage, mOutputShapeImage);
    
    //ofxCvGrayscaleImage smallerImage;
    smallerImage.allocate(RELIEF_PHYSICAL_SIZE_X, RELIEF_PHYSICAL_SIZE_Y);
    
    //RELIEF_SIZE_X RELIEF_SIZE_Y
    //int h = mPinHeightReceive[RELIEF_SIZE_X/2][RELIEF_SIZE_Y/2];
    cout << "in touch so pointer :"<< (int)mPinHeightReceive << endl;
    char *height = mPinHeightReceive[RELIEF_SIZE_X/2];
    if(height != NULL){
    int h = height[RELIEF_SIZE_Y/2];
        cout << "in touch so value :"<< h << endl;
        
    }
    
    //this->mImageWarper->warpIntoImage(mKinectHeightImage, smallerImage);
    
    for(int i = 0; i < RELIEF_PHYSICAL_SIZE_X * RELIEF_PHYSICAL_SIZE_Y; i++)
        allPixels[i] = 100;
    
    //allPixels = smallerImage.getPixels();
}

//----------------------------------------------------

void TouchShapeObject::renderShape()
{
    mOutputShapeImage.draw(0,0, RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
    //smallerImage.draw(0, 0, RELIEF_PHYSICAL_SIZE_X, RELIEF_PHYSICAL_SIZE_Y);
}

//----------------------------------------------------

void TouchShapeObject::renderGraphics(int x, int y, int w, int h)
{
    mOutputShapeImage.draw(0,0, RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
}

//----------------------------------------------------

void TouchShapeObject::drawGuiScreen(int x, int y, int w, int h)
{
    
}

//----------------------------------------------------

void TouchShapeObject::setTableValuesForShape(ShapeIOManager *pIOManager)
{
    pIOManager->set_max_speed(200);
    pIOManager->set_gain_p(1.5f);
    pIOManager->set_gain_i(0.045f);
    pIOManager->set_max_i(25);
    pIOManager->set_deadzone(2);
};

//----------------------------------------------------

unsigned char* TouchShapeObject::getPixels()
{
    return allPixels;
}