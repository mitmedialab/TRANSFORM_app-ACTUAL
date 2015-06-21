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
    
}

void TouchShapeObject::setup()
{
    
}

//----------------------------------------------------

void TouchShapeObject::update(float dt)
{
    unsigned char * pixels;

    smallerImage.allocate(RELIEF_PHYSICAL_SIZE_X, RELIEF_PHYSICAL_SIZE_Y);
    
    int lineSize = sizeof(char) * RELIEF_SIZE_Y;
    int x = RELIEF_SIZE_X / 2;
    int y = RELIEF_SIZE_Y / 2;
    unsigned char h = mPinHeightReceive[x * lineSize + y];
    
    for (int i = 0; i < RELIEF_PHYSICAL_SIZE_X; i++) {
        for(int j = 0; j < RELIEF_PHYSICAL_SIZE_Y; j++){
            int d = ofDist(RELIEF_PHYSICAL_SIZE_X/2, RELIEF_PHYSICAL_SIZE_Y/2, i, j);
            if(d>15){ d = 15; };
                int dHeight = ofMap(d, 0, 15, (int)h, 230);
            allPixels[RELIEF_PHYSICAL_SIZE_X*j+i] =  dHeight;
            
        }
    }
    
    
    allPixels[RELIEF_PHYSICAL_SIZE_X*RELIEF_PHYSICAL_SIZE_Y/2+RELIEF_PHYSICAL_SIZE_X/2]=230;
    
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