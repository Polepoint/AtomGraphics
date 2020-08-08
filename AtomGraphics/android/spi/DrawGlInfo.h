//
// Created by neo on 2019-08-16.
//

#ifndef ATOMGRAPHICS_DRAWGLINFO_H
#define ATOMGRAPHICS_DRAWGLINFO_H


namespace AtomGraphics {

struct DrawGlInfo {
    // Input: current clip rect
    int clipLeft;
    int clipTop;
    int clipRight;
    int clipBottom;
    // Input: current width/height of destination surface
    int width;
    int height;
    // Input: is the render target an FBO
    bool isLayer;
    // Input: current transform matrix, in OpenGL format
    float transform[16];
    // Output: dirty region to redraw
    float dirtyLeft;
    float dirtyTop;
    float dirtyRight;
    float dirtyBottom;
    /**
     * Values used as the "what" parameter of the functor.
     */
    enum Mode {
        // Indicates that the functor is called to perform a draw
                kModeDraw,
        // Indicates the the functor is called only to perform
        // processing and that no draw should be attempted
                kModeProcess,
        // Same as kModeProcess, however there is no GL context because it was
        // lost or destroyed
                kModeProcessNoContext,
        // Invoked every time the UI thread pushes over a frame to the render thread
        // *and the owning view has a dirty display list*. This is a signal to sync
        // any data that needs to be shared between the UI thread and the render thread.
        // During this time the UI thread is blocked.
                kModeSync
    };
    /**
     * Values used by OpenGL functors to tell the framework
     * what to do next.
     */
    enum Status {
        // The functor is done
                kStatusDone = 0x0,
        // DisplayList actually issued GL drawing commands.
        // This is used to signal the HardwareRenderer that the
        // buffers should be flipped - otherwise, there were no
        // changes to the buffer, so no need to flip. Some hardware
        // has issues with stale buffer contents when no GL
        // commands are issued.
                kStatusDrew = 0x4
    };
}; // struct DrawGlInfo

}

#endif //ATOMGRAPHICS_DRAWGLINFO_H
