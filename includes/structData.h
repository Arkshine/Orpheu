
#ifndef _INCLUDE_STRUCT_DATA_
#define _INCLUDE_STRUCT_DATA_

#include <typeHandler.h>
#include <am-string.h>
#include <am-vector.h>
#include <structInfo.h>
#include <global.h>

using namespace Global;

template <typename T> class StructData
{
private:

	ke::Vector<Member> membersVector;
	StructInfo structInfo;

protected:

	T* structure;
	void addMember(const char *name, void* offset, TypeHandler* type)
	{
		Member member;

		member.name = name;
		member.offset = (long)offset - (long)structure;
		member.type = type;

		membersVector.append(member);
	}

public:

	virtual const char *getName() = 0;

	StructInfo& getStructInfo()
	{
		unsigned int membersCount = membersVector.length();

		structInfo.membersCount = membersCount;
		structInfo.members = new Member[membersCount];
		structInfo.type = getName();
		structInfo.size = sizeof(T);

		for (unsigned int i=0; i < membersCount; i++)
		{
			structInfo.members[i].name = membersVector[i].name;
			structInfo.members[i].type = membersVector[i].type;
			structInfo.members[i].offset = membersVector[i].offset;
		}
		return structInfo;
	}
};

#endif

