
#ifndef _INCLUDE_STRUCT_HANDLER_
#define _INCLUDE_STRUCT_HANDLER_

#include <typeHandlerImplementations/longHandler.h>
#include <am-string.h>
#include <structInfo.h>
#include <sm_stringhashmap.h>

class StructHandler : public LongHandler
{
protected:
	ke::AString name;
	StringHashMap<int> memberNameToMember;
	StructInfo structInfo;

public:

	StructHandler(StructInfo structInfo);
	const char* getName() { return name.chars(); }
	bool isStructure(){ return true; }
	int getMemberID(char* memberName);
	TypeHandler* getMemberTypeHandler(unsigned int memberID);
	long convertMemberToAmx(AMX* amx, cell* params, int memberID, long structureAddress);
	void convertMemberFromAmx(AMX* amx, cell* params, int memberID, long structureAddress);
	long allocate();
	int getStructSize();
	void convertToAmx(cell& value, long standardReturn, ConvertMode convertMode = Pass);
};

#endif

