
#ifndef _INCLUDE_STRUCT_INFO_
#define _INCLUDE_STRUCT_INFO_

struct Member
{
	ke::AString name;
	long offset;
	TypeHandler* type;
};

struct StructInfo
{
	Member* members;
	int membersCount;
	ke::AString type;
	unsigned int size;
};

#endif
