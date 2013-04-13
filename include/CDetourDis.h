//////////////////////////////////////////////////////////////////////////////
//
//	Module:		detours.lib
//  File:		disasm.h
//
//	Detours for binary functions.  Version 1.5 (Build 46)
//  Includes support for all x86 chips prior to the Pentium III.
//
//	Copyright 1999-2001, Microsoft Corporation
//

#pragma once
#ifndef _DISASM_H_
#define _DISASM_H_

#if defined __linux__

#include <stdint.h>
#include <string.h>
#include <stddef.h>

typedef unsigned char BYTE;
typedef BYTE* PBYTE;
typedef long LONG;
typedef long* PLONG;
typedef void VOID;
typedef short SHORT;
typedef short* PSHORT;
typedef char CHAR;
typedef char* PCHAR;
typedef void* PVOID;
typedef uint32_t UINT32;
typedef int32_t INT32;
typedef unsigned long ULONG;
typedef long DWORD;

typedef int BOOL;

#ifndef FALSE
#define FALSE false
#endif

#ifndef TRUE
#define TRUE true
#endif

#else

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <windows.h>

#endif

class CDetourDis {
public:
	CDetourDis(PBYTE* ppbTarget, LONG* plExtra);
	
	PBYTE CopyInstruction(PBYTE pbDst, PBYTE pbSrc);
	PBYTE CopyInstructionEx(PBYTE pbDst, PBYTE pbSrc, PBYTE pbDstOverride);
	PBYTE CopyInstructionZero(PBYTE pbDst, PBYTE pbSrc);
	BYTE InstructionLen(PBYTE pbSrc);

	static BOOL	SanityCheckSystem();

public:
	struct COPYENTRY;
	typedef const COPYENTRY* REFCOPYENTRY;

	typedef PBYTE (CDetourDis::* COPYFUNC)(REFCOPYENTRY pEntry, PBYTE pbDst, PBYTE pbSrc);

	enum {
		DYNAMIC 	= 0x1u,
		ADDRESS 	= 0x2u,
		NOENLARGE	= 0x4u,

		SIB			= 0x10u,
		NOTSIB		= 0x0fu,
	};

	struct COPYENTRY {
		ULONG 		nOpcode 		: 8;				// Opcode
		ULONG		nFixedSize 		: 3;				// Fixed size of opcode
		ULONG		nFixedSize16 	: 3;				// Fixed size when 16 bit operand
		ULONG		nModOffset 		: 3;				// Offset to mod/rm byte (0=none)
		LONG		nRelOffset 		: 3;				// Offset to relative target.
		ULONG		nFlagBits		: 4;				// Flags for DYNAMIC, etc.
		COPYFUNC	pfCopy;								// Function pointer.
	};

protected:
#define ENTRY_CopyBytes1			1, 1, 0, 0, 0,			&CDetourDis::CopyBytes
#define ENTRY_CopyBytes1Dynamic		1, 1, 0, 0, DYNAMIC,	&CDetourDis::CopyBytes
#define ENTRY_CopyBytes2			2, 2, 0, 0, 0,			&CDetourDis::CopyBytes
#define ENTRY_CopyBytes2Jump		2, 2, 0, 1, 0,			&CDetourDis::CopyBytes
#define ENTRY_CopyBytes2CantJump	2, 2, 0, 1, NOENLARGE,	&CDetourDis::CopyBytes
#define ENTRY_CopyBytes2Dynamic		2, 2, 0, 0, DYNAMIC,	&CDetourDis::CopyBytes
#define ENTRY_CopyBytes3			3, 3, 0, 0, 0,			&CDetourDis::CopyBytes
#define ENTRY_CopyBytes3Dynamic		3, 3, 0, 0, DYNAMIC,	&CDetourDis::CopyBytes
#define ENTRY_CopyBytes3Or5			5, 3, 0, 0, 0,			&CDetourDis::CopyBytes
#define ENTRY_CopyBytes3Or5Target	5, 3, 0, 1, 0,			&CDetourDis::CopyBytes
#define ENTRY_CopyBytes5Or7Dynamic	7, 5, 0, 0, DYNAMIC,	&CDetourDis::CopyBytes
#define ENTRY_CopyBytes3Or5Address	5, 3, 0, 0, ADDRESS,	&CDetourDis::CopyBytes
#define ENTRY_CopyBytes4			4, 4, 0, 0, 0,			&CDetourDis::CopyBytes
#define ENTRY_CopyBytes5			5, 5, 0, 0, 0,			&CDetourDis::CopyBytes
#define ENTRY_CopyBytes7			7, 7, 0, 0, 0,			&CDetourDis::CopyBytes
#define ENTRY_CopyBytes2Mod			2, 2, 1, 0, 0,			&CDetourDis::CopyBytes
#define ENTRY_CopyBytes2Mod1		3, 3, 1, 0, 0,			&CDetourDis::CopyBytes
#define ENTRY_CopyBytes2ModOperand	6, 4, 1, 0, 0,			&CDetourDis::CopyBytes
#define ENTRY_CopyBytes3Mod			3, 3, 2, 0, 0,			&CDetourDis::CopyBytes
#define ENTRY_CopyBytesPrefix		1, 1, 0, 0, 0,			&CDetourDis::CopyBytesPrefix
#define ENTRY_Copy0F				1, 1, 0, 0, 0,			&CDetourDis::Copy0F
#define ENTRY_Copy66				1, 1, 0, 0, 0,			&CDetourDis::Copy66
#define ENTRY_Copy67				1, 1, 0, 0, 0,			&CDetourDis::Copy67
#define ENTRY_CopyF6				0, 0, 0, 0, 0,			&CDetourDis::CopyF6
#define ENTRY_CopyF7				0, 0, 0, 0, 0,			&CDetourDis::CopyF7
#define ENTRY_CopyFF				0, 0, 0, 0, 0,			&CDetourDis::CopyFF
#define ENTRY_Invalid				1, 1, 0, 0, 0,			&CDetourDis::Invalid
#define ENTRY_End					0, 0, 0, 0, 0,			NULL
	
	PBYTE CopyBytes(REFCOPYENTRY pEntry, PBYTE pbDst, PBYTE pbSrc);
	PBYTE CopyBytesPrefix(REFCOPYENTRY pEntry, PBYTE pbDst, PBYTE pbSrc);
	
	PBYTE Invalid(REFCOPYENTRY pEntry, PBYTE pbDst, PBYTE pbSrc);

	PBYTE AdjustTarget(PBYTE pbDst, PBYTE pbSrc, LONG cbOp, LONG cbTargetOffset);
	
	VOID Set16BitOperand();
	VOID Set32BitOperand();
	VOID Set16BitAddress();
	VOID Set32BitAddress();
	
protected:
	PBYTE Copy0F(REFCOPYENTRY pEntry, PBYTE pbDst, PBYTE pbSrc);
	PBYTE Copy66(REFCOPYENTRY pEntry, PBYTE pbDst, PBYTE pbSrc);
	PBYTE Copy67(REFCOPYENTRY pEntry, PBYTE pbDst, PBYTE pbSrc);
	PBYTE CopyF6(REFCOPYENTRY pEntry, PBYTE pbDst, PBYTE pbSrc);
	PBYTE CopyF7(REFCOPYENTRY pEntry, PBYTE pbDst, PBYTE pbSrc);
	PBYTE CopyFF(REFCOPYENTRY pEntry, PBYTE pbDst, PBYTE pbSrc);

protected:	
	static const COPYENTRY s_rceCopyTable[257];
	static const COPYENTRY s_rceCopyTable0F[257];
	static const BYTE s_rbModRm[256];

protected:
	BOOL m_b16BitOperand;
	BOOL m_b16BitAddress;

	PBYTE* m_ppbTarget;
	LONG* m_plExtra;
	
	LONG m_lScratchExtra;
	PBYTE m_pbScratchTarget;
	BYTE m_rbScratchDst[64];

	BYTE* m_pbDstOverride;
	BOOL m_bAdjustZero;
};

//////////////////////////////////////////////////////////////////////////////

enum {
	OP_PRE_ES       = 0x26,
	OP_PRE_CS       = 0x2e,
	OP_PRE_SS       = 0x36,
	OP_PRE_DS       = 0x3e,
	OP_PRE_FS       = 0x64,
	OP_PRE_GS       = 0x65,
	OP_JMP_SEG      = 0x25,
    
	OP_JA           = 0x77,
	OP_NOP          = 0x90,
	OP_CALL         = 0xe8,
	OP_JMP          = 0xe9,
	OP_PREFIX       = 0xff,
	OP_MOV_EAX      = 0xa1,
	OP_SET_EAX      = 0xb8,
	OP_JMP_EAX      = 0xe0,
	OP_RET_POP      = 0xc2,
	OP_RET          = 0xc3,
	OP_BRK          = 0xcc,

	SIZE_OF_JMP     = 5,
	SIZE_OF_NOP     = 1,
	SIZE_OF_BRK     = 1,
	SIZE_OF_TRP_OPS = SIZE_OF_JMP /* + SIZE_OF_BRK */,
};

//////////////////////////////////////////////////////////////////////////////

inline PBYTE DetourGenMovEax(PBYTE pbCode, UINT32 nValue){
	*pbCode++ = 0xB8;
	*((UINT32*&)pbCode)++ = nValue;
	return pbCode;
}

inline PBYTE DetourGenMovEbx(PBYTE pbCode, UINT32 nValue){
	*pbCode++ = 0xBB;
	*((UINT32*&)pbCode)++ = nValue;
	return pbCode;
}

inline PBYTE DetourGenMovEcx(PBYTE pbCode, UINT32 nValue){
	*pbCode++ = 0xB9;
	*((UINT32*&)pbCode)++ = nValue;
	return pbCode;
}

inline PBYTE DetourGenMovEdx(PBYTE pbCode, UINT32 nValue){
	*pbCode++ = 0xBA;
	*((UINT32*&)pbCode)++ = nValue;
	return pbCode;
}

inline PBYTE DetourGenMovEsi(PBYTE pbCode, UINT32 nValue){
	*pbCode++ = 0xBE;
	*((UINT32*&)pbCode)++ = nValue;
	return pbCode;
}

inline PBYTE DetourGenMovEdi(PBYTE pbCode, UINT32 nValue){
	*pbCode++ = 0xBF;
	*((UINT32*&)pbCode)++ = nValue;
	return pbCode;
}

inline PBYTE DetourGenMovEbp(PBYTE pbCode, UINT32 nValue){
	*pbCode++ = 0xBD;
	*((UINT32*&)pbCode)++ = nValue;
	return pbCode;
}

inline PBYTE DetourGenMovEsp(PBYTE pbCode, UINT32 nValue){
	*pbCode++ = 0xBC;
	*((UINT32*&)pbCode)++ = nValue;
	return pbCode;
}

inline PBYTE DetourGenPush(PBYTE pbCode, UINT32 nValue){
	*pbCode++ = 0x68;
	*((UINT32*&)pbCode)++ = nValue;
	return pbCode;
}

inline PBYTE DetourGenPushad(PBYTE pbCode){
	*pbCode++ = 0x60;
	return pbCode;
}

inline PBYTE DetourGenPopad(PBYTE pbCode){
	*pbCode++ = 0x61;
	return pbCode;
}

inline PBYTE DetourGenJmp(PBYTE pbCode, PBYTE pbJmpDst, PBYTE pbJmpSrc = 0){
	if(pbJmpSrc == 0)
		pbJmpSrc = pbCode;
	*pbCode++ = 0xE9;
	*((INT32*&)pbCode)++ = pbJmpDst - (pbJmpSrc + 5);
	return pbCode;
}

inline PBYTE DetourGenCall(PBYTE pbCode, PBYTE pbJmpDst, PBYTE pbJmpSrc = 0){
	if(pbJmpSrc == 0)
		pbJmpSrc = pbCode;
	*pbCode++ = 0xE8;
	*((INT32*&)pbCode)++ = pbJmpDst - (pbJmpSrc + 5);
	return pbCode;
}

inline PBYTE DetourGenBreak(PBYTE pbCode){
	*pbCode++ = 0xcc;
	return pbCode;
}

inline PBYTE DetourGenRet(PBYTE pbCode){
	*pbCode++ = 0xc3;
	return pbCode;
}

inline PBYTE DetourGenNop(PBYTE pbCode){
	*pbCode++ = 0x90;
	return pbCode;
}

#define DETOUR_INSTRUCTION_TARGET_NONE          ((PBYTE)0)
#define DETOUR_INSTRUCTION_TARGET_DYNAMIC       ((PBYTE)~0ul)

#endif //_DISASM_H_

