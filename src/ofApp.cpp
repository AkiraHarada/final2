#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(0, 0, 0);
    ofSetWindowShape(640, 480);
    ofSetFrameRate(30);
    
    kinect.setup();
    kinect.setRegister(true);
    kinect.setMirror(true);
    kinect.addDepthGenerator();
    kinect.addImageGenerator();
    
    kinect.getDepthGenerator().GetAlternativeViewPointCap().SetViewPoint(kinect.getImageGenerator());
    
    kinect.start();
    
    //maskedImage.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);
    //scenery.loadImage("stage.jpeg");
    //circleImage.allocate(640, 480, OF_IMAGE_GRAYSCALE);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    //unsigned char *maskedImageData = maskedImage.getPixels();
    //unsigned char *imageData = kinect.getImagePixels().getPixels();
    //unsigned char *circleImageData = circleImage.getPixels();
    unsigned short *depthData = kinect.getDepthRawPixels().getPixels();
    
    
    for(int k = 0; k < 640*480; k++){
        if(880 <= depthData[k] && depthData[k] < 1200){
            circleImageData[k] = 1;
        } else{
            circleImageData[k] = 0;
        }
    }
//    for(int i = 0; i < 640*480; i++){
//        if((circleImageData[i*4+3]) == 0){
//            circleRGBAsum[i]=0;
//        } else{
//            circleRGBAsum[i]=1;
//        }
//    }
    for(int i=0; i < 12; i++){
        for(int j=0; j < 16; j++){
            for(int k=0; k < 40; k *=640){
                for(int l=0; l < 40; l++){
                    circleADD[j+16*i] += circleImageData[640*40*i+j*40+k+l];
                }
            }
        }
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    for(int i=0; i < 12; i++){
        for(int j=0; j < 16; j++){
            if(circleADD[j+16*i]>480){
                ofSetColor(255, 255, 255);
                ofCircle(20+(j-1)*40, 20+(i-1)*40, 20);
            }
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
    //    unsigned short*depthData = kinect.getDepthRawPixels().getPixels();
    //    unsigned short depthMM = depthData[y*640 + x];
    //    sprintf(distanceBuffer, "%d mm", depthMM);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}