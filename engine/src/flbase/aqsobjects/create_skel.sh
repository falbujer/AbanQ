#!/bin/bash

CLASS=$1
BASE_CLASS=$2
TYPE_CLASS=$3
MACRO_DECL=""

CLASS_UPPER=$(echo $CLASS | awk '{ print toupper($1) }')
CLASS_LOWER=$(echo $CLASS | awk '{ print tolower($1) }')
INCLUDE_CLASS="#include <q${CLASS_LOWER}.h>"

if [ "$TYPE_CLASS" == "" ]; then
  case $BASE_CLASS in
    Object)
      TYPE_CLASS="AQSOBJECT"
    ;;
    VoidPtr)
      TYPE_CLASS="AQSVOIDPTR"
    ;;
    *)
      TYPE_CLASS="AQSWIDGET"
    ;;
  esac
fi

case $TYPE_CLASS in
  AQSVOIDPTR)
    MACRO_DECL="AQ_DECLARE_AQS_VOID"
  ;;
  AQOBJECT | AQWIDGET)
    MACRO_DECL="AQ_DECLARE_AQS_AQOBJECT"
    INCLUDE_CLASS="#include \"AQObjects.h\""
  ;;
  AQOBJECT_VOID)
    MACRO_DECL="AQ_DECLARE_AQS_VOID_AQOBJECT"
    INCLUDE_CLASS="#include \"AQObjects.h\""
  ;;
  AQABSTRACT)
    MACRO_DECL="AQ_SKIP_DECLARE"
    INCLUDE_CLASS=""
    TYPE_CLASS="AQSOBJECT"
  ;;
  AQABSTRACT_VOID)
    MACRO_DECL="AQ_SKIP_DECLARE"
    INCLUDE_CLASS=""
    TYPE_CLASS="AQSVOIDPTR"
  ;;
  *)
    MACRO_DECL="AQ_DECLARE_AQS_OBJECT"
  ;;
esac

cat > AQS${CLASS}_p.h <<EOF
/***************************************************************************
 AQS${CLASS}_p.h
 -------------------
 begin                : $(date +%d/%m/%Y)
 copyright            : (C) 2003-$(date +%Y) by InfoSiAL S.L.
 email                : mail@infosial.com
 ***************************************************************************/
/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 ***************************************************************************/
/***************************************************************************
 Este  programa es software libre. Puede redistribuirlo y/o modificarlo
 bajo  los  términos  de  la  Licencia  Pública General de GNU   en  su
 versión 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#ifndef AQS${CLASS_UPPER}_P_H_
#define AQS${CLASS_UPPER}_P_H_

#include "AQS${BASE_CLASS}_p.h"
${INCLUDE_CLASS}

class AQS${CLASS} : public AQS${BASE_CLASS}
{
  Q_OBJECT

  ${MACRO_DECL}(${CLASS}, ${BASE_CLASS});
  
//@AQ_BEGIN_DEF_PUB_SLOTS@
//@AQ_END_DEF_PUB_SLOTS@
};

//@AQ_BEGIN_IMP_PUB_SLOTS@
//@AQ_END_IMP_PUB_SLOTS@

#endif /* AQS${CLASS_UPPER}_P_H_ */
// @${TYPE_CLASS}@
EOF

