
#ifndef _INCLUDE_HOOKER_
#define _INCLUDE_HOOKER_

#include <amxxmodule.h>
#include <global.h>
#include <librariesManager.h>
#include <memoryUtil.h>

namespace Hooker
{
	class HookerCvarRegister
	{
	private:

		static const int patchSize = sizeof(unsigned char) + sizeof(long);
		unsigned char originalBytes[patchSize / sizeof(unsigned char)];
		unsigned char patchedBytes[patchSize / sizeof(unsigned char)];

		void createPatch();
		void doPatch();
		void undoPatch();

	public:

		HookerCvarRegister();
		static void hooker(cvar_t *cvar);
	};

	extern HookerCvarRegister* hookerCvarRegister;
}

#endif

