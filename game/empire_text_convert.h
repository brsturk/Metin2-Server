#ifndef __INC_EMPIRE_CHAT_CONVERT
#define __INC_EMPIRE_CHAT_CONVERT

bool LoadEmpireTextConvertTable(DWORD dwEmpireID, const char* c_szFileName);
void ConvertEmpireText(DWORD dwEmpireID, char* szText, size_t len, int iPct);

#endif
//martysama0134's cc449580f8a0ea79d66107125c7ee3d3
