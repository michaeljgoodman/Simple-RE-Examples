#include <iostream>
#include <windows.h>
#include "Processthreadsapi.h"
#include "Libloaderapi.h"
#include <winnt.h>
#include <winternl.h>
#include <Lmcons.h>
#include "Processthreadsapi.h"
#include "Libloaderapi.h"
#include <tlhelp32.h>

//#define ADDR unsigned __int64
//don't need this anymore, using std::uintptr_t to switch between 32 and 64 bit


HRESULT UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA);

// Dynamically finds the base address of a DLL in memory
std::uintptr_t find_dll_base(const char* dll_name)
{

    PTEB teb = reinterpret_cast<PTEB>(__readgsqword(reinterpret_cast<DWORD_PTR>(&static_cast<NT_TIB*>(nullptr)->Self)));
    PPEB_LDR_DATA loader = teb->ProcessEnvironmentBlock->Ldr;

    PLIST_ENTRY head = &loader->InMemoryOrderModuleList;
    PLIST_ENTRY curr = head->Flink;

    // Iterate through every loaded DLL in the current process
    do {
        PLDR_DATA_TABLE_ENTRY dllEntry = CONTAINING_RECORD(curr, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
        char* dllName;
        // Convert unicode buffer into char buffer for the time of the comparison, then free it
        UnicodeToAnsi(dllEntry->FullDllName.Buffer, &dllName);
        char* result = strstr(dllName, dll_name);
        CoTaskMemFree(dllName); // Free buffer allocated by UnicodeToAnsi

        if (result != NULL) {
            // Found the DLL entry in the PEB, return its base address
            return (std::uintptr_t)dllEntry->DllBase;
        }
        curr = curr->Flink;
    } while (curr != head);

    return NULL;
}


HRESULT UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA)
{
    ULONG cbAnsi, cCharacters;
    DWORD dwError;
    if (pszW == NULL) {
        *ppszA = NULL;
        return NOERROR;
    }
    cCharacters = wcslen(pszW) + 1;
    cbAnsi = cCharacters * 2;

    *ppszA = (LPSTR)CoTaskMemAlloc(cbAnsi);
    if (NULL == *ppszA)
        return E_OUTOFMEMORY;

    if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, *ppszA, cbAnsi, NULL, NULL)) {
        dwError = GetLastError();
        CoTaskMemFree(*ppszA);
        *ppszA = NULL;
        return HRESULT_FROM_WIN32(dwError);
    }
    return NOERROR;
}

std::uintptr_t find_dll_export(std::uintptr_t dll_base, const char* export_name)
{
    PIMAGE_DOS_HEADER peHeader = (PIMAGE_DOS_HEADER)dll_base;

    PIMAGE_NT_HEADERS peNtHeaders = (PIMAGE_NT_HEADERS)(dll_base + peHeader->e_lfanew);

    DWORD exportDescriptorOffset = peNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

    PIMAGE_EXPORT_DIRECTORY exportTable = (PIMAGE_EXPORT_DIRECTORY)(dll_base + exportDescriptorOffset);

    DWORD* name_table = (DWORD*)(dll_base + exportTable->AddressOfNames);
    WORD* ordinal_table = (WORD*)(dll_base + exportTable->AddressOfNameOrdinals);
    DWORD* func_table = (DWORD*)(dll_base + exportTable->AddressOfFunctions);

    for (int i = 0; i < exportTable->NumberOfNames; ++i) {
        char* funcName = (char*)(dll_base + name_table[i]);
        std::uintptr_t func_ptr = dll_base + func_table[ordinal_table[i]];
        if (!_strcmpi(funcName, export_name)) {
            return func_ptr;
        }
    }
    return NULL;
}

int main() {
    std::uintptr_t kernel32_ptr = find_dll_base("KERNEL32");
    //std::cout << kernel32_ptr;
    std::uintptr_t createfilea_addr = find_dll_export(kernel32_ptr, "createfilea");
    std::uintptr_t readfile_addr = find_dll_export(kernel32_ptr, "readfile");

    auto dynamic_createfilea = reinterpret_cast<HANDLE(*)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE)>(createfilea_addr);
    auto dynamic_readfile = reinterpret_cast<int(*)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED)>(readfile_addr);

    HANDLE hFile;
    BOOL bErrorFlag = FALSE;



    HANDLE result = dynamic_createfilea("C:\\Users\\CF-WS-08\\Documents\\test.txt", GENERIC_READ, 0x00000001, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    char buffer[6] = { 0 };
    

    LPDWORD bytes_read = 0;
    LPOVERLAPPED overlapped = NULL;

    int read = dynamic_readfile(result,buffer,5,bytes_read,overlapped);
    
    buffer[5] = NULL;

    if (read) {
        printf(buffer, 4);
    }
    

    return 0;
}