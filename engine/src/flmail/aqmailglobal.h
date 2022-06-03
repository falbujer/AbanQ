/***************************************************************************
 aqmailglobal.h
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
 bajo  los  t�rminos  de  la  Licencia  P�blica General de GNU   en  su
 versi�n 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#ifndef AQMAILGLOBAL_H_
#define AQMAILGLOBAL_H_

#if defined(Q_OS_WIN32) && !defined(AQMAIL_NO_DLL)
#  if defined(AQMAIL_DLL)
#    define AQMAIL_EXPORT __declspec(dllexport)
#  else
#    define AQMAIL_EXPORT __declspec(dllimport)
#  endif
#else
#  define AQMAIL_EXPORT
#endif

#endif /* AQMAILGLOBAL_H_ */
