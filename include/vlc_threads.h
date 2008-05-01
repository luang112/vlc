/*****************************************************************************
 * vlc_threads.h : threads implementation for the VideoLAN client
 * This header provides portable declarations for mutexes & conditions
 *****************************************************************************
 * Copyright (C) 1999, 2002 the VideoLAN team
 * $Id$
 *
 * Authors: Jean-Marc Dressler <polux@via.ecp.fr>
 *          Samuel Hocevar <sam@via.ecp.fr>
 *          Gildas Bazin <gbazin@netcourrier.com>
 *          Christophe Massiot <massiot@via.ecp.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#if !defined( __LIBVLC__ )
  #error You are not libvlc or one of its plugins. You cannot include this file
#endif

#ifndef _VLC_THREADS_H_
#define _VLC_THREADS_H_

#if defined( UNDER_CE )
                                                                /* WinCE API */
#elif defined( WIN32 )
#   include <process.h>                                         /* Win32 API */
#   include <errno.h>

#elif defined( HAVE_KERNEL_SCHEDULER_H )                             /* BeOS */
#   include <kernel/OS.h>
#   include <kernel/scheduler.h>
#   include <byteorder.h>

#else                                         /* pthreads (like Linux & BSD) */
#   define LIBVLC_USE_PTHREAD 1
#   define _APPLE_C_SOURCE    1 /* Proper pthread semantics on OSX */

#   include <unistd.h> /* _POSIX_SPIN_LOCKS */
#   include <pthread.h>
    /* Needed for pthread_cond_timedwait */
#   include <errno.h>
#   include <time.h>

#endif

/*****************************************************************************
 * Constants
 *****************************************************************************/

/* Thread priorities */
#ifdef __APPLE__
#   define VLC_THREAD_PRIORITY_LOW (-47)
#   define VLC_THREAD_PRIORITY_INPUT 37
#   define VLC_THREAD_PRIORITY_AUDIO 37
#   define VLC_THREAD_PRIORITY_VIDEO (-47)
#   define VLC_THREAD_PRIORITY_OUTPUT 37
#   define VLC_THREAD_PRIORITY_HIGHEST 37

#elif defined(SYS_BEOS)
#   define VLC_THREAD_PRIORITY_LOW 5
#   define VLC_THREAD_PRIORITY_INPUT 10
#   define VLC_THREAD_PRIORITY_AUDIO 10
#   define VLC_THREAD_PRIORITY_VIDEO 5
#   define VLC_THREAD_PRIORITY_OUTPUT 15
#   define VLC_THREAD_PRIORITY_HIGHEST 15

#elif defined(LIBVLC_USE_PTHREAD)
#   define VLC_THREAD_PRIORITY_LOW 0
#   define VLC_THREAD_PRIORITY_INPUT 20
#   define VLC_THREAD_PRIORITY_AUDIO 10
#   define VLC_THREAD_PRIORITY_VIDEO 0
#   define VLC_THREAD_PRIORITY_OUTPUT 30
#   define VLC_THREAD_PRIORITY_HIGHEST 40

#elif defined(WIN32) || defined(UNDER_CE)
/* Define different priorities for WinNT/2K/XP and Win9x/Me */
#   define VLC_THREAD_PRIORITY_LOW 0
#   define VLC_THREAD_PRIORITY_INPUT \
        (IS_WINNT ? THREAD_PRIORITY_ABOVE_NORMAL : 0)
#   define VLC_THREAD_PRIORITY_AUDIO \
        (IS_WINNT ? THREAD_PRIORITY_HIGHEST : 0)
#   define VLC_THREAD_PRIORITY_VIDEO \
        (IS_WINNT ? 0 : THREAD_PRIORITY_BELOW_NORMAL )
#   define VLC_THREAD_PRIORITY_OUTPUT \
        (IS_WINNT ? THREAD_PRIORITY_ABOVE_NORMAL : 0)
#   define VLC_THREAD_PRIORITY_HIGHEST \
        (IS_WINNT ? THREAD_PRIORITY_TIME_CRITICAL : 0)

#else
#   define VLC_THREAD_PRIORITY_LOW 0
#   define VLC_THREAD_PRIORITY_INPUT 0
#   define VLC_THREAD_PRIORITY_AUDIO 0
#   define VLC_THREAD_PRIORITY_VIDEO 0
#   define VLC_THREAD_PRIORITY_OUTPUT 0
#   define VLC_THREAD_PRIORITY_HIGHEST 0

#endif

/*****************************************************************************
 * Type definitions
 *****************************************************************************/

#if defined( WIN32 ) || defined( UNDER_CE )
typedef struct
{
    /* thread id */
    DWORD  id;
    /*
    ** handle to created thread, needs be closed to dispose of it
    ** even after thread has exited
    */
    HANDLE hThread;
} vlc_thread_t;

typedef BOOL (WINAPI *SIGNALOBJECTANDWAIT) ( HANDLE, HANDLE, DWORD, BOOL );

typedef HANDLE  vlc_mutex_t;

typedef struct
{
    volatile int        i_waiting_threads;
    HANDLE              event;
} vlc_cond_t;

typedef DWORD   vlc_threadvar_t;

#elif defined( HAVE_KERNEL_SCHEDULER_H )
/* This is the BeOS implementation of the vlc threads, note that the mutex is
 * not a real mutex and the cond_var is not like a pthread cond_var but it is
 * enough for what we need */

typedef thread_id vlc_thread_t;

typedef struct
{
    int32_t         init;
    sem_id          lock;
} vlc_mutex_t;

typedef struct
{
    int32_t         init;
    thread_id       thread;
} vlc_cond_t;

typedef struct
{
} vlc_threadvar_t;


#else
typedef pthread_t       vlc_thread_t;
typedef pthread_mutex_t vlc_mutex_t;
typedef pthread_cond_t  vlc_cond_t;
typedef pthread_key_t   vlc_threadvar_t;

#endif

#endif /* !_VLC_THREADS_H */
