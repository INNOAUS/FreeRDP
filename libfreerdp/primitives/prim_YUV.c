/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 *
 * Copyright 2014 Marc-Andre Moreau <marcandre.moreau@gmail.com>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <freerdp/types.h>
#include <freerdp/primitives.h>
#include <freerdp/codec/color.h>

#include "prim_internal.h"
#include "prim_YUV.h"

/**
 * | R |    ( | 256     0    403 | |    Y    | )
 * | G | = (  | 256   -48   -120 | | U - 128 |  ) >> 8
 * | B |    ( | 256   475      0 | | V - 128 | )
 *
 * | Y |    ( |  54   183     18 | | R | )         |  0  |
 * | U | = (  | -29   -99    128 | | G |  ) >> 8 + | 128 |
 * | V |    ( | 128  -116    -12 | | B | )         | 128 |
 */

pstatus_t general_YUV420ToRGB_8u_P3AC4R(const BYTE* pSrc[3], int srcStep[3],
		BYTE* pDst, int dstStep, const prim_size_t* roi)
{
	int x, y;
	int dstPad;
	int srcPad[3];
	BYTE Y, U, V;
	int halfWidth;
	int halfHeight;
	const BYTE* pY;
	const BYTE* pU;
	const BYTE* pV;
	int R, G, B;
	int Yp, Up, Vp;
	int Up48, Up475;
	int Vp403, Vp120;
	BYTE* pRGB = pDst;
	int nWidth, nHeight;
	int lastRow, lastCol;

	pY = pSrc[0];
	pU = pSrc[1];
	pV = pSrc[2];
	
	lastCol = roi->width & 0x01;
	lastRow = roi->height & 0x01;
	
	nWidth = (roi->width + 1) & ~0x0001;
	nHeight = (roi->height + 1) & ~0x0001;

	halfWidth = nWidth / 2;
	halfHeight = nHeight / 2;

	srcPad[0] = (srcStep[0] - nWidth);
	srcPad[1] = (srcStep[1] - halfWidth);
	srcPad[2] = (srcStep[2] - halfWidth);

	dstPad = (dstStep - (nWidth * 4));

	for (y = 0; y < halfHeight; )
	{
		if (++y == halfHeight)
			lastRow <<= 1;

		for (x = 0; x < halfWidth; )
		{
			if (++x == halfWidth)
				lastCol <<= 1;

			U = *pU++;
			V = *pV++;

			Up = U - 128;
			Vp = V - 128;

			Up48 = 48 * Up;
			Up475 = 475 * Up;

			Vp403 = Vp * 403;
			Vp120 = Vp * 120;

			/* 1st pixel */

			Y = *pY++;
			Yp = Y << 8;

			R = (Yp + Vp403) >> 8;
			G = (Yp - Up48 - Vp120) >> 8;
			B = (Yp + Up475) >> 8;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;

			if (G < 0)
				G = 0;
			else if (G > 255)
				G = 255;

			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;

			*pRGB++ = (BYTE) B;
			*pRGB++ = (BYTE) G;
			*pRGB++ = (BYTE) R;
			*pRGB++ = 0xFF;

			/* 2nd pixel */

			if (!(lastCol & 0x02))
			{
				Y = *pY++;
				Yp = Y << 8;

				R = (Yp + Vp403) >> 8;
				G = (Yp - Up48 - Vp120) >> 8;
				B = (Yp + Up475) >> 8;

				if (R < 0)
					R = 0;
				else if (R > 255)
					R = 255;

				if (G < 0)
					G = 0;
				else if (G > 255)
					G = 255;

				if (B < 0)
					B = 0;
				else if (B > 255)
					B = 255;

				*pRGB++ = (BYTE) B;
				*pRGB++ = (BYTE) G;
				*pRGB++ = (BYTE) R;
				*pRGB++ = 0xFF;
			}
			else
			{
				pY++;
				pRGB += 4;
				lastCol >>= 1;
			}
		}

		pY += srcPad[0];
		pU -= halfWidth;
		pV -= halfWidth;
		pRGB += dstPad;

		for (x = 0; x < halfWidth; )
		{
			if (++x == halfWidth)
				lastCol <<= 1;

			U = *pU++;
			V = *pV++;

			Up = U - 128;
			Vp = V - 128;

			Up48 = 48 * Up;
			Up475 = 475 * Up;

			Vp403 = Vp * 403;
			Vp120 = Vp * 120;

			/* 3rd pixel */

			Y = *pY++;
			Yp = Y << 8;

			R = (Yp + Vp403) >> 8;
			G = (Yp - Up48 - Vp120) >> 8;
			B = (Yp + Up475) >> 8;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;

			if (G < 0)
				G = 0;
			else if (G > 255)
				G = 255;

			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;

			*pRGB++ = (BYTE) B;
			*pRGB++ = (BYTE) G;
			*pRGB++ = (BYTE) R;
			*pRGB++ = 0xFF;

			/* 4th pixel */

			if (!(lastCol & 0x02))
			{
				Y = *pY++;
				Yp = Y << 8;

				R = (Yp + Vp403) >> 8;
				G = (Yp - Up48 - Vp120) >> 8;
				B = (Yp + Up475) >> 8;

				if (R < 0)
					R = 0;
				else if (R > 255)
					R = 255;

				if (G < 0)
					G = 0;
				else if (G > 255)
					G = 255;

				if (B < 0)
					B = 0;
				else if (B > 255)
					B = 255;

				*pRGB++ = (BYTE) B;
				*pRGB++ = (BYTE) G;
				*pRGB++ = (BYTE) R;
				*pRGB++ = 0xFF;
			}
			else
			{
				pY++;
				pRGB += 4;
				lastCol >>= 1;
			}
		}

		pY += srcPad[0];
		pU += srcPad[1];
		pV += srcPad[2];
		pRGB += dstPad;
	}

	return PRIMITIVES_SUCCESS;
}

void primitives_init_YUV(primitives_t* prims)
{
	prims->YUV420ToRGB_8u_P3AC4R = general_YUV420ToRGB_8u_P3AC4R;
	
	primitives_init_YUV_opt(prims);
}

void primitives_deinit_YUV(primitives_t* prims)
{

}
