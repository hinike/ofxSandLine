#include "ofxSandLine.h"

//------------------------------------------------
ofxSandLine::ofxSandLine(){

}

//------------------------------------------------
ofxSandLine::ofxSandLine(ofPoint _p1, ofPoint _p2){
    
    p1 = _p1;
    p4 = _p2;
    
    getMidPoints(p1, p4);
    
    p2 = midPoints[0];
    p3 = midPoints[1];
    
    mode = "SAND_MODE_LINE";
    
    setDefaults();
}

//------------------------------------------------
ofxSandLine::ofxSandLine(ofPoint _p1, ofPoint _p2, ofPoint _p3, ofPoint _p4){
    
    p1 = _p1;
    p2 = _p2;
    p3 = _p3;
    p4 = _p4;
    
    mode = "SAND_MODE_SPLINE";
    
    setDefaults();
    
}

//------------------------------------------------
void ofxSandLine::update(int _res){
    
    res = _res;
    
    points = makeSpline(p1, p2, p3, p4, res * 4);
    
    grains.clear();
    
    for(int i = 0; i < points.size()/4; i++){
        ofPoint grain_;
        grain_ = points[ofRandom(points.size())];
        grain_ += ofVec2f(breadth * ofRandomf(),  breadth * ofRandomf());
        
        grains.push_back(grain_);
    }
    
    if(mode == "SAND_MODE_LINE"){
        p1 += ofVec2f(offset * ofRandomf(), offset * ofRandomf());
        p4 += ofVec2f(offset * ofRandomf(), offset * ofRandomf());
        
        getMidPoints(p1, p4);
        
        p2 = midPoints[0];
        p3 = midPoints[1];
        
    }
    
    if(mode == "SAND_MODE_SPLINE"){
    p1 += ofVec2f(offset * ofRandomf(), offset * ofRandomf());
    p2 += ofVec2f(offset * ofRandomf(), offset * ofRandomf());
    p3 += ofVec2f(offset * ofRandomf(), offset * ofRandomf());
    p4 += ofVec2f(offset * ofRandomf(), offset * ofRandomf());
    }
    
}

//------------------------------------------------
void ofxSandLine::draw(int _res){
    
    update(_res);
    
    for(int i = 0; i<grains.size(); i++){
        ofSetColor(color, ofRandom(maxAlpha));
        ofDrawCircle(grains[i].x, grains[i].y, ofRandom(maxSize));
    }
}

//------------------------------------------------
void ofxSandLine::setOffset(float _offset){
    offset = _offset;
}

//------------------------------------------------
void ofxSandLine::setBreadth(float _breadth){
    breadth = _breadth;
}

//------------------------------------------------
void ofxSandLine::forceSetMode(string _mode){
    if((_mode == "SAND_MODE_SPLINE") || (_mode == "SAND_MODE_LINE")){
        mode = _mode;
    } else {
        cout << "Wrong mode specified, acceptable modes are SAND_MODE_LINE or SAND_MODE_SPLINE" << endl;
    }
}

//------------------------------------------------
void ofxSandLine::setColor(ofColor _c){
    color = _c;
}

//------------------------------------------------
void ofxSandLine::setMaxSize(int _maxSize){
    maxSize = _maxSize;
}

//------------------------------------------------
void ofxSandLine::setMaxAlpha(int _maxAlpha){
    maxAlpha = _maxAlpha;
}

//------------------------------------------------
vector<ofPoint> ofxSandLine::makeSpline(ofPoint _p1, ofPoint _p2, ofPoint _p3, ofPoint _p4, int _res){
    
    vector<ofPoint> points(res + 1);
    
    ofMatrix4x4 controlPoints;
    ofMatrix4x4 splineMatrix;
    
    controlPoints._mat[0] = ofVec4f(_p1.x, _p2.x, _p3.x, _p4.x);
    controlPoints._mat[1] = ofVec4f(_p1.y, _p2.y, _p3.y, _p4.y);
    controlPoints._mat[2] = ofVec4f(_p1.z, _p2.z, _p3.z, _p4.z);
    controlPoints._mat[3] = ofVec4f(1.0, 1.0, 1.0, 1.0);
    
    splineMatrix._mat[0] = ofVec4f(1.0, -3.0, 3.0, -1.0);
    splineMatrix._mat[1] = ofVec4f(0, 3.0, -6.0, 3.0);
    splineMatrix._mat[2] = ofVec4f(0, 0, 3.0, -3.0);
    splineMatrix._mat[3] = ofVec4f(0, 0, 0, 1.0);
    
    for(int i = 0; i <=res; i++){
        float t = float(i)/res;
        ofVec4f point = controlPoints * splineMatrix * ofVec4f(1, t, pow(t, 2), pow(t, 3));
        points[i] = ofVec3f(point.x, point.y, point.z);
    }
    
    return points;
    
}

//------------------------------------------------
void ofxSandLine::getMidPoints(ofPoint _p1, ofPoint _p4){
    
    midPoints[0] = (_p1 * 2.0/float(3)) + (_p4 * 1.0/float(3));
    midPoints[1] = (_p1 * 1.0/float(3)) + (_p4 * 2.0/float(3));
    
}

//------------------------------------------------
void ofxSandLine::setDefaults(){
    
    offset = 1.0;
    breadth = 1;
    color = (255, 255, 255);
    maxSize = 1;
    maxAlpha = 64;
    
}
