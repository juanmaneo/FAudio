/* FAudio - XAudio Reimplementation for FNA
 *
 * Copyright (c) 2011-2018 Ethan Lee, Luigi Auriemma, and the MonoGame Team
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software in a
 * product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Ethan "flibitijibibo" Lee <flibitijibibo@flibitijibibo.com>
 *
 */

#include "FAudioFX.h"
#include "FAudio_internal.h"

/* Volume Meter FAPO Implementation */

static FAPORegistrationProperties VolumeMeterProperties =
{
	/* .clsid = */ {0},
	/* .FriendlyName = */
	{
		'V', 'o', 'l', 'u', 'm', 'e', 'M', 'e', 't', 'e', 'r', '\0'
	},
	/*.CopyrightInfo = */
	{
		'C', 'o', 'p', 'y', 'r', 'i', 'g', 'h', 't', ' ', '(', 'c', ')',
		'E', 't', 'h', 'a', 'n', ' ', 'L', 'e', 'e', '\0'
	},
	/*.MajorVersion = */ 0,
	/*.MinorVersion = */ 0,
	/*.Flags = */(
		FAPO_FLAG_FRAMERATE_MUST_MATCH |
		FAPO_FLAG_BITSPERSAMPLE_MUST_MATCH |
		FAPO_FLAG_BUFFERCOUNT_MUST_MATCH |
		FAPO_FLAG_INPLACE_SUPPORTED |
		FAPO_FLAG_INPLACE_REQUIRED
	),
	/*.MinInputBufferCount = */ 1,
	/*.MaxInputBufferCount = */  1,
	/*.MinOutputBufferCount = */ 1,
	/*.MaxOutputBufferCount =*/ 1
};

typedef struct FAudioFXVolumeMeter
{
	FAPOBase base;

	/* TODO */
} FAudioFXVolumeMeter;

void FAudioFXVolumeMeter_Process(
	FAudioFXVolumeMeter *fapo,
	uint32_t InputProcessParameterCount,
	const FAPOProcessBufferParameters* pInputProcessParameters,
	uint32_t OutputProcessParameterCount,
	FAPOProcessBufferParameters* pOutputProcessParameters,
	uint8_t IsEnabled
) {
	FAudioFXVolumeMeterLevels *levels = (FAudioFXVolumeMeterLevels*)
		FAPOBase_BeginProcess(&fapo->base);

	if (levels->pPeakLevels)
	{
		/* TODO: pPeakLevels */
		FAudio_zero(
			levels->pPeakLevels,
			levels->ChannelCount * sizeof(float)
		);
	}
	if (levels->pRMSLevels)
	{
		/* TODO: pRMSLevels */
		FAudio_zero(
			levels->pRMSLevels,
			levels->ChannelCount * sizeof(float)
		);
	}

	FAPOBase_EndProcess(&fapo->base);
}

void FAudioFXVolumeMeter_Free(void* fapo)
{
	FAudio_free(fapo);
}

/* Public API */

uint32_t FAudioCreateVolumeMeter(FAPO** ppApo, uint32_t Flags)
{
	/* Allocate... */
	FAudioFXVolumeMeter *result = (FAudioFXVolumeMeter*) FAudio_malloc(
		sizeof(FAudioFXVolumeMeter)
	);
	uint8_t *params = (uint8_t*) FAudio_malloc(
		sizeof(FAudioFXVolumeMeterLevels) * 3
	);
	FAudio_zero(params, sizeof(FAudioFXVolumeMeterLevels) * 3);

	/* Initialize... */
	CreateFAPOBase(
		&result->base,
		&VolumeMeterProperties,
		params,
		sizeof(FAudioFXVolumeMeterLevels),
		1
	);

	/* Function table... */
	result->base.base.Process = (ProcessFunc)
		FAudioFXVolumeMeter_Process;
	result->base.Destructor = FAudioFXVolumeMeter_Free;

	/* Finally. */
	*ppApo = &result->base.base;
	return 0;
}
