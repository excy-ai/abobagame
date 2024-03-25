#ifndef ABOBAVULKAN_LOOP_H
#define ABOBAVULKAN_LOOP_H

#include "includes.h"
#include "ui.h"

void gameLoop() {
    while (!glfwWindowShouldClose(window)) {
        renderUI();
    }
}

#endif //ABOBAVULKAN_LOOP_H