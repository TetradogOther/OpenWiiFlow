#include <malloc.h>
#include <gccore.h>
#include <string.h>

#include "dolloader.h"

typedef struct _dolheader {
	u32 text_pos[7];
	u32 data_pos[11];
	u32 text_start[7];
	u32 data_start[11];
	u32 text_size[7];
	u32 data_size[11];
	u32 bss_start;
	u32 bss_size;
	u32 entry_point;
} dolheader;

u32 load_dol_image(const void *dolstart, struct __argv *argv)
{
	if (dolstart)
	{
		u32 i;
		dolheader *dolfile = (dolheader *) dolstart;
		for (i = 0; i < 7; i++)
		{
			if ((!dolfile->text_size[i]) || (dolfile->text_start[i] < 0x100)) continue;
			ICInvalidateRange ((void *) dolfile->text_start[i],dolfile->text_size[i]);
			memcpy((void *) dolfile->text_start[i],dolstart+dolfile->text_pos[i],dolfile->text_size[i]);
		}

		for (i = 0; i < 11; i++)
		{
			if ((!dolfile->data_size[i]) || (dolfile->data_start[i] < 0x100)) continue;
			memcpy((void *) dolfile->data_start[i],dolstart+dolfile->data_pos[i],dolfile->data_size[i]);
			DCFlushRangeNoSync ((void *) dolfile->data_start[i],dolfile->data_size[i]);
		}

		memset ((void *) dolfile->bss_start, 0, dolfile->bss_size);
		DCFlushRange((void *) dolfile->bss_start, dolfile->bss_size);

		if (argv && argv->argvMagic == ARGV_MAGIC)
		{
			void *new_argv = (void *)(dolfile->entry_point + 8);
			memcpy(new_argv, argv, sizeof(*argv));
			DCFlushRange(new_argv, sizeof(*argv));
		}

		return dolfile->entry_point;
	}
	return 0;
}
