#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofSetWindowShape(640, 480);
    ofSetFrameRate(30);
    //ofBackground(0, 0, 0);
    
    kinect.setup();
    kinect.setRegister(true);
    kinect.setMirror(true);
    kinect.addDepthGenerator();
    kinect.addImageGenerator();
    
    kinect.addHandsGenerator();
    kinect.addAllHandFocusGestures();
    kinect.setMaxNumHands(2);
    
    kinect.getDepthGenerator().GetAlternativeViewPointCap().SetViewPoint(kinect.getImageGenerator());
    
    kinect.start();
    
    //mode = 0;
    ballappear = false;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    kinect.update();
    scenery.loadImage("stage.jpg");
    
    unsigned short *depthData = kinect.getDepthRawPixels().getPixels();
    
    for(int k = 0; k < 640*480; k++){
        circleImageData[k] = 0;
    }
    for(int i = 0; i < 32*24; i++){
        circleADD[i] = 0;
    }
    
    for(int k = 0; k < 640*480; k++){
        if(880 <= depthData[k] && depthData[k] < 1300){
            circleImageData[k] = 1;
        } else{
            circleImageData[k] = 0;
        }
    }
    for(int i=0; i < 24; i++){
        for(int j=0; j < 32; j++){
            for(int k=0; k < 20; k++){
                for(int l=0; l < 20; l++){
                    circleADD[j+32*i] += circleImageData[640*20*i+j*20+640*k+l];
                }
            }
        }
    }
    if(kinect.getNumTrackedHands() > 0){
        for(int i = 0; i < kinect.getNumTrackedHands(); i++){
            ofxOpenNIHand hand = kinect.getTrackedHand(i);
            p[i] = hand.getPosition();
            //ofCircle(p[i].x, p[i].y, 30);
            int newacc = ofDist(p[i].x, p[i].y, preP[i].x, preP[i].y);
            circleY[i] = ofNoise(50.0);
            if (acc[i] < 20 && newacc > 20) {
                ballappear = true; //一度現れたら消えない
            }
            acc[i] = newacc;
        }
        for(int j = 0; j < 2; j++){
            preP[j] = p[j];
        }
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    //    kinect.drawDepth(0,0,640,480);
    ofDisableNormalizedTexCoords();
    ofBackground(0, 0, 0);
    ofSetColor(255, 255, 255);
    scenery.draw(0, 0, 640, 480);
    for(int i=0; i < 24; i++){
        for(int j=0; j < 32; j++){
            if(circleADD[j+32*i]>120){
                ofMap(circleADD[j+32*i], 0, 400, 0, 360);
                ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255), 255*circleADD[j+32*i]/360);
                
            } else if((circleADD[j+32*i] <= 120) || (circleADD[j+32*i] > 90)){
                ofMap(circleADD[j+32*i], 0, 400, 0, 360);
                ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255), 255*circleADD[j+32*i]/360);
                
            } else {
                ofMap(circleADD[j+32*i], 0, 400, 0, 360);
                ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255), 255*circleADD[j+32*i]/360);
            }
            ofTriangle(10+j*20, 10+i*20,
                       5+j*20, 10+i*20,
                       10+j*20, i*20);
            ofTriangle(10+j*20, 10+i*20,
                       15+j*20, 10+i*20,
                       10+j*20, i*20);
            ofTriangle(10+j*20, 10+i*20,
                       5+j*20, 10+i*20,
                       10+j*20, 20+i*20);
            ofTriangle(10+j*20, 10+i*20,
                       15+j*20, 10+i*20,
                       10+j*20, 20+i*20);
        }
    }
    for(int i = 0; i < 2; i++){
        if(ballappear == true){
            ofSetColor(0, 255, 255);
            //ofCircle(p[i].x, 50+circleY[i], 30);
            ofCircle(30+ofNoise(30.0, 60.0), p[i].y, 30);
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}