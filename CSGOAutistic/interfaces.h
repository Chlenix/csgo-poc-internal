#pragma once

#include <iostream>
#include <Windows.h>
#include <string>

namespace interfaces
{
	class InterfacesHead;
	class InterfaceNode;

	class InterfacesHead
	{
	public:
		InterfaceNode* HeadNode; //0x0000 
	};//Size=0x0040

	typedef int(*oCapture)(void);

	class InterfaceNode
	{
	public:
		oCapture get; //0x0000 
		char* pName; //0x0004 
		InterfaceNode* pNext; //0x0008 
	};//Size=0x001C

	std::uintptr_t make(char const *interface_name, char const *object_name)
	{
		InterfacesHead* interface_head;

		DWORD *pCreateInterfaceFn = reinterpret_cast<DWORD*>(GetProcAddress(GetModuleHandleA(interface_name), "CreateInterface"));
		pCreateInterfaceFn = (DWORD*)((DWORD)pCreateInterfaceFn + 0x4); // Later on replace this +4 with a search for the first jmp(0xE9)
		int JmpConv = *(int*)((DWORD)pCreateInterfaceFn + 1); // + 1 to skip the opcode

		pCreateInterfaceFn = (DWORD*)(5 + (DWORD)pCreateInterfaceFn + (DWORD)JmpConv); // Jmp is relative to the end of this line (hence the +5 bytes)

		//std::cout << "pCreateInterfaceFn: 0x" << std::dec << pCreateInterfaceFn << std::endl;
		// Step 2: Get the head of the list
		// MOV ESI,DWORD PTR DS:[1A69F904]
		interface_head = *(InterfacesHead**)((DWORD)pCreateInterfaceFn + 0x6); // Later use a search instead

		//std::cout << "pIntHead: 0x" << std::dec << pIntHead << std::endl;

		if (interface_head)
		{
			for (InterfaceNode* interface_node = interface_head->HeadNode; interface_node; interface_node = interface_node->pNext)
			{
				if (std::string(interface_node->pName).find(object_name) != std::string::npos)
				{
					std::uintptr_t obj = interface_node->get();
					std::cout << interface_name << "::" << object_name << "-> 0x" << std::hex << obj << std::endl;
					return obj;
				}
			}
		}
	}
}