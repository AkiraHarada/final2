
#include "ofxOpenNI.h"
#include "ofMain.h"

class ofApp : public ofBaseApp{
    
//ofxOpenNI
private:
    ofxOpenNI kinect;
    ofImage maskedImage,scenery;
    int circleRGBAsum[640*480], circleADD[16*12], circleImageData[640*480];
    ofColor Circlecolor[16*12];
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
    
    
    
};

