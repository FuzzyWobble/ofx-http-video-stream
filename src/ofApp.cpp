// =============================================================================
//
// Copyright (c) 2012-2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofApp.h"


void ofApp::setup()
{
    ofSetFrameRate(8);
    ofSetVerticalSync(true);
    
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    for(int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    vidGrabber.setDeviceID(1);
    vidGrabber.setDesiredFrameRate(CAMFPS);
    vidGrabber.initGrabber(CAMX,CAMY);


    ofx::HTTP::SimpleIPVideoServerSettings settings;
    settings.setPort(9696);
    settings.ipVideoRouteSettings.setMaxClientConnections(100); //100 is clearly too high. but at what point have we crossed the threshold? would be wise to serve a static after a given number of connections.
    server.setup(settings);
    server.start();

#if !defined(TARGET_LINUX_ARM)
    // Launch a browser with the address of the server.
    ofLaunchBrowser(server.getURL());
#endif

}


void ofApp::update()
{
    
    ofBackground(0);
    
    vidGrabber.update();
    if(vidGrabber.isFrameNew()){
        server.send(vidGrabber.getPixels());
    }
    

}


void ofApp::draw()
{

    ofSetColor(255);
    vidGrabber.draw(0,0);

    // Display the number of connected clients for reference.
    std::stringstream ss;
    ss << "Num clients connected: ";
    ss << server.getNumConnections();
    ofDrawBitmapStringHighlight(ss.str(), 20, 20);
}




