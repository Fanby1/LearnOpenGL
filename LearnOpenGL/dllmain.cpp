// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include <Windows.h>
#include <GLFW/glfw3.h>
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        //if (!glfwInit()) {
        //    return FALSE;
        //}
        //break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

