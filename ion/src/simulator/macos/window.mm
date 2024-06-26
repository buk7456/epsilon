#define SDL_VIDEO_OPENGL_EGL 0
#include "../external/sdl/src/video/SDL_sysvideo.h"
#include "../external/sdl/src/video/cocoa/SDL_cocoawindow.h"
#include "../shared/window.h"
#import <Cocoa/Cocoa.h>

namespace Ion {
namespace Simulator {
namespace Window {

void didInit(SDL_Window * window) {
  NSWindow * nswindow = reinterpret_cast<SDL_WindowData *>(window->driverdata)->nswindow;
  [nswindow setTitlebarAppearsTransparent:true];
  [nswindow setStyleMask:(NSWindowStyleMaskTitled|NSWindowStyleMaskClosable|NSWindowStyleMaskMiniaturizable|NSWindowStyleMaskFullSizeContentView|NSWindowStyleMaskResizable)];
  [nswindow setTitle:@""];

  // Constraint aspect ratio
  [nswindow setContentAspectRatio:NSMakeSize(perfectWidth, perfectHeight)];

  // Change the size to apply the aspect ratio without waiting for a resize
  [nswindow setContentSize:NSMakeSize(perfectWidth, perfectHeight)];

  // Save position of the window using the OS-provided routine
  [nswindow setFrameAutosaveName:@"Calculator"];

  // TODO: This should be activated but it triggers assertions in SDL
  // [nswindow setMovableByWindowBackground:true];
}

void willShutdown(SDL_Window *) {
}

}
}
}
