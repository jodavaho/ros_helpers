#include "helpers/jv_help.h"

///////////////////////////////IMPL//////////////////////////////////
#ifdef WIN32
#define WINDOWS
#endif

#ifdef WINDOWS
/* To cooperate with windows programs */
	#include <Windows.h>
	void toWC(const char* msg,wchar_t *buf);
	LARGE_INTEGER getFILETIMEoffset();
	int	clock_gettime(int X, struct timeval *tv);
	typedef struct timeval TTYPE;
	#define CLOCK_MONOTONIC -1
#else
	typedef struct timespec TTYPE;
#endif

#ifndef _MAX_TIMERS
#define _MAX_TIMERS 16
#endif

static TTYPE _start_timers[_MAX_TIMERS];
static TTYPE _stop_timers[_MAX_TIMERS];
static double _running_timers[_MAX_TIMERS];
static int _isrunning[_MAX_TIMERS];

void _dropout(const int errcode){
	exit(errcode);
}

#ifdef WINDOWS
void _fmecho(const char* pref, const char* str,const char* file,int line, int nl,const char* s,...){
	wchar_t buf[4096];
	wchar_t obuf[4096];
	wchar_t fname[4096];
	wchar_t pbuf[64];
	char fbuf[4096];
	wchar_t wfbuf[4096];
	va_list args;
	va_start(args,s);
	vsprintf_s(fbuf,4096,s,args);

	toWC(fbuf,wfbuf);
	toWC(file,fname);
	toWC(str,buf);
	toWC(pref,pbuf);
	if (nl){
		swprintf(obuf,4096,L"%s: (%s,%d) %s %s\n",pbuf,fname,line,buf,wfbuf);
	}
	else{
		swprintf(obuf,4096,L"%s: (%s,%d) %s %s",pbuf,fname,line,buf,wfbuf);
	}
	OutputDebugString(obuf);
	wprintf(obuf);
}
void _mecho(const char* pref, const char* str,const char* file,int line, int nl){
	wchar_t buf[4096];
	wchar_t obuf[4096];
	wchar_t fname[4096];
	wchar_t pbuf[64];
	toWC(file,fname);
	toWC(str,buf);
	toWC(pref,pbuf);
	if (nl){
		swprintf(obuf,4096,L"%s: (%s,%d) %s\n",pbuf,fname,line,buf);
	}
	else{
		swprintf(obuf,4096,L"%s: (%s,%d) %s",pbuf,fname,line,buf);
	}
	OutputDebugString(obuf);
	wprintf(obuf);
}
#else
void _fmecho(const char* pref, const char* str,const char* fs,int ll, int nl,const char*s, ...){
	char fbuf[4096];
	va_list args;
	va_start(args,s);
	vsnprintf(fbuf,4096,s,args);
#ifdef WIRITING_MPI
	if (nl)
		printf("%s: %s, %d/%d (%s,%d) %s %s\n",pref,proc_name,myid,P-1,fs,ll,str,fbuf);
	else
		printf("%s: %s, %d/%d (%s,%d) %s %s",pref,proc_name,myid,P-1,fs,ll,str,fbuf);
#else
	if (nl)
		printf("%s: (%s,%d) %s\n",pref,fs,ll,str);
	else
		printf("%s: (%s,%d) %s",pref,fs,ll,str);
#endif
       
}
void _mecho(const char* pref, const char* str,const char* fs,int ll, int nl){
#ifdef WIRITING_MPI
	if (nl)
		printf("%s: %s, %d/%d (%s,%d) %s\n",pref,proc_name,myid,P-1,fs,ll,str);
	else
		printf("%s: %s, %d/%d (%s,%d) %s",pref,proc_name,myid,P-1,fs,ll,str);
#else
	if (nl)
		printf("%s: (%s,%d) %s\n",pref,fs,ll,str);
	else
		printf("%s: (%s,%d) %s",pref,fs,ll,str);
#endif
       
}
#endif

#ifdef WINDOWS
void toWC(const char* msg,wchar_t *buf){
	if (msg!=NULL){
		size_t origsize = strlen(msg) + 1;
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, buf, origsize, msg, _TRUNCATE);
	} else {
		//wcscat_s(buf, 5, L"NULL");
	}
}


LARGE_INTEGER
getFILETIMEoffset()
{
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;

    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return (t);
}

int
clock_gettime(int X, struct timeval *tv)
{
    LARGE_INTEGER           t;
    FILETIME            f;
    LONGLONG                  microseconds;
    static LARGE_INTEGER    offset;
    static double           frequencyToMicroseconds;
    static int              initialized = 0;
    static BOOL             usePerformanceCounter = 1;

    if (!initialized) {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
        } else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter) QueryPerformanceCounter(&t);
    else {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }

    t.QuadPart -= offset.QuadPart;
    microseconds = (LONGLONG)( t.QuadPart / frequencyToMicroseconds);
    t.QuadPart = microseconds;
    tv->tv_sec = (long)( t.QuadPart / 1000000L);
    tv->tv_usec = t.QuadPart % 1000000L;
    return (0);
}
#endif

#ifdef WRITING_MPI
void _mpi_settings(int argc, char** argv){
	// Initialize MPI
	MPI_Init(&argc, &argv);
	// Obtain the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &P);
	// Obtain the process id
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	// Obtain name of machine process is executing on
	MPI_Get_processor_name(proc_name, &name_len);
}
#endif

//////////////////////////TIME HELP BELOW//////////////////////////////////
double since_start(const int which){
	TTYPE *start;
	TTYPE now;
	clock_gettime(CLOCK_MONOTONIC,&now);
	start=&_start_timers[which];
	double elapsed = (now.tv_sec - start->tv_sec);
#ifndef WINDOWS
	elapsed += (now.tv_nsec - start->tv_nsec) / 1000000000.0;
#else
	elapsed += (now.tv_usec - start->tv_usec) / 1000000.0;
#endif
	return elapsed;
}
static inline double _get_time_diff(const int which){
	TTYPE *start,*finish;
	start=&_start_timers[which];
	finish=&_stop_timers[which];
	double elapsed = (finish->tv_sec - start->tv_sec);
#ifndef WINDOWS
	elapsed += (finish->tv_nsec - start->tv_nsec) / 1000000000.0;
#else
	elapsed += (finish->tv_usec - start->tv_usec) / 1000000.0;
#endif
	return elapsed;
}

double current_time(const int which){
	if (_isrunning[which]){
		return _get_time_diff(which) + _running_timers[which];
	} else {
		return _running_timers[which];
	}
}
double get_time(const int which){
	return current_time(which);
}
void start_timer(const int which){
	_running_timers[which]=0.0;
	continue_timer(which);
}

void continue_timer(const int which){
	clock_gettime(CLOCK_MONOTONIC, &_start_timers[which]);
}

double stop_timer(const int which){
	clock_gettime(CLOCK_MONOTONIC, &_stop_timers[which]);
	_running_timers[which]+=_get_time_diff(which);
	return get_time(which);
}

#define restart_timer(a) start_timer(a)
#define pause_timer(a) stop_timer(a)
