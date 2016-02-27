
#include "ofxOpenNI.h"
#include "ofMain.h"

class ofApp : public ofBaseApp{
    
    //ofxOpenNI
private:
    
    ofxOpenNI kinect;
    float circleADD[32*24];
    int circleImageData[640*480];
    
public:
    
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    //ofPoint p[2],preP[2];
    //float acc[2], newacc[2];
    //float  circleY[2];
    //bool ballappear[2], ballthrown;
    //ofSpherePrimitive suisyou[2];
    //int HandThrowNumber, HandAppearNumber;
    
    ofImage scenery;
    ofPoint p,preP;
    ofLight light;
    ofSpherePrimitive suisyou;
    ofSoundPlayer soundplay;
    int throwntime, appeartime, falsetime, floatingtime;
    float R;
    float acc;
    float Xvelocity, Yvelocity;
    float XoriginalPosition, YoriginalPosition;
    float circleX, circleY;
    float noiseball, noiseheart;
    float floatingX, floatingY;
    bool ballappear, ballthrown;
};

