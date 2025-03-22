#include "UI.h"

Visualizer::Visualizer() {

}

void Visualizer::Process() {
    InitWindow(1600, 900, "Data Structure Visualizer");

    menu.Init();
    SettingScr.Init();  // Khởi tạo màn hình Settings
    HelperScr.Init(); // Khởi tạo màn hình Helper
    AboutScr.Init(); // Khởi tạo màn hình About
    LinkedListScr.Init(); // Khởi tạo màn hình Linked List
    audioSys.Init();  //  Khởi tạo âm thanh
    hashtableScr.Init();

    int currentState = Menu_state;
    while (!WindowShouldClose()) {
        audioSys.Update();  // Cập nhật âm thanh nền

        // Định nghĩa biến màu nền
        Color bgColor = lightmode;  // Sử dụng màu từ Constants.h

        if (currentState == Menu_state) {
            int nextState = menu.handleEvent();
            if (nextState != Menu_state) {
                currentState = nextState;
            }
        }
        else if (currentState == Settings_state) {
            SettingScr.Update(currentState); // Cập nhật trạng thái của màn hình Settings
        }
        else if (currentState == HowToUse_state) {
            HelperScr.Update(currentState); // Cập nhật màn hình "How To Use"
        }
        else if (currentState == About_state) {
            AboutScr.Update(currentState); // Cập nhật màn hình About 
        }
        else if (currentState == LinkedList_state) {
            LinkedListScr.Update(currentState);
        }
        else if (currentState == HashTable_state) {
            hashtableScr.Update(currentState);
        }

        // Vẽ màn hình phù hợp
        BeginDrawing();
        ClearBackground(bgColor);

        if (currentState == Menu_state) {
            menu.Draw();
        }
        else if (currentState == Settings_state) {
            SettingScr.Draw(); // Vẽ màn hình Settings
        }
        else if (currentState == HowToUse_state) {
            HelperScr.Draw(); // Vẽ màn hình "How To Use"
        }
        else if (currentState == About_state) {
            AboutScr.Draw();
        }
        else if (currentState == LinkedList_state) {
            LinkedListScr.Draw();
        }
        else if (currentState == HashTable_state) {
            hashtableScr.Draw();
        }

        EndDrawing();
    }
    SettingScr.Unload();  // Giải phóng tài nguyên của màn hình Settings
    AboutScr.Unload();    // Giải phóng tài nguyên của màn hình About
    HelperScr.Unload();    // Giải phóng tài nguyên của màn hình Helper
    LinkedListScr.Unload();  // Giải phóng tài nguyên của màn hình Linked List
    audioSys.Unload();    // Giải phóng âm thanh 
    hashtableScr.Unload();  // Giải phóng tài nguyên của màn hình Hash Table 
    CloseWindow();
}