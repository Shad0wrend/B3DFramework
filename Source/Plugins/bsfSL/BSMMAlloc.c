//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsMMAlloc.h"
#include <stdlib.h>
#include <string.h>

typedef struct tagMMAllocHeader MMAllocHeader;

struct tagMMAllocHeader
{
	MMAllocHeader* Next;
	MMAllocHeader* Prev;
};

void* mmalloc_new_context()
{
	MMAllocHeader* header = (MMAllocHeader*)malloc(sizeof(MMAllocHeader));
	header->Next = 0;
	header->Prev = 0;

	return header;
}

void mmalloc_free_context(void* context)
{
	MMAllocHeader* header = (MMAllocHeader*)context;
	while (header->Next != 0)
		mmfree((char*)header->Next + sizeof(MMAllocHeader));

	free(header);
}

void* mmalloc(void* context, int size)
{
	void* buffer = malloc(size + sizeof(MMAllocHeader));

	MMAllocHeader* header = (MMAllocHeader*)buffer;
	MMAllocHeader* parent = (MMAllocHeader*)context;

	header->Next = parent->Next;
	if (parent->Next)
		parent->Next->Prev = header;

	header->Prev = parent;
	parent->Next = header;

	return (char*)buffer + sizeof(MMAllocHeader);
}

void mmfree(void* ptr)
{
	void* buffer = (char*)ptr - sizeof(MMAllocHeader);
	MMAllocHeader* header = (MMAllocHeader*)buffer;

	if (header->Prev)
		header->Prev->Next = header->Next;

	if (header->Next)
		header->Next->Prev = header->Prev;

	free(buffer);
}

char* mmalloc_strdup(void* context, const char* input)
{
	size_t length = strlen(input);
	char* output = (char*)mmalloc(context, (int)(sizeof(char) * (length + 1)));

	memcpy(output, input, length);
	output[length] = '\0';

	return output;
}
