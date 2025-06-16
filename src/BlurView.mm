#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>
#import <QWidget>

// This function adds a macOS blur behind any QWidget
void addMacOSBlurEffect(QWidget* widget, int width, int height) {
    if (!widget) return;

    widget->winId();  // Force native handle creation

    NSView* nativeView = (__bridge NSView*)widget->window()->winId();
    if (!nativeView) return;

    NSVisualEffectView* blurView = [[NSVisualEffectView alloc] initWithFrame:NSMakeRect(0, 0, width, height)];
    blurView.material = NSVisualEffectMaterialSidebar; // You can choose others: Menu, FullScreenUI, etc.
    blurView.blendingMode = NSVisualEffectBlendingModeBehindWindow;
    blurView.state = NSVisualEffectStateActive;

    [nativeView setWantsLayer:YES];
    [nativeView addSubview:blurView positioned:NSWindowBelow relativeTo:nil];
}

/*
void addLiquidGlassEffect(NSView* parentView, NSRect frame) {
    NSGlassEffectView* glassView = [[NSGlassEffectView alloc] initWithFrame:frame];

    // Configure material — Apple provides new materials for Liquid Glass
    glassView.material = NSVisualEffectMaterialSidebar;  // or try .HUDWindow, .FullScreenUI etc.
    glassView.state = NSVisualEffectStateActive;
    glassView.blendingMode = NSVisualEffectBlendingModeBehindWindow;

    [parentView addSubview:glassView positioned:NSWindowBelow relativeTo:nil];
}
*/
