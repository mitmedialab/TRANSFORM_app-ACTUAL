//
//  TouchShapeObject.h
//  basicExample
//
//  Created by Ken Nakagaki on 6/16/15.
//
//

#ifndef __basicExample__TouchShapeObject__
#define __basicExample__TouchShapeObject__

#include <iostream>
#include "ShapeObject.h"
#include "Constants.h"
#include "ShapeIOManager.h"
#include "ofxOpenCv.h"

class TouchShapeObject : public ShapeObject {
    
#define NUM_FILTER_FRAME 5
#define NUM_WAVE_FRAME 50
    
public:
    TouchShapeObject();
    void setup();
    void update(float dt);
    void renderShape();
    void renderGraphics(int x, int y, int w, int h);
    void drawGuiScreen(int x, int y, int w, int h);
    void setPinHeight(unsigned char pinHeightReceive[RELIEF_SIZE_X][RELIEF_SIZE_Y]){
        mPinHeightReceive = (unsigned char *) pinHeightReceive;
    };
    //void setImageWarper(ImageWarper * pImageWarper) {mImageWarper = pImageWarper;};
    void setTableValuesForShape(ShapeIOManager *pIOManager);
    void singleElasticSurface();
    void triSurface();
    void waveSurface();
    
    void modeChange();
    int touchMode = 0;
    int touchModeNum = 3;
    
    unsigned char* getPixels();
    
    int xCoordinateShift(int num);
    
    
    
    string get_shape_name() {return shape_name; };
    string shape_name = "Touch";
    
    
private:
    unsigned char *mPinHeightReceive;
   // ImageWarper * mImageWarper;
    ofxCvGrayscaleImage mOutputShapeImage, smallerImage;
    
    unsigned char* allPixels;
    int differenceHeight[RELIEF_SIZE_X][RELIEF_SIZE_Y];
    int filterFrame = NUM_FILTER_FRAME;
    int lineSize;
    
    int defaultHeight = LOW_THRESHOLD;
    
    unsigned char allPixels_store[RELIEF_SIZE][NUM_FILTER_FRAME];
    
    unsigned char mPinHeightReceive_store[RELIEF_SIZE_X][RELIEF_SIZE_Y][NUM_WAVE_FRAME]; //Store touched record for Wave Interaction
    
    Boolean isTouched[RELIEF_SIZE_X][RELIEF_SIZE_Y];

    
};


#endif /* defined(__basicExample__TouchShapeObject__) */
