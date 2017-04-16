#pragma once

#include "ofMain.h"
#include "constant.h"

class Effector {
public:
	void setup();
	void setBlur(Direction, const float&);
	void setBlurRotate(const float&);
	void setBlurBrightness(const float&);
	void setBloomSpread(Direction, const float&);
	void setBloomGain(const float&);
	
	void record();
	void drawBlur();
	void process();

private:
	void drawSquare();

	float blur[2];
	float blurRotate;
	float blurBrightness;
	float bloomSpread[2];
	float bloomGain;

	ofShader bloomShader;
	ofFbo effect2DFbo;
	ofFbo bloom2DFbo;
	ofTexture recuresiveTex;
};


inline void Effector::setup() {
	bloomShader.load("shader/bloom");
	recuresiveTex.allocate(WIDTH, HEIGHT, GL_RGB);
	effect2DFbo.allocate(WIDTH, HEIGHT);
	bloom2DFbo.allocate(WIDTH, HEIGHT);
}

inline void Effector::record() {
	effect2DFbo.begin();
}

inline void Effector::setBlur(Direction dir, const float& b) {
	switch (dir) {
		case Direction::x: blur[0] = b; break;
		case Direction::y: blur[1] = b; break;
	}
}

inline void Effector::setBlurRotate(const float& bRotate) {
	blurRotate = bRotate;
}

inline void Effector::setBlurBrightness(const float& bBrightness) {
	blurBrightness = bBrightness;
}

inline void Effector::setBloomSpread(Direction dir, const float &bSpread ){
	switch (dir) {
		case Direction::x: bloomSpread[0] = bSpread; break;
		case Direction::y: bloomSpread[1] = bSpread; break;
	}
}

inline void Effector::setBloomGain(const float &bGain){
	bloomGain = bGain;
}

inline void Effector::drawBlur() {
	ofPushMatrix();

	float col = blurBrightness;
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	glColor4f(1.0, 1.0, 1.0, blurBrightness);
	recuresiveTex.bind();
	glRotatef(blurRotate, 0, 0, 1);


	glBegin(GL_POLYGON);
	glTexCoord2f(0, HEIGHT);
	glVertex2f(-blur[0], -blur[1]);

	glTexCoord2f(WIDTH, HEIGHT);
	glVertex2f(WIDTH + blur[0], -blur[1]);

	glTexCoord2f(WIDTH, 0);
	glVertex2f(WIDTH + blur[0], HEIGHT + blur[1]);

	glTexCoord2f(0, 0);
	glVertex2f(-blur[0], HEIGHT + blur[1]);
	glEnd();

	recuresiveTex.unbind();
	ofDisableBlendMode();

	ofPopMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0);
}

inline void Effector::process() {

	effect2DFbo.end(); // end of recording
	effect2DFbo.draw(0,0); // end of recording

	//bloom2DFbo.begin();
	//bloomShader.begin();
	//bloomShader.setUniform1f("bloomgain", bloomGain);
	//bloomShader.setUniform2f("bloomspread", bloomSpread[0], bloomSpread[1]);
	//bloomShader.setUniformTexture("basetex", effect2DFbo.getTextureReference(), 0);
	//
	//drawSquare();

	//bloomShader.end();
	//bloom2DFbo.end();
	//bloom2DFbo.draw(0, 0);
	recuresiveTex.loadScreenData(0, 0, WIDTH, HEIGHT);
}

inline void Effector::drawSquare() {
	glEnable(GL_TEXTURE_RECTANGLE_ARB);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, HEIGHT);
	glVertex2f(0, 0);
	glTexCoord2f(WIDTH, HEIGHT);
	glVertex2f(WIDTH, 0);
	glTexCoord2f(WIDTH, 0);
	glVertex2f(WIDTH, HEIGHT);
	glTexCoord2f(0, 0);
	glVertex2f(0, HEIGHT);
	glEnd();
	glDisable(GL_TEXTURE_RECTANGLE_ARB);
}