// ------------------------------
#pragma once


#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (HAPYTHONBOOTSTRAP_HAS_DLL)
#  define HAPYTHONBOOTSTRAP_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && ! HAPYTHONBOOTSTRAP_HAS_DLL */

#if !defined (HAPYTHONBOOTSTRAP_HAS_DLL)
#  define HAPYTHONBOOTSTRAP_HAS_DLL 1
#endif /* ! HAPYTHONBOOTSTRAP_HAS_DLL */

#if defined (HAPYTHONBOOTSTRAP_HAS_DLL) && (HAPYTHONBOOTSTRAP_HAS_DLL == 1)
#  if defined (HAPYTHONBOOTSTRAP_BUILD_DLL)
#    define HAPYTHONBOOTSTRAP_Export ACE_Proper_Export_Flag
#    define HAPYTHONBOOTSTRAP_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define HAPYTHONBOOTSTRAP_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* HAPYTHONBOOTSTRAP_BUILD_DLL */
#    define HAPYTHONBOOTSTRAP_Export ACE_Proper_Import_Flag
#    define HAPYTHONBOOTSTRAP_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define HAPYTHONBOOTSTRAP_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* HAPYTHONBOOTSTRAP_BUILD_DLL */
#else /* HAPYTHONBOOTSTRAP_HAS_DLL == 1 */
#  define HAPYTHONBOOTSTRAP_Export
#  define HAPYTHONBOOTSTRAP_SINGLETON_DECLARATION(T)
#  define HAPYTHONBOOTSTRAP_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* HAPYTHONBOOTSTRAP_HAS_DLL == 1 */

// Set HAPYTHONBOOTSTRAP_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (HAPYTHONBOOTSTRAP_NTRACE)
#  if (ACE_NTRACE == 1)
#    define HAPYTHONBOOTSTRAP_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define HAPYTHONBOOTSTRAP_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !HAPYTHONBOOTSTRAP_NTRACE */

#if (HAPYTHONBOOTSTRAP_NTRACE == 1)
#  define HAPYTHONBOOTSTRAP_TRACE(X)
#else /* (HAPYTHONBOOTSTRAP_NTRACE == 1) */
#  define HAPYTHONBOOTSTRAP_TRACE(X) ACE_TRACE _IMPL(X)
#endif /* (HAPYTHONBOOTSTRAP_NTRACE == 1) */


