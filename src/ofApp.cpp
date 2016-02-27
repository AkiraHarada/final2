#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofSetWindowShape(640, 480);
    ofSetFrameRate(30);
    
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
    kinect.setMaxNumHands(1);
    
    kinect.getDepthGenerator().GetAlternativeViewPointCap().SetViewPoint(kinect.getImageGenerator());
    
    //    soundplay.loadSound("からくりピエロ.mp3");
    //    soundplay.setLoop(true);
    //    soundplay.play();
    
    ballappear = false;
    ballthrown = false;
    throwntime = 0;
    appeartime = 0;
    falsetime = 0;
    floatingtime = 0;
    noiseball = ofRandom(1);
    noiseheart = 0.1;
    kinect.start();
    
    
    //解決しないといけないこと
    //1 ballappearにしようとするときに誤って飛んでいってしまうのを修正　→ ballappearによって出てきてから一定時間は投げられないように調整
    //2 なかなか出てこないのをより改善する　→　速度を少し遅めに
    //3 ハートのギミックを付け加えたい　失敗した回数が増えるとハートが出てくる
    //4 背景を薄くしたい →　後ろに透明度100くらいの長方形を引く
    //5 上下にボールを少しゆらゆらさせたい → ofNoiseの正しい使い方
    //6 一定回数失敗すると終了にしたい
}

//--------------------------------------------------------------
void ofApp::update(){
    
    kinect.update();
    scenery.loadImage("stage.jpg");
    if(falsetime > 3){
        ofExit();
    }
    
    //ここからは体の描写
    
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
    
    //ここからはボールの描写
    if(kinect.getNumTrackedHands() > 0){
        ofxOpenNIHand hand = kinect.getTrackedHand(0);
        p = hand.getPosition();
        int newacc = ofDist(p.x, p.y, preP.x, preP.y);
        if (acc < 30){
            if((p.y-preP.y) < -30) {
                ballappear = true;
            } else if((p.y-preP.y) > 60){
                ballappear = false;
                floatingtime = 0;
            }
        }
        acc = newacc;
        if(ballappear == true){
            if((p.x - preP.x) > 30 || (p.x - preP.x) < -30){
                if(ballthrown == false && appeartime > 30){
                    ballthrown = true;//ボールを投げるスイッチを入れる(横方向への速度の絶対値が一定値以上のとき)
                    Xvelocity = (p.x - preP.x)/6;//その時の初速度を手の初速度として代入
                    Yvelocity = -16;
                    XoriginalPosition = p.x;//円の中心はそのときの手の位置
                    YoriginalPosition = p.y;
                    floatingtime = 0;
                }
            }
        }
        preP = p; //直前の手の速さを保存
    }
    
    //ボールを浮かせるためのコード
    if(ballappear == true){
        if((p.x - preP.x < 10) && (p.x - preP.x > -10)){
            floatingtime++;
            if(floatingtime == 60){
                floatingX = p.x;
                floatingY = p.y;
            }
        }
    }
    
    //ballthrownがtrueで投げられてから10フレーム以上経った時手が近くにあればボールを受け取る
    if((ballthrown == true) && throwntime > 10 ){
        //        cout << "dist:" << ofDist(circleX, circleY, p.x, p.y) << endl;
        if(ofDist(circleX, circleY, p.x, p.y) < 100){
            ballthrown = false;
            throwntime = 0;
            falsetime = 0;
        }
    }
    
    //ボールが飛び出たら消す
    if(circleX > 652.5 || circleX < -12.5){
        ballappear = false;
        ballthrown = false;
        circleX = 0;
        circleY = 0;
        throwntime = 0;
        appeartime = 0;
        falsetime = falsetime + 1;
    }
    
    //ボールが画面上に存在するときはballthrownの挙動によって描画変化
    if(ballappear == true){
        if(ballthrown == false){
            if(floatingtime < 60){
                suisyou.set(30, 10);
                suisyou.setPosition(p.x, p.y-(40*ofNoise(noiseball)), 0);
                appeartime = appeartime + 1;
            } else { //以下は浮かせるときの挙動
                suisyou.set(30,10);
                suisyou.setPosition(floatingX, floatingY-(40*ofNoise(noiseball)), 0);
            }
        } else if(ballthrown == true){
            suisyou.set(30, 10);
            suisyou.setPosition((XoriginalPosition+Xvelocity*throwntime), (YoriginalPosition+Yvelocity*throwntime+0.20*throwntime*throwntime), 0);
            circleX = XoriginalPosition+Xvelocity*throwntime;
            circleY = YoriginalPosition+Yvelocity*throwntime+0.20*throwntime*throwntime;
            throwntime = throwntime + 1;
        }
    }
    noiseball = noiseball + 0.1;
    R = 3.5*ofNoise(noiseheart);
    noiseheart = noiseheart + 0.01;
}




//--------------------------------------------------------------
void ofApp::draw(){
    
    ofDisableNormalizedTexCoords();
    ofBackground(0, 0, 0);
    ofSetColor(255, 255, 255);
    scenery.draw(0, 0, 640, 480);
    ofSetColor(0, 0, 0, 200);
    ofRect(0, 0, 640, 480);
    
    //400pixelの中にいくつ条件を満たしたpixelがあったかによって透明度を変化させる
    for(int i=0; i < 24; i++){
        for(int j=0; j < 32; j++){
            ofMap(circleADD[j+32*i], 0, 400, 0, 360);
            ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255), (255*circleADD[j+32*i]/360)-falsetime*80);
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
    
    //    ofColor colorHsb = ofColor::fromHsb(255, 255, 15+80*(3-falsetime), 255*falsetime/3);
    //    ofSetColor(colorHsb);
    //    ofCircle(320, 240, ofNoise(noiseball)*70);
    
    //ボールの描画
    ofEnableDepthTest();
    if(ballappear == true){
        //        ofSetColor(255, 255 ,255);
        //        ofCircle(p.x, p.y, 100);
        ofSetColor(0, 255, 255);
        suisyou.draw();
    }
    ofDisableDepthTest();
    
    //ハートの描画
    ofFill();
    ofColor colorHsb = ofColor::fromHsb(255, 255, 75+60*(3-falsetime), 255*falsetime/3);
    ofSetColor(colorHsb);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2+50);
    ofBeginShape();
    for (int theta = 0; theta < 360; theta++) {
        float x = R * (16 * sin(ofDegToRad(theta)) * sin(ofDegToRad(theta)) * sin(ofDegToRad(theta)));
        float y = (-1) * R * (13 * cos(ofDegToRad(theta)) - 5 * cos(ofDegToRad(2 * theta))
                              - 2 * cos(ofDegToRad(3 * theta)) - cos(ofDegToRad(4 * theta)));
        
        ofVertex(x, y);
    }
    ofEndShape();
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