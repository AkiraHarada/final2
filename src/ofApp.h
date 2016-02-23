
#include "ofxOpenNI.h"
#include "ofMain.h"

class ofApp : public ofBaseApp{
    
    //ofxOpenNI
private:
    ofxOpenNI kinect;
    //ofImage maskedImage;
    float circleADD[32*24];
    int circleImageData[640*480];
    //vector<int> circleImageData, circleADD;
    //char distanceBuffer[100];
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
    
    ofImage scenery;
    ofPoint p[2];
    ofPoint preP[2];
    float acc[2];
    int mode;
    float  circleY[2];
    bool ballappear;
};

