// spotifymute.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include <stdafx.h>
#include <iostream>
#include <Objbase.h>
#include <combaseapi.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <audiopolicy.h>
#include <winuser.h>
#include <Windows.h>
#include <psapi.h>
#include <tchar.h>
#include <stdio.h>


bool iscurrentad = false;

ISimpleAudioVolume * Init() {
    int counter = 0;
    while (true) {


        const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
        const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
        const IID IID_IAudioSessionManager2 = __uuidof(IAudioSessionManager2);

        CoInitializeEx(NULL, COINIT_MULTITHREADED);


        IMMDeviceEnumerator* Enumerator;
        CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&Enumerator);

        IMMDevice* Device;
        Enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &Device);

        IAudioSessionManager2* SessionManager;
        Device->Activate(IID_IAudioSessionManager2, CLSCTX_ALL, NULL, (void**)&SessionManager);

        IAudioSessionEnumerator* SessionEnum;
        SessionManager->GetSessionEnumerator(&SessionEnum);

        IAudioSessionControl* SessionControl;
        SessionEnum->GetSession(counter, &SessionControl);


        DWORD PID[8];
        IAudioSessionControl2* SessionControl2;
        SessionControl->QueryInterface(_uuidof(IAudioSessionControl2), (void**)&SessionControl2);
        SessionControl2->GetProcessId(PID);

        HANDLE ProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, *PID);
        TCHAR FileName[MAX_PATH];
        GetModuleFileNameEx(ProcessHandle, 0, FileName, MAX_PATH);

        if (!(_tcscmp(FileName, _T("C:\\Program Files\\WindowsApps\\SpotifyAB.SpotifyMusic_1.191.824.0_x86__zpdnekdrzrea0\\Spotify.exe")))) {
        std::cout << "success\n";

        std::wcout << FileName;
        std::cout << "\n";
        std::cout << *PID;
        std::cout << "\n";
        std::cout << counter;
        std::cout << "\n";

        ISimpleAudioVolume* VolumeController;
        //SessionManager->GetSimpleAudioVolume(NULL, true, &VolumeController);
        SessionControl->QueryInterface(&VolumeController);

        //VolumeController->SetMute(IsMuted, NULL);

        return VolumeController;
        };
        counter++;
    }
}

BOOL CALLBACK CallbackFunction(_In_ HWND hwnd, _In_ LPARAM lParam) {
    char title[14];
    GetWindowTextA(hwnd, title, 14);
    if (strcmp(title, "Spotify Free") == 0 || strcmp(title, "Advertisement") == 0 || strcmp(title, "Spotify") == 0) {
        iscurrentad = true;
        return false;
    }

    return true;
}

//int main() { Mute(true); return 0; }
int main()
{
    std::cout << "SpotifyMute has started\n";
//    std::cout << "Session number: ";
//    int num;
//    std::cin >> num;


    ISimpleAudioVolume* VolumeController;
    VolumeController = Init();
    
    
    //int counter = 1;
    char param[1];
    VolumeController->SetMute(false, NULL);
    while (true) {
        //std::cout << counter;
        //counter ++;
        EnumWindows(CallbackFunction, *param);
        if (iscurrentad) {
            VolumeController->SetMute(true, NULL);
            while (true) {
                iscurrentad = false;
                EnumWindows(CallbackFunction, *param);
                if (!iscurrentad) {Sleep(1000); VolumeController->SetMute(false, NULL); break; }
                Sleep(100);
            }
        }
        Sleep(100);
    }
    
    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
