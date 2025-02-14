#pragma once

#include "ofMain.h"
#include "baseVideoPlayer.h"
#include "ShapeIOManager.h"
#include "KinectTracker.h"
#include "ofImage.h"
#include "ofxMovieExporter.h"
#include "KinectVideoPlayer.h"
#include "MachineAnimationVideoPlayer.h"
#include "videoRecorder.h"
#include "TableSimulator.h"
#include <memory>
#include "ImageWarper.h"
#include "KinectShapeObject.h"
#include "MachineAnimationShapeObject.h"
#include "CalmShapeObject.h"
#include "FadeLoopShapeObject.h"
#include "FlockShapeObject.h"
#include "WavyShapeObject.h"
#include "EscherShapeObject.h"
#include "ImageShapeObject.h"
#include "HandShapeObject.h"
#include "TouchShapeObject.h"
#include "ofxTimeline.h"
#include "ofxUI.h"

// @todo see https://github.com/danomatika/ofxAppUtils for alternate quad wrapper, openGL based?

class ReliefApplication : public ofBaseApp  {
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    
    
    
    // buffer rendering
    void renderPinDisplayBuffer(); // render pin display to buffer
    void renderHeightMapBuffer(); // render height map to buffer
    
    void drawGeneralMessage();
    
    void keyPressed(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    
    bool useLiveKinect;
    bool useRecording;
    bool useTable = false;
    bool drawTableSimulation = true;
    
    // table config variables
    float                       gain_P;
    float                       gain_I;
    int                         max_I;
    int                         deadZone;
    int                         maxSpeed;
    
    ImageWarper                 * mImageWarper; // warps images from one coordinate system to another
    
    vector                      <ShapeObject*> mCurrentShapeObjects;
    vector                      <ShapeObject*> allShapeObjects;
    vector                      <string> allsShapeObjectsNames;
    
    bool                        controlManual = true;
    bool                        controlTimeline = false;
    bool                        isVideoLoaded = false;
    
    bool                        bEscherStart = false;
    bool                        bEscherRun = false;
    bool                        bEscherStop = false;
    vector <string>             allEscherModeNames;
    
    // this is the kinect image thresholded and rendered
    KinectShapeObject           * mKinectShapeObject;
    MachineAnimationShapeObject * mMachineAnimationShapeObject;
    FlockShapeObject            * mFlockShapeObject;
    WavyShapeObject             * mWavyShapeObject;
    CalmShapeObject             * mCalmShapeObject; // calm shape object, does nothing but used to reset
    FadeLoopShapeObject         * mFadeLoopShapeObject;
    ImageShapeObject            * mImageShapeObject;
    EscherShapeObject           * mEscherShapeObject;
    HandShapeObject             * mHandShapeObject;
    TouchShapeObject            * mTouchShapeObject;
    
    ofxCvColorImage             cvColorImage;
    ofxCvColorImage             cvWarpedImage;
    
    ShapeIOManager              * mIOManager; // manages communication with the pin display
    
    ofFbo                       pinDisplayImage; //FBO where we render graphics for pins
    ofFbo                       pinHeightMapImage; //FBO where we render height map
    ofFbo                       pinHeightMapImageSmall; //FBO where we render height map
    ofFbo                       pinHeightMapImage2;
    
    KinectTracker               mKinectTracker;
    ofRectangle                 guiIn;
       
    KinectVideoPlayer           kinectVideoPlayer;
    VideoRecorder               recorder;
    
    unsigned char fakeArray[RELIEF_SIZE_X][RELIEF_SIZE_Y];
    unsigned char pinHeightReceive[RELIEF_SIZE_X][RELIEF_SIZE_Y];
    
    unsigned char* compPixels;
    unsigned char* targetPixels;
    
    
    // setup available screens
    // this is more robust then setting by integer
    // to reorder the screens change order in array.
    string                      screens[3] = {"timeline", "kinect", "shapeobject"};
    int                         maxScreens = 3;
    int                         screenIndex = 0; // starting screen
    string                      currentScreen = screens[0];
    
    TableSimulator              * tableSimulation;
    
    ofxTimeline                 timeline;
    void bangFired(ofxTLBangEventArgs& args);
    void pageChanged(ofxTLPageEventArgs& args);
    
    ofxUITabBar                 * guiTabBar;
    ofxUICanvas                 * coolGui_1;
    ofxUICanvas                 * coolGui_2;
    ofxUICanvas                 * coolGui_3;
    ofxUICanvas                 * coolGui_4;
    ofxUICanvas                 * coolGui_5; // Escher mode
    
    ofxUICanvas                 * easyGui;
    ofxUICanvas                 * tableGui;
    ofxUICanvas                 * kinectGui;
    
    void guiEvent(ofxUIEventArgs &e); // kinect uses this too
    void tableGuiEvent(ofxUIEventArgs &e); // seperate table callback
    
    void pauseApp();
    void resumeApp();

private:
    Apex::ofxMovieExporter      movieExporter;
    void incrementAndSetCurrentScreen();
    void drawActualPinHeightImageFromTable(int x, int y, int w, int h);
    void setupTimeline();
    void setupTableGui();
    void setupKinectGui();
    void setupCoolGui();
    void setupEasyGui();
    
    void initalizeShapeObjects();
    void registerShapeObjectNamesForGui();
    bool isPaused = false;
    bool didPauseTimeline = false;
    string currentPage;
    bool bAnimationLooping = true;
    void setPageBasedOnBlobCount();
    bool isSwitchOnForCurrentPage(string switchName);
    float getValueForCurrentPage(string curveName);
    int getIntValueForCurrentPage(string curveName);
    void blendCurrentShapeObjectsByHighestValue();
    void setCurrentShapeObjectsFromActiveTimelineSwitches();
    void setPageAndPlayFromFirstFrame(string page_name);
    void repeatCurrentPage();
    bool isSkipFish = false;
    bool isSkipMachine = false;
    bool isEasyModeFishNotShyToggled = false;
};
