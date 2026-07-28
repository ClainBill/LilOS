#include "app_manager.h"
// #include <vector>
// #include "hal/hal.h"
#include "app.h"


AppManager::~AppManager(){
    for (auto app : apps){
        delete app;
    }
}

void AppManager::openApp(App* app, bool grabFocus){
    apps.push_back(app);
    if (grabFocus) setFocus(app);
}

void AppManager::setFocus(App* focusApp){
    focusedApp = focusApp;
}

void AppManager::handleInput(hal::KeyEvent event) {
    // Only the focused app sees input
    if (focusedApp) focusedApp->handleInput(event);
}

void AppManager::update(float deltaTime) {
    // All apps update (they might not all be visible, but they run)
    for (auto app : apps) {
        app->update(deltaTime);
    }
}

void AppManager::render() {
    // Render all apps (back to front for layering)
    for (auto app : apps) {
        app->render();
    }
}