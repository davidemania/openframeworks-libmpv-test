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
//  main.cpp -> application entry point and startup                                 //
//                                                                                  //
//**********************************************************************************//

#include "ofApp.h"

//************************************************************************************

int main()
{
    // const bool fullScreen = false;
    const bool fullScreen = true;
    const bool decorated = true;

    ofGLFWWindowSettings settings;
    settings.windowMode = fullScreen ? OF_GAME_MODE : OF_WINDOW;

    string resolutionCombo = ofSystem("xdpyinfo | grep -oP 'dimensions:\\s+\\K\\S+'");
    resolutionCombo = ofTrim(resolutionCombo); //.find_last_not_of(" \n\r\t")+1);

    int resolutionX = ofToInt(resolutionCombo.substr(0, resolutionCombo.find_first_of('x')));
    int resolutionY = ofToInt(resolutionCombo.substr(resolutionCombo.find_first_of('x')+1));

    ofLogNotice() << "Screen resolution is " << resolutionX << " x " << resolutionY << " (as reported by OS)";

    // @TODO: debug only
    resolutionX = 1920;
    resolutionY = 1080;

    settings.setSize(resolutionX, resolutionY);
    settings.decorated = decorated;

	ofCreateWindow(settings);

	ofRunApp(new ofApp(resolutionX, resolutionY));
}

//************************************************************************************
//************************************************************************************
