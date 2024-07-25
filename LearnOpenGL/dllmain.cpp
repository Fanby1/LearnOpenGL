//BOOL APIENTRY D11Main(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
//{
//	switch (ul_reason_for_call) {
//		case DLL_PROCESS_ATTACH:
//			if (!glfwInit()) {
//				HIVE_LOG_ERROR("Failed to initialize GLFW");
//				return FALSE;
//			}
//			break;
//		case DLL_PROCESS_DETACH:
//			TerminateGLFW();
//			break;
//		case DLL_THREAD_ATTACH:
//		case DLL_THREAD_DETACH:
//			break;
//	}
//	return TRUE;
//}