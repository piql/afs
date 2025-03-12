#ifndef AFS_IO_H
#define AFS_IO_H

/*****************************************************************************
**
**  Storage of platform dependent macros.
**
**  Creation date:  2017/07/04
**  Created by:     Piql AS
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define _OPEN_SYS_ITOA_EXT

#if defined (D_OS_WIN32)
#define STRING_TO_LONG_INT(string) _atoi64(string) 
#else
#define STRING_TO_LONG_INT(string) atoll(string) 
#endif

#ifdef __GNUC__
#ifdef __x86_64__
#define AFS_ROL(x,n) rotl(x,n)
#define AFS_ROLc(x,n) AFS_ROL(x,n)
#else
#define AFS_ROL(x,n) rotl(x,n)
#define AFS_ROLc(x,n) AFS_ROL(x,n)
#endif
#else
#define AFS_ROL(x,n) _lrotl(x,n)
#define AFS_ROLc(x,n) _lrotl(x,n)
#endif

#if defined ( D_OS_WIN32 )
#define DFSEEK _fseeki64
#define DFTELL _ftelli64
#else
#define DFSEEK fseeko
#define DFTELL ftello
#endif



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_IO_H
