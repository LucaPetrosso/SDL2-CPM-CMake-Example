#define SDL_MAIN_HANDLED true
#include <iostream>
#include <algorithm>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
//#include <SDL_ttf.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
EM_JS(int, canvas_get_width, (), {
return canvas.width;
});

EM_JS(int, canvas_get_height, (), {
return canvas.height;
});
#endif

std::string getButtonLabel(int panState) {
    switch (panState) {
        case 0:
            return "Center Pan";
        case 1:
            return "Full Left Pan";
        case 2:
            return "Full Right Pan";
        default:
            return "Unknown Pan State";
    }
}

int main(int argc, char* argv[]) {
    // Unused argc, argv
    (void) argc;
    (void) argv;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        std::cerr << "SLD_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    const char* tracka = "/Users/lucapetrosso/Repos/SDL2-CPM-CMake-Example/assets/SDLTrackA.wav";
    Mix_Chunk* audioa = Mix_LoadWAV(tracka);
    if(!audioa) {
        std::cerr << "Failed to load TrackA! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }
    const char* trackb = "/Users/lucapetrosso/Repos/SDL2-CPM-CMake-Example/assets/SDLTrackB.wav";
    Mix_Chunk* audiob = Mix_LoadWAV(trackb);
    if(!audiob) {
        std::cerr << "Failed to load TrackB! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }
    const char* trackc = "/Users/lucapetrosso/Repos/SDL2-CPM-CMake-Example/assets/SDLTrackC.wav";
    Mix_Chunk* audioc = Mix_LoadWAV(trackc);
    if(!audioc) {
        std::cerr << "Failed to load TrackC! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }
    const char* trackd = "/Users/lucapetrosso/Repos/SDL2-CPM-CMake-Example/assets/SDLTrackD.wav";
    Mix_Chunk* audiod = Mix_LoadWAV(trackd);
    if(!audiod) {
        std::cerr << "Failed to load TrackD! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    const char* tracke = "/Users/lucapetrosso/Repos/SDL2-CPM-CMake-Example/assets/SDLTrackE.wav";
    Mix_Chunk* audioe = Mix_LoadWAV(tracke);
    if(!audioe) {
        std::cerr << "Failed to load TrackE! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    if (Mix_PlayChannel(0, audioa, -1) == -1){
        std::cerr << "Failed to play audioa! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    if (Mix_PlayChannel(1, audiob, -1) == -1){
        std::cerr << "Failed to play audiob! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    if (Mix_PlayChannel(2, audioc, -1) == -1){
        std::cerr << "Failed to play audioc! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    if (Mix_PlayChannel(3, audiod, -1) == -1){
        std::cerr << "Failed to play audiod! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    if (Mix_PlayChannel(4, audioe, -1) == -1){
        std::cerr << "Failed to play audioe! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }
    /*while (Mix_Playing(-1) != 0){
        SDL_Delay(100);
    }*/

    auto width = 1280;
    auto height = 720;
#ifdef __EMSCRIPTEN__
    width = canvas_get_width();
    height = canvas_get_height();
#endif

    // Setup window
    SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    SDL_Window *window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, width, height, window_flags);

    if (!window) {
        std::cout << "Window could not be created!" << std::endl
                  << "SDL_Error: " << SDL_GetError() << std::endl;
        abort();
    }

    // Setup SDL_Renderer instance
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_Log("Error creating SDL_Renderer!");
        abort();
    }
    //SDL_RendererInfo info;
    //SDL_GetRendererInfo(renderer, &info);
    //SDL_Log("Current SDL_Renderer: %s", info.name);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    bool isAudioPlaying = false;
    int panState = 0;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;

    // Declare rect of square
    SDL_Rect squareRect;

    // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
    squareRect.w = std::min(width, height) / 2;
    squareRect.h = std::min(width, height) / 2;

    // Square position: In the middle of the screen
    squareRect.x = width / 2 - squareRect.w / 2;
    squareRect.y = height / 2 - squareRect.h / 2;

    // Event loop
    while (!done) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();


        static int channela_value = 0;
        static int channelb_value = 0;
        static int channelc_value = 0;
        static int channeld_value = 0;
        static int channele_value = 0;

        if (isAudioPlaying) {
            if (ImGui::Button("Pause Audio")) {
                Mix_Pause(-1); // Pause all channels
                isAudioPlaying = false;
            }
        } else {
            if (ImGui::Button("Play Audio")) {
                Mix_Resume(-1); // Resume all paused channels
                isAudioPlaying = true;
            }
        }
        //initializes all sliders
        ImGui::VSliderInt("##intv1", ImVec2(18, 160), &channela_value, 0, 5);
        Mix_Volume(0, channela_value * MIX_MAX_VOLUME / 5);
        ImGui::SameLine();
        ImGui::VSliderInt("##intv2", ImVec2(18, 160), &channelb_value, 0, 5);
        Mix_Volume(1, channelb_value * MIX_MAX_VOLUME / 5);
        ImGui::SameLine();
        ImGui::VSliderInt("##intv3", ImVec2(18, 160), &channelc_value, 0, 5);
        Mix_Volume(2, channelc_value * MIX_MAX_VOLUME / 5);
        ImGui::SameLine();
        ImGui::VSliderInt("##intv4", ImVec2(18, 160), &channeld_value, 0, 5);
        Mix_Volume(3, channeld_value * MIX_MAX_VOLUME / 5);
        ImGui::SameLine();
        ImGui::VSliderInt("##intv5", ImVec2(18, 160), &channele_value, 0, 5);
        Mix_Volume(4, channele_value * MIX_MAX_VOLUME / 5);

        if (ImGui::Button(getButtonLabel(panState).c_str())) {
            // Button was clicked, change the pan state
            panState = (panState + 1) % 3;

            // Apply audio panning based on panState (use SDL_Mixer API to set panning)
            switch (panState) {
                case 0:
                    // Set audio to center pan
                    // SDL_Mixer API usage: Mix_SetPanning(channel, left, right);
                    Mix_SetPanning(-1, 255, 255);
                    break;
                case 1:
                    // Set audio to full left pan
                    Mix_SetPanning(-1, 255, 0);
                    break;
                case 2:
                    // Set audio to full right pan
                    Mix_SetPanning(-1, 0, 255);
                    break;
            }
        }
        //raises and lowers all tracks
        if (ImGui::Button("Raise All")){
            channela_value++;
            channelb_value++;
            channelc_value++;
            channeld_value++;
            channele_value++;
        }
        ImGui::SameLine();
        if (ImGui::Button("Lower All")){
            channela_value--;
            channelb_value--;
            channelc_value--;
            channeld_value--;
            channele_value--;
        } // Keeps the values from going past set limit
        if (channela_value >= 6)
            channela_value--;
        if (channela_value <= -1)
            channela_value++;
        if (channelb_value >= 6)
            channelb_value--;
        if (channelb_value <= -1)
            channelb_value++;
        if (channelc_value >= 6)
            channelc_value--;
        if (channelc_value <= -1)
            channelc_value++;
        if (channeld_value >= 6)
            channeld_value--;
        if (channeld_value <= -1)
            channeld_value++;
        if (channele_value >= 6)
            channele_value--;
        if (channele_value <= -1)
            channele_value++;


        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }


        // Rendering
        ImGui::Render();

        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);

        // todo: add your game logic here to be drawn before the ui rendering
        // Set renderer color red to draw the square
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        // Draw filled square
        SDL_RenderFillRect(renderer, &squareRect);

        // present ui on top of your drawings
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);

        SDL_Delay(0);
    }

    // Cleanup
    Mix_FreeChunk(audioa);
    Mix_CloseAudio();
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}