/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * Dynamic Virtual Channel Extension
 *
 * Copyright 2013 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FREERDP_CHANNEL_CLIENT_RDPEI_H
#define FREERDP_CHANNEL_CLIENT_RDPEI_H

#include <freerdp/channels/rdpei.h>

/**
 * Client Interface
 */

typedef struct _rdpei_client_context RdpeiClientContext;

typedef int (*pcRdpeiGetVersion)(RdpeiClientContext* context);typedef int (*pcRdpeiAddContact)(RdpeiClientContext* context, RDPINPUT_CONTACT_DATA* contact);

typedef int (*pcRdpeiTouchBegin)(RdpeiClientContext* context, int externalId, int x, int y);
typedef int (*pcRdpeiTouchUpdate)(RdpeiClientContext* context, int externalId, int x, int y);
typedef int (*pcRdpeiTouchEnd)(RdpeiClientContext* context, int externalId, int x, int y);

typedef int (*pcRdpeiSuspendTouch)(RdpeiClientContext* context);
typedef int (*pcRdpeiResumeTouch)(RdpeiClientContext* context);

struct _rdpei_client_context
{
	void* handle;
	void* custom;

	pcRdpeiGetVersion GetVersion;

	pcRdpeiAddContact AddContact;

	pcRdpeiTouchBegin TouchBegin;
	pcRdpeiTouchUpdate TouchUpdate;
	pcRdpeiTouchEnd TouchEnd;

	pcRdpeiSuspendTouch SuspendTouch;
	pcRdpeiResumeTouch ResumeTouch;
};

#endif /* FREERDP_CHANNEL_CLIENT_RDPEI_H */
