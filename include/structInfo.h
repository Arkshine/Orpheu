
#ifndef _INCLUDE_STRUCT_INFO_
#define _INCLUDE_STRUCT_INFO_

struct Member
{
	string name;
	long offset;
	TypeHandler* type;
};

struct StructInfo
{
	Member* members;
	int membersCount;
	string type;
	unsigned int size;
};

#endif
