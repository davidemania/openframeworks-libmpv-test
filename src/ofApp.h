//**********************************************************************************//
//   _  _   _____   ____  _____   ____    ___  _____  _               ___    ___    //
//  | || | |  __ \ / __ \|  __ \ / __ \  |__ \|  __ \| |             |__ \  / _ \   //
//  | || |_| |  | | |  | | |  | | |  | |    ) | |__) | | __ _ _   _     ) || | | |  //
//  |__   _| |  | | |  | | |  | | |  | |   / /|  ___/| |/ _` | | | |   / / | | | |  //
//     | | | |__| | |__| | |__| | |__| |  / /_| |    | | (_| | |_| |  / /_ | |_| |  //
//     |_| |_____/ \____/|_____/ \____/  |____|_|    |_|\__,_|\__, | |____(_)___/   //
//                                                            __/ /                 //
//                                                           |___/                  //
//                                                                                  //
//  Micro Video Server, client and standalone system                                //
//  Software by davide Manià & 4DODO. All rights reserved, (c) 2019 4DODO srl       //
//                                                                                  //
//  ofApp.h -> main application declarations header                                 //
//                                                                                  //
//**********************************************************************************//

#pragma once

#include "ofMain.h"

#include <mpv/client.h>
#include <mpv/render_gl.h>

class ofApp : public ofBaseApp
{
	public:
        ofApp(int width, int height);

		void setup();
		void update();
		void draw();
		void exit();
        
		void keyPressed(int key);

    private:
        static void wakeupCallback(void* data);
        void wakeupCallbackReceived();
        
        static void renderCallback(void* data);
        void renderCallbackReceived();

		mpv_handle *mpvHandle;
        mpv_render_context *mpvGL;

        ofMutex wakeupCallbackMutex, renderCallbackMutex;
        bool mpvEventReady, renderEventReady;

        int windowWidth, windowHeight;

        static void* getProcAddress(void* data, const char* name);

        unsigned int glFbo, glTexture;
        ofFbo fbo;
};

//************************************************************************************
//************************************************************************************
