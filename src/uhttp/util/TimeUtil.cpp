/******************************************************************
*
* uHTTP for C++
*
* Copyright (C) Satoshi Konno 2002-2012
*
* This is licensed under BSD-style license, see file COPYING.
*
******************************************************************/

#if defined(WIN32) && defined(ITRON) && defined (_AFXDLL)
#include <afxwin.h>
#endif

#if defined(WIN32)
	#include <windows.h>
	#include <time.h>
	#if defined(ITRON)
	#include <kernel.h>
	#endif
#elif defined(BTRON)
	#include <btron/proctask.h>
	#include <btron/clk.h>
	#include <time.h>
#elif defined(ITRON)
	#include <kernel.h>
#elif defined(TENGINE) && !defined(PROCESS_BASE)
	#include <tk/tkernel.h>
	#include <time.h>
#elif defined(TENGINE) && defined(PROCESS_BASE)
	#include <tk/tkernel.h>
	#include <btron/proctask.h>
	#include <time.h>
#else
	#include <unistd.h>
	#include <time.h>
#endif

#include <stdlib.h>

#include <uhttp/util/TimeUtil.h>

using namespace uHTTP;

void uHTTP::Wait(long mtime)
{
#if defined(WIN32)
	Sleep(mtime);
#elif defined(BTRON)
	slp_tsk(mtime);
#elif defined(ITRON)
	tslp_tsk(mtime);
#elif defined(TENGINE) && !defined(PROCESS_BASE)
	tk_slp_tsk(mtime);
#elif defined(TENGINE) && defined(PROCESS_BASE)
	b_slp_tsk(mtime);
#else
	usleep(mtime*1000);
#endif
}

void uHTTP::WaitRandom(long mtime)
{
	double factor = (double)rand() / (double)RAND_MAX;
	long waitTime = (long)((double)mtime * factor);
	Wait(waitTime);
}

long uHTTP::GetCurrentSystemTime()
{
#if defined(BTRON)
	STIME systime;
	TIMEZONE tz;
	if (get_tim(&systime, &tz) != 0)
		return 0;
	STIME localtime = systime - tz.adjust + (tz.dst_flg ? (tz.dst_adj*60): 0);
	return localtime;
#elif defined(ITRON)
	static BOOL initialized = FALSE;
	SYSTIM sysTim;
	if (initialized == FALSE) {
		sysTim.utime = 0;
		sysTim.ltime = 0;
		set_tim(&sysTim);
	}
	get_tim(&sysTim);
	return ((sysTim.utime / 1000) << 32) + (sysTim.ltime / 1000);
#else
	return time(NULL);
#endif
}

float uHTTP::Random()
{
	static bool seedDone = false;
	if (seedDone == false) {
		srand(time(NULL));
		seedDone = true;
	}
	return (float)rand() / (float)RAND_MAX;
}
