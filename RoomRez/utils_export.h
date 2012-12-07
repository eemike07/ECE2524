#ifndef UTILS_EXPORT_H
#define UTILS_EXPORT_H

#include <qglobal.h>
/** This macro is only for building DLLs on windows. */
#ifndef Q_OS_WIN
#define UTILS_EXPORT
#elif defined(UTILS_DLL)
#define UTILS_EXPORT Q_DECL_EXPORT
#else
#define UTILS_EXPORT Q_DECL_IMPORT
#endif

#endif        //  #ifndef MYEXPORT_H

