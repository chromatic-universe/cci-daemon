// ------------------------------
#pragma once


#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (HACCIFS_HAS_DLL)
#  define HACCIFS_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && ! HACCIFS_HAS_DLL */

#if !defined (HACCIFS_HAS_DLL)
#  define HACCIFS_HAS_DLL 1
#endif /* ! HACCIFS_HAS_DLL */

#if defined (HACCIFS_HAS_DLL) && (HACCIFS_HAS_DLL == 1)
#  if defined (HACCIFS_BUILD_DLL)
#    define HACCIFS_Export ACE_Proper_Export_Flag
#    define HACCIFS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define HACCIFS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* HACCIFS_BUILD_DLL */
#    define HACCIFS_Export ACE_Proper_Import_Flag
#    define HACCIFS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define HACCIFS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* HACCIFS_BUILD_DLL */
#else /* HACCIFS_HAS_DLL == 1 */
#  define HACCIFS_Export
#  define HACCIFS_SINGLETON_DECLARATION(T)
#  define HACCIFS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* HACCIFS_HAS_DLL == 1 */

// Set HACCIFS_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (HACCIFS_NTRACE)
#  if (ACE_NTRACE == 1)
#    define HACCIFS_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define HACCIFS_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !HACCIFS_NTRACE */

#if (HACCIFS_NTRACE == 1)
#  define HACCIFS_TRACE(X)
#else /* (HACCIFS_NTRACE == 1) */
#  define HACCIFS_TRACE(X) ACE_TRACE _IMPL(X)
#endif /* (HACCIFS_NTRACE == 1) */


