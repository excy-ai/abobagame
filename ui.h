#ifndef ABOBAVULKAN_UI_H
#define ABOBAVULKAN_UI_H

#include "includes.h"
#include "gfx.h"

char state[10] = {' ',
                  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
char currentPlayer = 'X';
uint8_t stepCount = 0;
char winner = 0;
bool isGame = true;
bool gameEnded = false;
int cellStep = 50;

void cleanup() {
    for (auto &item: state) {
        item = ' ';
    }
    currentPlayer = 'X';
    winner = 0;
    stepCount = 0;
    gameEnded = false;
}

int checkCombination(int first, int second, int third) {
    return !(state[first] == ' ' || state[second] == ' ' || state[third] == ' ')
           && (state[first] == state[second] && state[second] == state[third]);
}

int winCombinations[8][3]{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9},
        {1, 5, 9},
        {3, 5, 7},
};

char checkWin() {
    for (int *combination: winCombinations) {
        if (checkCombination(combination[0], combination[1], combination[2])) {
            return state[combination[0]];
        }
    }
    if (stepCount == 9) {
        return 'D';
    }
    return 0;
}

void togglePlayer() {
    currentPlayer = currentPlayer == 'X' ? 'O' : 'X';
}

void makeMove(uint8_t pos) {
    std::cout << (int) pos << " " << currentPlayer;
    if (state[pos] == ' ') {
        state[pos] = currentPlayer;
        togglePlayer();
    }
    winner = checkWin();
    if (winner) {
        gameEnded = true;
    }
}

void toggleGame() {
    isGame = !isGame;
}

//Why DrawList https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-how-can-i-display-custom-shapes-using-low-level-imdrawlist-api
void drawCross(ImVec2 pivot, float radius, ImU32 color) {
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    ImVec2 p1 = ImVec2(radius, -radius) + pivot;
    ImVec2 p2 = ImVec2(-radius, radius) + pivot;
    draw_list->AddLine(p1, p2, color, 2);

    ImVec2 p3 = ImVec2(radius, radius) + pivot;
    ImVec2 p4 = ImVec2(-radius, -radius) + pivot;
    draw_list->AddLine(p3, p4, color, 2);
}

void drawCircle(ImVec2 pivot, float radius, ImU32 color) {
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    draw_list->AddCircle(pivot, radius, color, 0, 2);
}

void drawCell(ImVec2 pivot, float radius, ImU32 color) {
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    ImVec2 p1 = ImVec2(-radius, radius) + pivot;
    ImVec2 p2 = ImVec2(radius, radius) + pivot;
    ImVec2 p3 = ImVec2(radius, -radius) + pivot;
    ImVec2 p4 = ImVec2(-radius, -radius) + pivot;

    draw_list->AddQuad(p1, p2, p3, p4, color, 2);
}

void renderBoard() {
    ImVec2 p = ImGui::GetCursorScreenPos();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            uint8_t pos = i * 3 + j + 1;
            auto pivot = ImVec2(p.x + 25 + i * cellStep, p.y + 25 + j * cellStep);
            ImGui::SetCursorPos(pivot + ImVec2(-25.f, -25.f));
            drawCell(pivot, 25.f, IM_COL32(255, 0, 0, 255));
            ImGui::PushID(pos);
            if (ImGui::InvisibleButton(std::to_string(pos).c_str(), ImVec2(50.f, 50.f), 0)) {
                if (!gameEnded && state[pos] == ' ') makeMove(pos);
            }
            ImGui::PopID();
            if (state[pos] == ' ') continue;
            if (state[pos] == 'X') {
                drawCross(pivot, 23.f, IM_COL32(255, 0, 0, 255));
            } else {
                drawCircle(pivot, 23.0f, IM_COL32(255, 0, 0, 255));
            }
        }
    }
    //reset pos of cursor
    ImGui::SetCursorPos(p);
    //Fill window space with empty dummy data
    ImGui::Dummy(ImVec2(155, 155));
}

void drawTable() {
    static ImGuiTableFlags flags = ImGuiTableFlags_Borders;
    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;

    if (ImGui::BeginTable("table", 3, flags, ImVec2(TEXT_BASE_WIDTH * 10, 0.0f))) {
        for (int row = 0; row < 3; row++) {
            ImGui::TableNextRow();
            for (int column = 0; column < 3; column++) {
                ImGui::TableSetColumnIndex(column);
                uint8_t pos = row * 3 + column + 1;
                char buf[2] = {state[pos], '\0'};
                ImGui::PushID(pos);
                if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f))) {
                    makeMove(pos);
                }
                ImGui::PopID();
            }
        }
        ImGui::EndTable();
    }
}

void renderFrame() {
    //Init main imGUI window
    {
        //Set window position
        ImVec2 pos = ImGui::GetMainViewport()->Pos;
        ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.f, 0.f));

        //Set window size
        ImGui::SetNextWindowSize(ImVec2(gui_wd.w, gui_wd.h), ImGuiCond_Always);
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoMove;
        ImGui::Begin("Main", nullptr, window_flags);
    }

    ImGui::Checkbox("Demo Window", &show_demo_window);
    {
        //Play button
        ImGui::PushID(0);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
        if (ImGui::Button("Play?")) toggleGame();
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }

    if (isGame) {
        renderBoard();
        if (winner) {
            ImGui::Text("Winner %c!", winner);
        }
        if (gameEnded) {
            ImGui::PushID(0);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
            if (ImGui::Button("Play again?")) cleanup();
            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }
    }

    ImGui::Text("Application average %.1f FPS", io->Framerate);
    ImGui::End();
}

void renderUI() {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();

    // Resize swap chain?
    if (g_SwapChainRebuild) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        if (width > 0 && height > 0) {
            ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
            ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, &g_MainWindowData,
                                                   g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
            g_MainWindowData.FrameIndex = 0;
            g_SwapChainRebuild = false;
        }
    }

    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        renderFrame();
    }

    // Rendering
    ImGui::Render();
    ImDrawData *draw_data = ImGui::GetDrawData();
    const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
    if (!is_minimized) {
        wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
        wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
        wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
        wd->ClearValue.color.float32[3] = clear_color.w;
        FrameRender(wd, draw_data);
        FramePresent(wd);
    }
}

#endif //ABOBAVULKAN_UI_H