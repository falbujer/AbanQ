/***************************************************************************
 AQGlobal.h
 -------------------
 begin                : 09/10/2012
 copyright            : (C) 2003-2012 by InfoSiAL S.L.
 email                : mail@infosial.com
 ***************************************************************************/
/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 ***************************************************************************/
/***************************************************************************
 Este  programa es software libre. Puede redistribuirlo y/o modificarlo
 bajo  los  t?rminos  de  la  Licencia  P?blica General de GNU   en  su
 versi?n 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#ifndef AQGLOBAL_H_
#define AQGLOBAL_H_

#if defined(Q_OS_WIN32) && !defined(AQ_NO_DLL)
#  if defined(AQ_DLL)
#    define AQ_EXPORT __declspec(dllexport)
#  else
#    define AQ_EXPORT __declspec(dllimport)
#  endif
#else
#  define AQ_EXPORT
#endif

#endif /* AQGLOBAL_H_ */
