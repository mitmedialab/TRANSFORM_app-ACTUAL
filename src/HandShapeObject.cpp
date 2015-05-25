//
//  HandShapeObject.cpp
//  basicExample
//
//  Created by Philipp Schoessler on 7/9/14.
//
//

#include "HandShapeObject.h"


HandShapeObject::HandShapeObject()
{
    mKinectHeightImage.allocate(KINECT_X, KINECT_Y);
    mOutputShapeImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
    smallerImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
    allPixels = new unsigned char[RELIEF_SIZE];
    isMirror = false;
};

void HandShapeObject::setup()
{
    
}

//----------------------------------------------------

void HandShapeObject::update(float dt)
{
    unsigned char * pixels;
    pixels = mKinectTracker->depthThreshedPixels();
    
    mKinectHeightImage.setFromPixels(pixels, KINECT_X, KINECT_Y);
    mImageWarper->warpIntoImage(mKinectHeightImage, mOutputShapeImage);
    
    if(isMirror)
    {
        mOutputShapeImage.mirror(true, true);
    }
    
    smallerImage.setFromPixels(mOutputShapeImage.getPixels(),RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
    smallerImage.resize(RELIEF_PHYSICAL_SIZE_X, RELIEF_PHYSICAL_SIZE_Y);
    
    allPixels = smallerImage.getPixels();
    
    //make all grey pixels white so the hand is displayed with maximum pin height
    for(int i = 0; i < RELIEF_SIZE; i ++)
    {
        if(allPixels[i] > 0)
            allPixels[i] = 255;
    }
}

//----------------------------------------------------

void HandShapeObject::renderShape()
{
    mOutputShapeImage.draw(0,0, RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
}

//----------------------------------------------------

void HandShapeObject::renderGraphics(int x, int y, int w, int h)
{
    mOutputShapeImage.draw(0,0, RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_Y);
}

//----------------------------------------------------

void HandShapeObject::drawGuiScreen(int x, int y, int w, int h)
{
    
}

//----------------------------------------------------

void HandShapeObject::setTableValuesForShape(ShapeIOManager *pIOManager)
{
    pIOManager->set_max_speed(200);
    pIOManager->set_gain_p(1.5f);
    pIOManager->set_gain_i(0.045f);
    pIOManager->set_max_i(25);
    pIOManager->set_deadzone(2);
};

//----------------------------------------------------

void HandShapeObject::setMirror(bool _val)
{
    isMirror = _val;
}

//----------------------------------------------------


unsigned char* HandShapeObject::getPixels()
{
    return allPixels;
}