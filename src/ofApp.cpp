#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofSetWindowShape(640, 480);
    ofSetFrameRate(30);
    //ofEnableDepthTest();
    ofEnableSmoothing();
    //ofBackground(0, 0, 0);
    
    light.enable();
    light.setPointLight();
    light.setPosition(320, 0, 1000);
    light.setAmbientColor(ofFloatColor(0.5, 0.2, 0.2, 1.0));
    light.setDiffuseColor(ofFloatColor(0.5, 0.5, 1.0));
    light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    
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
    
    bool ballappear[] = {false, false};
    bool ballthrown = false;
    int time = 0;
    
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
            int newacc = ofDist(p[i].x, p[i].y, preP[i].x, preP[i].y);
            if (acc[i] < 100 && newacc > 100){
                if((p[i].y-preP[i].y) < 0) {
                    ballappear[i] = true;
                } else {
                    ballappear[i] = false;
                }
            }
            acc[i] = newacc;
            
            if((ballappear[0]==true && ballappear[1] ==false) ||(ballappear[1]==true && ballappear[0] ==false)){//円が一個しかないときに以下を行う
                if((((p[i].x-preP[i].x) > 40) || ((p[i].x-preP[i].x) < -40)) && (ballthrown == false)){
                    ballthrown == true;//ボールを投げるスイッチを入れる(横方向への速度の絶対値が一定値以上のとき)
                    Xvelocity = 10;//(p[i].x - preP[i].x)/3;//その時の初速度を手の初速度として代入
                    Yvelocity = 10;//(p[i].y = preP[i].y)/3;
                    XoriginalPosition = p[i].x;//円の中心はそのときの手の位置
                    YoriginalPosition = p[i].y;
                }
            }
        }
        
        for(int j = 0; j < 2; j++){
            preP[j] = p[j];
        }
        
        
        for(int i = 0; i < 2; i++){
            if((ballappear[i] == true) && (ballthrown == false)){
                suisyou[i].set(30, 10);
                suisyou[i].setPosition(p[i].x, p[i].y-(40*ofNoise(0.5, 1.0)), 0);
            } else if(ballthrown == true){
                suisyou[i].set(30, 10);
                suisyou[i].setPosition(XoriginalPosition+Xvelocity*time, YoriginalPosition+Yvelocity*time+10.0*time*time, 0);
                time++;
            }
        }
    }
}



//--------------------------------------------------------------
void ofApp::draw(){
    //kinect.drawDepth(0,0,640,480);
    ofDisableNormalizedTexCoords();
    ofBackground(0, 0, 0);
    ofSetColor(255, 255, 255);
    scenery.draw(0, 0, 640, 480);
    
    for(int i=0; i < 24; i++){
        for(int j=0; j < 32; j++){
            ofMap(circleADD[j+32*i], 0, 400, 0, 360);
            ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255), 255*circleADD[j+32*i]/360);
            ofTriangle(10+j*20, 10+i*20, 0,
                       5+j*20, 10+i*20, 0,
                       10+j*20, i*20, 0);
            ofTriangle(10+j*20, 10+i*20, 0,
                       15+j*20, 10+i*20, 0,
                       10+j*20, i*20, 0);
            ofTriangle(10+j*20, 10+i*20, 0,
                       5+j*20, 10+i*20, 0,
                       10+j*20, 20+i*20, 0);
            ofTriangle(10+j*20, 10+i*20, 0,
                       15+j*20, 10+i*20, 0,
                       10+j*20, 20+i*20, 0);
        }
    }
    for(int i = 0; i < 2; i++){
        if(ballappear[i] == true){
            if(ballthrown==false){
                ofSetColor(0, 255, 255);
                suisyou[i].draw();
                //                ofSetColor(255, 255, 255);
                //                ofCircle(p[i].x, p[i].y, 10);
                //                ofCircle(30+ofNoise(30.0, 60.0), p[i].y, 30);
            }else if(ballthrown==true){
                ofSetColor(0, 255, 255);
                suisyou[i].draw();
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
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}