#pragma once
#include <Windows.h>
#include <iostream>

template <typename T = NTSTATUS>
class makesyscall
{
public:
	makesyscall(byte sysindex1, byte sysindex2, byte sysindex3, byte sysindex4, LPCSTR lpFuncName, DWORD offsetToFunc,
		byte retCode, byte ret1, byte ret2);
	~makesyscall();
public:
	template <typename... Args>
	T operator()(Args ... arg);
private:
	uint32_t GetSyscallIndex(const byte* pFuncAddr);
	void CreateShellSysCall(byte sysindex1, byte sysindex2, byte sysindex3, byte sysindex4, LPCSTR lpFuncName,
		DWORD offsetToFunc, byte retCode, byte ret1, byte ret2);
private:
	char* m_pShellCode;
};

template <typename T>
template <typename... Args>
T makesyscall<T>::operator()(Args ... arg)
{
	if (!m_pShellCode)
		return T(0);

	typedef T(__stdcall* mFunc)(Args ...);
	mFunc pFunc = mFunc(m_pShellCode);

	return pFunc(arg...);
}


template <typename T>
makesyscall<T>::makesyscall(byte sysindex1, byte sysindex2, byte sysindex3, byte sysindex4, LPCSTR lpFuncName,
	DWORD offsetToFunc, byte retCode, byte ret1, byte ret2)
{
	CreateShellSysCall(sysindex1, sysindex2, sysindex3, sysindex4, lpFuncName, offsetToFunc, retCode, ret1, ret2);
}

template <typename T>
makesyscall<T>::~makesyscall()
{
	if (m_pShellCode)
		VirtualFree(m_pShellCode, NULL, MEM_RELEASE);
}

template <typename T>
uint32_t makesyscall<T>::GetSyscallIndex(const byte* pFuncAddr)
{
	if (!pFuncAddr)
		return 0;

	for (int8_t i = 0; i < 0xF; ++i)
	{
		if (*(pFuncAddr + i) == 0xB8)
		{
			return *PUINT(pFuncAddr + i + 1);
		}
	}

	return 0;
}

template <typename T>
void makesyscall<T>::CreateShellSysCall(byte sysindex1, byte sysindex2, byte sysindex3, byte sysindex4,
	LPCSTR lpFuncName, DWORD offsetToFunc, byte retCode, byte ret1, byte ret2)
{
	if (!sysindex1 && !sysindex2 && !sysindex3 && !sysindex4)
		return;

	byte ShellCode[]
	{
		0xB8, 0x00, 0x00, 0x00, 0x00, //mov eax, SysCallIndex
		0xBA, 0x00, 0x00, 0x00, 0x00, //mov edx, [function]
		0xFF, 0xD2, //call edx
		0xC2, 0x00, 0x00 //ret
	};

	m_pShellCode = static_cast<char*>(VirtualAlloc(nullptr, sizeof(ShellCode), MEM_RESERVE | MEM_COMMIT,
		PAGE_EXECUTE_READWRITE));

	if (!m_pShellCode)
		return;

	memcpy(m_pShellCode, ShellCode, sizeof(ShellCode));

	*(uintptr_t*)(m_pShellCode + 6) = static_cast<uintptr_t>((DWORD)GetProcAddress(
		GetModuleHandleA("ntdll.dll"), lpFuncName) + offsetToFunc);

	*(byte*)(m_pShellCode + 1) = sysindex1;
	*(byte*)(m_pShellCode + 2) = sysindex2;
	*(byte*)(m_pShellCode + 3) = sysindex3;
	*(byte*)(m_pShellCode + 4) = sysindex4;

	*(byte*)(m_pShellCode + 12) = retCode;
	*(byte*)(m_pShellCode + 13) = ret1;
	*(byte*)(m_pShellCode + 14) = ret2;
}

template<typename Ret = NTSTATUS>
class syscall_t
{
public:
	explicit syscall_t(const char* func_name) :
		func_addr_((uint8_t*)GetProcAddress(GetModuleHandleA("ntdll.dll"), func_name)),
		shell_code_(nullptr)
	{
		create_shell_code();
	}

	~syscall_t()
	{
		if (shell_code_)
			VirtualFree(shell_code_, 0, MEM_RELEASE);
	}

	template<typename ...Args>
	Ret operator()(Args... args)
	{
		if (!shell_code_)
			return Ret();

		typedef Ret(__stdcall* func_t)(Args...);
		func_t func = (func_t)shell_code_;

		return func(args...);
	}
private:
	void create_shell_code()
	{
		uint32_t sys_index = 0;
		if (!func_addr_)
			return;

		for (int8_t i = 0; i < 0xF; ++i)
		{
			if (*(func_addr_ + i) != 0xB8)
				continue;
			sys_index = *(uint32_t*)(func_addr_ + i + 1);
		}

		if (!sys_index)
			return;

		uint8_t shell_code[]
		{
			0x90, 0x90, 0x90,
			0xB8, 0x00, 0x00, 0x00, 0x00,
			0xBA, 0x00, 0x00, 0x00, 0x00,
			0xFF, 0xD2,
			0xC2, 0x14, 0x00
		};

		*(uintptr_t*)(shell_code + 9) = *(uintptr_t*)GetProcAddress(GetModuleHandleA("ntdll.dll"), "Wow64Transition");

		*(uint32_t*)(shell_code + 4) = sys_index;

		shell_code_ = (uint8_t*)VirtualAlloc(nullptr, sizeof shell_code, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

		if (!shell_code_)
			return;

		memcpy(shell_code_, shell_code, sizeof shell_code);
	}

	uint8_t* func_addr_;
	uint8_t* shell_code_;
};
