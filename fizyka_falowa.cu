#pragma warning(disable:4996)

#include "imgui_i_grafika_setup.h"

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <string>

#include "pomocne_funkcje.h"

fp_t c = FP_JEDEN;

#include "podstawa.h"
#include "prezentacja.h"

int main(int argc, char** argv)
{

    int dev = findCudaDevice(argc, (const char**)argv);
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, dev);

    static_assert(sizeof(void*) == 8, "Powinno byc 8 bytow w pointerze");

    GLFWwindow* window; ImGuiIO* io = new ImGuiIO;
    int ret = imgui_i_grafika_setup(window, io, "Szybkie spacerki losowe");
    if (ret != 0) return ret;

    pokaz_interaktywny pokaz;

    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        pokaz.pokaz_okno();

        if (show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
            ImPlot::ShowDemoWindow(&show_demo_window);
            ImPlot3D::ShowDemoWindow(&show_demo_window);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // czesciowo z grafiki

        ImGui::Render();
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot3D::DestroyContext();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    delete io;

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


