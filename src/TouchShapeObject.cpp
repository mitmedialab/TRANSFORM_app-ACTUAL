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
    
    
    for(int i = 0; i< RELIEF_SIZE; i++){
        for(int j = 0; j< filterFrame; j++){
            allPixels_store[i][j] = 210;
        }
    }
    
    
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            differenceHeight[i][j] = 0;
        }
    }
    lineSize = sizeof(char) * RELIEF_SIZE_Y;
    
    
    for (int i = 0; i < RELIEF_SIZE_X; i++) {
        for(int j = 0; j < RELIEF_SIZE_Y; j++){
            for (int k = 0; k < NUM_WAVE_FRAME ; k++) {
                mPinHeightReceive_store[i][j][k] = defaultHeight;
            }
        }
    }
}

void TouchShapeObject::setup()
{
    
}

//----------------------------------------------------

void TouchShapeObject::update(float dt)
{
    unsigned char * pixels;
    
    
    
    //store data
    for(int i = 0; i < RELIEF_SIZE; i++){
        for(int j = filterFrame-1; j > 0; j--){
            allPixels_store[i][j] = allPixels_store[i][j-1];
        }
        allPixels_store[i][0] = allPixels[i];
    }
    
    //check each pin's difference over time - if it's flat or not
    Boolean flat[RELIEF_SIZE_X][RELIEF_SIZE_Y];
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        int XShift =  xCoordinateShift(i);

        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            
            int maxVal=0, minVal=255;
            for(int k =0; k < filterFrame; k++){
                maxVal = MAX(maxVal,(int)allPixels_store[RELIEF_PHYSICAL_SIZE_X*j+XShift][k]);
                minVal = MIN(minVal,(int)allPixels_store[RELIEF_PHYSICAL_SIZE_X*j+XShift][k]);
            }
            if (abs(maxVal - minVal) > 2) {
                flat[i][j] = false;
            } else {
                flat[i][j] = true;
            }
            
        }
    }

    
    
    // caliculate difference between send and receive value
    
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            int XShift =  xCoordinateShift(i);;

            int output = int(allPixels[RELIEF_PHYSICAL_SIZE_X*j+XShift]);
            int input = int(mPinHeightReceive[i * lineSize + j]);
            
            differenceHeight[i][j] =  output - input;
            
        }
    }
    
    // determine if each pin were touched or not
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            if(flat[i][j]){
                if (abs(differenceHeight[i][j]) > 21) {
                    isTouched[i][j] = true;
                } else {
                    isTouched[i][j] = false;
                }
            } else {
                isTouched[i][j] = false;
            }
        }
    }
    
    
    
    
    
    if (touchMode ==0) {
        waveSurface();
    } else if (touchMode ==1){
        singleElasticSurface();
    } else if (touchMode==2){
        
        triSurface();
    }
    
    
    
    //*** MODE: One Center Pin Input ***//
//    int x = RELIEF_SIZE_X / 2;
//    int y = RELIEF_SIZE_Y / 2;
//    unsigned char h = MAX(LOW_THRESHOLD,mPinHeightReceive[x * lineSize + y]);
//    
//    int XShift = xCoordinateShift(x);
//    
//    for (int i = 0; i < RELIEF_PHYSICAL_SIZE_X; i++) {
//        for(int j = 0; j < RELIEF_PHYSICAL_SIZE_Y; j++){
//            
//            int d = ofDist(XShift, y, i, j);
//            if(d>15){ d = 15; };
//            int dHeight = ofMap(d, 0, 15, (int)h, HIGH_THRESHOLD);
//            dHeight = MAX(LOW_THRESHOLD, dHeight);
//            allPixels[RELIEF_PHYSICAL_SIZE_X* j+ i] =  dHeight;
//        }
//    }
//    
//    allPixels[RELIEF_PHYSICAL_SIZE_X*y+XShift]=HIGH_THRESHOLD;
    
    
    
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
    int pixelSize = 5;
    
    ofPushMatrix();
    ofTranslate(630, 0);
    ofFill();
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        if(i==16 || i == 32){
            ofTranslate(5, 0);
        }
        
        int XShift = xCoordinateShift(i);
        
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            int val = differenceHeight[i][j];
            if(val< 0){
                ofSetColor(ofMap(val, 0, -160, 0, 255),0,0);
            } else {
                ofSetColor(0,ofMap(val, 0, 160, 0, 255),0);
            }
            
            ofRect(i*pixelSize,j*pixelSize,pixelSize,pixelSize);
            if (isTouched[i][j]) {
                ofNoFill();
                ofSetColor(255, 0, 0);
                ofRect(i*pixelSize,j*pixelSize,pixelSize,pixelSize);
                ofFill();
            }
            
            
            int output = int(allPixels[RELIEF_PHYSICAL_SIZE_X*j+XShift]);
            
            ofSetColor(0,0,ofMap(output, 0, 160, 0, 255));
            ofRect(i*pixelSize +270,j*pixelSize,pixelSize,pixelSize);
        }
    }
    
    ofTranslate(0, 130);
    for (int i = 0; i < RELIEF_PHYSICAL_SIZE_X; i++) {
        for(int j = 0; j < RELIEF_PHYSICAL_SIZE_Y; j++){
            
            int output = int(allPixels[RELIEF_PHYSICAL_SIZE_X*j+i]);
            ofSetColor(0,0,ofMap(output, 0, 160, 0, 255));
            ofRect(i*pixelSize,j*pixelSize,pixelSize,pixelSize);
        }
    }
    ofNoFill();
    ofSetColor(255, 0, 0);
    ofRect(PINBLOCK_0_X_OFFSET*pixelSize, 0, PINBLOCK_0_WIDTH*pixelSize, RELIEF_SIZE_Y*pixelSize);
    ofRect(PINBLOCK_1_X_OFFSET*pixelSize, 0, PINBLOCK_1_WIDTH*pixelSize, RELIEF_SIZE_Y*pixelSize);
    ofRect(PINBLOCK_2_X_OFFSET*pixelSize, 0, PINBLOCK_2_WIDTH*pixelSize, RELIEF_SIZE_Y*pixelSize);
    
    ofPopMatrix();
    
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

int TouchShapeObject::xCoordinateShift (int num){
    int val = num;
    if (num<16) {
        val =  PINBLOCK_0_X_OFFSET +num ;
    } else if (num<32){
        val = PINBLOCK_1_X_OFFSET +num -16 ;
    } else {
        val = PINBLOCK_2_X_OFFSET +num  -32;
    }
    return val;
}

//----------------------------------------------------

void TouchShapeObject::singleElasticSurface()
{
    //*** MODE: Multi Touch Deformation ***//
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            allPixels[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)] = HIGH_THRESHOLD;
            
        }   
    }
    
    int rangeDef = 10; //range of deformation
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            if (isTouched[i][j]) {
                unsigned char h = MAX(LOW_THRESHOLD,mPinHeightReceive[i * lineSize + j]);
                for (int ii = MAX(0,i - rangeDef); ii< MIN(RELIEF_SIZE_X,i+rangeDef); ii++) {
                    for (int jj = MAX(0,j - rangeDef); jj< MIN(RELIEF_SIZE_Y,j+rangeDef); jj++) {
                        int d = ofDist(i, j, ii, jj);
                        if(d>rangeDef){ d = rangeDef; };
                        int dHeight = ofMap(d, 0, rangeDef, (int)h, HIGH_THRESHOLD);
                        dHeight = MAX(LOW_THRESHOLD, dHeight);
                        allPixels[RELIEF_PHYSICAL_SIZE_X* jj+ xCoordinateShift(ii)] =  MIN(allPixels[RELIEF_PHYSICAL_SIZE_X* jj+ xCoordinateShift(ii)],dHeight);
                    }
                }
            }
            
        }
    }
    
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            if (isTouched[i][j]) {
                unsigned char h = MAX(LOW_THRESHOLD,mPinHeightReceive[i * lineSize + j]);
                h = MIN((int)h+35,HIGH_THRESHOLD);
                allPixels[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)] = HIGH_THRESHOLD; //(int)h;
                for(int k = 0; k < filterFrame; k++){
                    allPixels_store[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)][k] = HIGH_THRESHOLD; //(int)h;
                }
            }
        }
    }
}

//----------------------------------------------------

void TouchShapeObject::triSurface()
{
    
    
    //*** FIRST Surface;  Rigid Surface ***//
    int minHeight = HIGH_THRESHOLD;
    for(int i = 0; i< 16; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            if (isTouched[i][j]) {
                unsigned char h = MAX(LOW_THRESHOLD,mPinHeightReceive[i * lineSize + j]);
                minHeight = MIN(h,minHeight);
            }
        }
    }
    
    for(int i = 0; i< 16; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            allPixels[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)] = minHeight;
        }
    }
    
    for(int i = 0; i< 16; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            if (isTouched[i][j]) {
                unsigned char h = MAX(LOW_THRESHOLD,mPinHeightReceive[i * lineSize + j]);
                h = MIN((int)h+35,HIGH_THRESHOLD);
                allPixels[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)] = HIGH_THRESHOLD; //(int)h;
                for(int k = 0; k < filterFrame; k++){
                    allPixels_store[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)][k] = HIGH_THRESHOLD; //(int)h;
                }
            }
        }
    }
    
    
    //*** SECOND Surface; Elastisity ***//
    for(int i = 16; i< 32; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            allPixels[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)] = HIGH_THRESHOLD;
            
        }
    }
    
    int rangeDef = 12; //range of deformation
    for(int i = 16; i< 32; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            if (isTouched[i][j]) {
                unsigned char h = MAX(LOW_THRESHOLD,mPinHeightReceive[i * lineSize + j]);
                for (int ii = MAX(16,i - rangeDef); ii< MIN(32,i+rangeDef); ii++) {
                    for (int jj = MAX(0,j - rangeDef); jj< MIN(RELIEF_SIZE_Y,j+rangeDef); jj++) {
                        int d = ofDist(i, j, ii, jj);
                        if(d>rangeDef){ d = rangeDef; };
                        int dHeight = ofMap(d, 0, rangeDef, (int)h, HIGH_THRESHOLD);
                        dHeight = MAX(LOW_THRESHOLD, dHeight);
                        allPixels[RELIEF_PHYSICAL_SIZE_X* jj+ xCoordinateShift(ii)] =  MIN(allPixels[RELIEF_PHYSICAL_SIZE_X* jj+ xCoordinateShift(ii)],dHeight);
                    }
                }
            }
            
        }
    }
    
    for(int i = 16; i< 32; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            if (isTouched[i][j]) {
                unsigned char h = MAX(LOW_THRESHOLD,mPinHeightReceive[i * lineSize + j]);
                h = MIN((int)h+35,HIGH_THRESHOLD);
                allPixels[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)] = HIGH_THRESHOLD; //(int)h;
                for(int k = 0; k < filterFrame; k++){
                    allPixels_store[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)][k] = HIGH_THRESHOLD; //(int)h;
                }
            }
        }
    }
    
    
    ////*** THIRD Surface; elastic Surface (pull) ***////
    for(int i = 32; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            allPixels[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)] = LOW_THRESHOLD;
        }
    }
    
    
    
    rangeDef = 8;
    for(int i = 32; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            if (isTouched[i][j]) {
                unsigned char h = MAX(LOW_THRESHOLD,MIN(HIGH_THRESHOLD,mPinHeightReceive[i * lineSize + j]));
                for (int ii = MAX(32,i - rangeDef); ii< MIN(RELIEF_SIZE_X,i+rangeDef); ii++) {
                    for (int jj = MAX(0,j - rangeDef); jj< MIN(RELIEF_SIZE_Y,j+rangeDef); jj++) {
                        int d = ofDist(i, j, ii, jj);
                        if(d>rangeDef){ d = rangeDef; };
                        int dHeight = ofMap(d, 0, rangeDef, (int)h, LOW_THRESHOLD);
                        dHeight = MAX(LOW_THRESHOLD, dHeight);
                            allPixels[RELIEF_PHYSICAL_SIZE_X* jj+ xCoordinateShift(ii)] = MAX((int)allPixels[RELIEF_PHYSICAL_SIZE_X* jj+ xCoordinateShift(ii)], dHeight);
                    }
                }
            }
            
        }
    }
    
    for(int i = 32; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            if (isTouched[i][j]) {
                allPixels[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)] = LOW_THRESHOLD; //(int)h;
                for(int k = 0; k < filterFrame; k++){
                    allPixels_store[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)][k] = LOW_THRESHOLD; //(int)h;
                }
            }
        }
    }
    
    
    
}


//----------------------------------------------------

void TouchShapeObject::waveSurface()
{
    //store the received value
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for (int j =0; j<RELIEF_SIZE_Y; j++) {
            for (int k = NUM_WAVE_FRAME -1; k >0 ; k--) {
                mPinHeightReceive_store[i][j][k] = mPinHeightReceive_store[i][j][k-1];
            }
            if(isTouched[i][j]){
                mPinHeightReceive_store[i][j][0] = mPinHeightReceive[i * lineSize + j];
            } else {
                mPinHeightReceive_store[i][j][0] = defaultHeight;
            }
        }
    }
    
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            allPixels[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)] = defaultHeight;
        }
    }

    for (int i = 0; i < RELIEF_SIZE_X; i++) {
        for(int j = 0; j < RELIEF_SIZE_Y; j++){
            for (int k = 1; k < NUM_WAVE_FRAME ; k++) {
                if (mPinHeightReceive_store[i][j][k] < defaultHeight) {
                    for (int ii = MAX(0, i-k); ii<MIN(RELIEF_SIZE_X, i+k+1); ii++) {
                        for (int jj = MAX(0, j-k); jj<MIN(RELIEF_SIZE_Y, j+k+1); jj++) {
                            int d = ofDist(i, j, ii, jj);
                            if (d==k) {
                                allPixels[RELIEF_PHYSICAL_SIZE_X* jj + xCoordinateShift(ii)] = MIN(HIGH_THRESHOLD, allPixels[RELIEF_PHYSICAL_SIZE_X* jj + xCoordinateShift(ii)] + (LOW_THRESHOLD - mPinHeightReceive_store[i][j][k])/2);
                            }
                        }
                    }
                }
            }
        }
    }
    
    
    for(int i = 0; i< RELIEF_SIZE_X; i++){
        for(int j = 0; j< RELIEF_SIZE_Y; j++){
            if (isTouched[i][j]) {
                allPixels[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)] = defaultHeight; //(int)h;
                for(int k = 0; k < filterFrame; k++){
                    allPixels_store[RELIEF_PHYSICAL_SIZE_X* j+ xCoordinateShift(i)][k] = defaultHeight; //(int)h;
                }
            }
        }
    }
    
    
}

void TouchShapeObject::modeChange()
{
    touchMode++;
    TouchShapeObject();
    if(touchMode == touchModeNum){
        touchMode=0;
    }
    
}

