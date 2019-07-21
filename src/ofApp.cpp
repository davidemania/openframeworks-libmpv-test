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
//  ofApp.cpp -> main application class implementation                              //
//                                                                                  //
//**********************************************************************************//

#include "ofApp.h"

#include "GLFW/glfw3.h"

ofApp::ofApp(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

//************************************************************************************
//************************************************************************************

void ofApp::setup()
{
    ofSetFrameRate(30);

    mpvEventReady = renderEventReady = false;

    fbo.allocate(windowWidth, windowHeight, GL_RGB);

    // fbo.begin();
    // ofBackground(255, 0, 0);
    // fbo.end();

    mpvHandle = mpv_create();

    if (!mpvHandle)
    {
        ofLogError() << "failed creating mpvHandle";
        exit();
    }

    int result;
    if((result = mpv_set_property_string(mpvHandle, "hwdec", "vaapi-copy")) < 0)
    {
        printf("failed setting property 1");
        exit();
    }
    if((result = mpv_set_property_string(mpvHandle, "vo", "libmpv")) < 0)
    {
        printf("failed setting property 2");
        exit();
    }
    // if((result = mpv_set_property_string(mpvHandle, "gpu-context", "egl")) < 0)
    // {
    //     printf("failed setting property 2");
    //     exit();
    // }

    if (mpv_initialize(mpvHandle) < 0)
    {
        printf("failed initializing mpvHandle");
        exit();
    }

    mpv_opengl_init_params glParams =
    {
        .get_proc_address = getProcAddress
    };

    Display* display = ofGetX11Display();
    
    int temp = 0;
    mpv_render_param params[] = {
        {
            MPV_RENDER_PARAM_API_TYPE,
            (void*)MPV_RENDER_API_TYPE_OPENGL
        },
        {
            MPV_RENDER_PARAM_OPENGL_INIT_PARAMS,
            &glParams,
        },
        {
            MPV_RENDER_PARAM_X11_DISPLAY,
            display,
        },
        {
            MPV_RENDER_PARAM_INVALID // end of list
        }
    };

    if (mpv_render_context_create(&mpvGL, mpvHandle, params) < 0)
    {
        printf("failed initializing mvp GL contest");
        exit();
    }

    // When normal mpv events are available.
    mpv_set_wakeup_callback(mpvHandle, ofApp::wakeupCallback, this);

    // When a new frame should be drawn with mpv_opengl_cb_draw().
    // (Separate from the normal event handling mechanism for the sake of
    //  users which run OpenGL on a different thread.)
    mpv_render_context_set_update_callback(mpvGL, ofApp::renderCallback, this);

    mpv_request_log_messages(mpvHandle, "debug"); //"trace");

    // Play this file. Note that this starts playback asynchronously.
    const char *cmd[] =
    {
        "loadfile",
        "/home/dodo/of_v0.10.1_linux64gcc6_release/apps/myApps/mpvTest/data/test.mov",
        NULL
    };

    mpv_command(mpvHandle, cmd);

}

//************************************************************************************
//************************************************************************************

void ofApp::wakeupCallback(void* data)
{
    ((ofApp*)data)->wakeupCallbackReceived();
}

//************************************************************************************
//************************************************************************************

void ofApp::wakeupCallbackReceived()
{
    wakeupCallbackMutex.lock();
    mpvEventReady = true;
    wakeupCallbackMutex.unlock();
}

//************************************************************************************
//************************************************************************************

void ofApp::renderCallback(void* data)
{
    ((ofApp*)data)->renderCallbackReceived();
}

//************************************************************************************
//************************************************************************************

void ofApp::renderCallbackReceived()
{
    renderCallbackMutex.lock();
    mpvEventReady = true;
    renderCallbackMutex.unlock();
}

//************************************************************************************
//************************************************************************************

void* ofApp::getProcAddress(void* data, const char* name)
{
    return (void*)glfwGetProcAddress(name);
}

//************************************************************************************
//************************************************************************************

void ofApp::update()
{
    bool wakeupEvent;
    wakeupCallbackMutex.lock();
    wakeupEvent = mpvEventReady;
    wakeupCallbackMutex.unlock();

    if(wakeupEvent)
    {
        mpv_event* mpvEvent = mpv_wait_event(mpvHandle, 0);
        if (mpvEvent->event_id == MPV_EVENT_LOG_MESSAGE)
        {
            mpv_event_log_message* message = (mpv_event_log_message*)(mpvEvent->data);
            string text = ofTrim(string(message->text));

            ofLogNotice() << "MPV log message: " << text;
        }
        else if (mpvEvent->event_id != MPV_EVENT_NONE)
        {
            ofLogNotice() <<"MPV event: " << mpv_event_name(mpvEvent->event_id);
        }

        wakeupEvent = false;
    } 
}

//************************************************************************************
//************************************************************************************

void ofApp::draw()
{
    bool renderEvent;
    renderCallbackMutex.lock();
    renderEvent = mpvEventReady;
    renderCallbackMutex.unlock();
    
    if(renderEvent)
    {
        int temp = 0;
        mpv_opengl_fbo fboParams =
        {
            .fbo = fbo.getId(),
            .w = windowWidth,
            .h = windowHeight,
            .internal_format = GL_RGB
        };

        mpv_render_param params[] =
        {
            // Specify the default framebuffer (0) as target. This will
            // render onto the entire screen. If you want to show the video
            // in a smaller rectangle or apply fancy transformations, you'll
            // need to render into a separate FBO and draw it manually.
            {
                MPV_RENDER_PARAM_OPENGL_FBO, &fboParams
            },
            // Flip rendering (needed due to flipped GL coordinate system).
            {
                MPV_RENDER_PARAM_FLIP_Y,
                &temp
            },
            {
                MPV_RENDER_PARAM_INVALID // end of list
            }
        };
        // See render_gl.h on what OpenGL environment mpv expects, and
        // other API details.
        // fbo.bind();
        // glBindFramebuffer(GL_FRAMEBUFFER, glFbo);
        fbo.begin();
        mpv_render_context_render(mpvGL, params);
        fbo.end();
        // fbo.unbind();
        fbo.draw(0, 0, windowWidth, windowHeight);
        string fpsMessage = "FPS: " + ofToString(ofGetFrameRate());
        ofDrawBitmapStringHighlight(fpsMessage, 200, 200);
        renderEvent = false;
    }   
}

//************************************************************************************
//************************************************************************************

void ofApp::keyPressed(int key)
{

}

//************************************************************************************
//************************************************************************************

void ofApp::exit()
{
    ofLog() << "Exiting application";
    // Destroy the GL renderer and all of the GL objects it allocated. If video
    // is still running, the video track will be deselected.
    mpv_render_context_free(mpvGL);

    mpv_terminate_destroy(mpvHandle);
}

//************************************************************************************
//************************************************************************************
