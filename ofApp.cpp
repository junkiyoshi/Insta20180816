#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);

	this->cap.open("D:\\video\\image26.mp4");
	this->cap_size = cv::Size(1280, 720);

	cv::Mat cap_frame;
	while (true) {

		this->cap >> cap_frame;
		if (cap_frame.empty()) { 
			
			break; 
		}

		cv::resize(cap_frame, cap_frame, this->cap_size);
		cv::cvtColor(cap_frame, cap_frame, CV_BGR2RGB);
		this->cap_frames.push_back(cap_frame);

		unique_ptr<ofImage> image(new ofImage());
		image->allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
		this->images.push_back(move(image));
	}

	this->blend_image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->blend_frame = cv::Mat(this->blend_image.getHeight(), this->blend_image.getWidth(), CV_MAKETYPE(CV_8UC3, this->blend_image.getPixels().getNumChannels()), this->blend_image.getPixels().getData(), 0);
}

//--------------------------------------------------------------
void ofApp::update() {

	int blend_count = 5;
	int frame_span = 1;
	cv::Mat diff, gray_diff, color_diff;

	this->blend_frame = this->cap_frames[(ofGetFrameNum() + blend_count * frame_span) % this->cap_frames.size()] * 1.0;
	for (int i = 0; i < blend_count; i++) {
	
		cv::absdiff(this->cap_frames[(ofGetFrameNum() + i * frame_span) % this->cap_frames.size()], this->cap_frames[(ofGetFrameNum() + (i + 1) * frame_span) % this->cap_frames.size()], diff);
		cv::cvtColor(diff, gray_diff, CV_RGB2GRAY);
		cv::threshold(gray_diff, gray_diff, 64, 255, CV_THRESH_BINARY);

		cv::applyColorMap(diff, color_diff, cv::COLORMAP_HSV);


		cv::imshow("diff", diff);
		cv::imshow("gray_diff", gray_diff);
		cv::imshow("color_diff", color_diff);
		cv::waitKey(1);

		cv::add(this->blend_frame, diff, this->blend_frame, gray_diff);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255);
	this->blend_image.update();
	this->blend_image.draw(0, 0);
}

//--------------------------------------------------------------
int main() {
	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
