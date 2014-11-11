
#ifndef _INCLUDE_TYPE_HANDLER_
#define _INCLUDE_TYPE_HANDLER_

#include <memoryUtil.h>
#include <amxxmodule.h>
#include <am-vector.h>

#if defined(WIN32)
	#pragma warning(disable:4800)
#endif

typedef enum
{
	Call,
	CallHooked,
	Pass,
}ConvertMode;

class TypeHandler
{
private:

	ke::Vector<void*>* allocatedMemoryPointers;

public:

	virtual void* convertFromAmx(AMX* amx, cell param) = 0;
	virtual void convertToAmx(cell& value, long standardReturn, ConvertMode convertMode = Pass) = 0;
	virtual void convertToAmxByRef(AMX* amx, cell* params, long standardReturn){}

	virtual void convertFromAmxToStructure(AMX* amx, cell param, void* address) = 0;
	virtual cell convertToAmxFromStructure(AMX* amx, cell* params, void* address) = 0;

	virtual int getIfByRefParamsCount(){ return 0; }
	virtual ForwardParam getParamType(){ return FP_CELL; }
	virtual const char* getName() = 0;
	virtual bool isPassedByRef(){ return false; }
	virtual bool isStructure(){ return false; }

	virtual bool isPointer()
	{
		return false;
	}

	virtual bool compareTo(AMX* amx, cell* params, void* address)
	{
		return *MF_GetAmxAddr(amx, params[0]) == convertToAmxFromStructure(amx, params, address);
	}

	virtual int getSize(AMX* amx, cell param) = 0;

	TypeHandler()
	{
		allocatedMemoryPointers = new ke::Vector<void*>();
		virtualTableOffsetSet = false;
	}

	void setVirtualTableOffset(long offset)
	{
		this->virtualTableOffsetSet = true;
		this->virtualTableOffset = offset;
	}

	long& getVirtualTableOffset()
	{
		return this->virtualTableOffset;
	}

	bool hasVirtualTableOffset()
	{
		return this->virtualTableOffsetSet;
	}

protected:

	long virtualTableOffset;
	bool virtualTableOffsetSet;

	template <class T> void convertFromAmxToStructureStandard(AMX* amx, cell param, void* address)
	{
		*(reinterpret_cast<T*>(address)) = (T)(long)convertFromAmx(amx, param);
	}

	template <class T> cell convertToAmxFromStructureStandard(AMX* amx, cell* params, void* address)
	{
		cell value;
		this->convertToAmx(value, *((T*)(address)));
		return value;
	}

	template <class T> T* allocateMemory(int elements = 1)
	{
		void* pointer = malloc(sizeof(T) * elements);
		allocatedMemoryPointers->append(pointer);
		return (T*)pointer;
	}

	void* allocateMemoryBySize(unsigned int size)
	{
		void* pointer = malloc(size);
		allocatedMemoryPointers->append(pointer);
		return pointer;
	}

public:

	void freeAllocatedMemory()
	{
		for (unsigned int i=0; i < allocatedMemoryPointers->length(); i++)
		{
			free(allocatedMemoryPointers->at(i));
		}

		allocatedMemoryPointers->clear();
	}
};

#endif

