#!/bin/bash

cat > AQS_g.h <<EOF
/***************************************************************************
 AQS_g.h
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

#ifndef AQS_G_H_
#define AQS_G_H_

EOF

EXCLUDE="AQSWrapperFactory_p.h AQSObjectFactory_p.h AQSVoidPtr_p.h AQSEvent_p.h AQSObject_p.h"

echo "#include \"AQSVoidPtr_p.h\"" >> AQS_g.h
echo "#include \"AQSEvent_p.h\"" >> AQS_g.h
echo "#include \"AQSObject_p.h\"" >> AQS_g.h
for i in $(ls AQS?*_p.h)
do
  if [ "$EXCLUDE" == "${EXCLUDE/$i/}" ]
  then
     echo "#include \"$i\"" >> AQS_g.h
  fi
done

EXCLUDE="AQSWrapperFactory_p.h AQSObjectFactory_p.h AQSVoidPtr_p.h AQSSProject_p.h AQSApplication_p.h AQSAbstractMemArray_p.h AQSSInterpreter_p.h"

echo "" >> AQS_g.h
echo "#define  AQ_GEN_REG_WRAP \\" >> AQS_g.h
for i in $(ls AQS?*_p.h)
do
  if [ "$EXCLUDE" == "${EXCLUDE/$i/}" ]
  then
    if [ "@AQOBJECT_VOID@" == "$(grep -o '@AQOBJECT_VOID@' $i)" ]
    then
      SUT="${i/AQS/}"
      echo "         AQ_REG_WRAP_AQOBJECT(${SUT/_p.h/}); \\" >> AQS_g.h
      continue
    fi
    
    if [ "@AQOBJECT@" == "$(grep -o '@AQOBJECT@' $i)" ]
    then
      SUT="${i/AQS/}"
      echo "         AQ_REG_WRAP_AQOBJECT(${SUT/_p.h/}); \\" >> AQS_g.h
      continue
    fi
    
    if [ "@AQWIDGET@" == "$(grep -o '@AQWIDGET@' $i)" ]
    then
      SUT="${i/AQS/}"
      echo "         AQ_REG_WRAP_AQOBJECT(${SUT/_p.h/}); \\" >> AQS_g.h
      continue
    fi
    
    SUT="${i/AQS/}"
    echo "         AQ_REG_WRAP(${SUT/_p.h/}); \\" >> AQS_g.h
  fi
done

EXCLUDE="AQSWrapperFactory_p.h AQSObjectFactory_p.h AQSVoidPtr_p.h AQSSProject_p.h AQSApplication_p.h AQSAbstractMemArray_p.h AQSSInterpreter_p.h"

echo "" >> AQS_g.h
echo "#define  AQ_GEN_CRE_WRAP \\" >> AQS_g.h
for i in $(ls AQS?*_p.h)
do
  if [ "$EXCLUDE" == "${EXCLUDE/$i/}" ]
  then
    if [ "@AQSVOIDPTR@" == "$(grep -o '@AQSVOIDPTR@' $i)" ]
    then
      SUT="${i/AQS/}"
      echo "         AQ_CRE_WRAP_VOIDPTR(${SUT/_p.h/}); \\" >> AQS_g.h
      continue
    fi

    if [ "@AQOBJECT_VOID@" == "$(grep -o '@AQOBJECT_VOID@' $i)" ]
    then
      SUT="${i/AQS/}"
      echo "         AQ_CRE_WRAP_VOIDPTR_AQOBJECT(${SUT/_p.h/}); \\" >> AQS_g.h
      continue
    fi
    
    if [ "@AQOBJECT@" == "$(grep -o '@AQOBJECT@' $i)" ]
    then
      SUT="${i/AQS/}"
      echo "         AQ_CRE_WRAP_AQOBJECT(${SUT/_p.h/}); \\" >> AQS_g.h
      continue
    fi
    
    if [ "@AQWIDGET@" == "$(grep -o '@AQWIDGET@' $i)" ]
    then
      SUT="${i/AQS/}"
      echo "         AQ_CRE_WRAP_AQOBJECT(${SUT/_p.h/}); \\" >> AQS_g.h
      continue
    fi
    
    SUT="${i/AQS/}"
    echo "         AQ_CRE_WRAP_OBJECT(${SUT/_p.h/}); \\" >> AQS_g.h
  fi
done

EXCLUDE="AQSWrapperFactory_p.h AQSObjectFactory_p.h AQSVoidPtr_p.h AQSSProject_p.h AQSSScript_p.h AQSApplication_p.h AQSEventLoop_p.h AQSAbstractMemArray_p.h"
EXCLUDE="$EXCLUDE AQSSqlDatabase_p.h AQSManager_p.h AQSManagerModules_p.h AQSSInterpreter_p.h"

echo "" >> AQS_g.h
echo "#define  AQ_GEN_REG_CLASS \\" >> AQS_g.h
for i in $(ls AQS?*_p.h)
do
  if [ "$EXCLUDE" == "${EXCLUDE/$i/}" ]
  then
    if [ "@AQOBJECT_VOID@" == "$(grep -o '@AQOBJECT_VOID@' $i)" ]
    then
      SUT="${i/AQS/}"
      echo "         AQ_REG_CLASS_AQOBJECT(${SUT/_p.h/}); \\" >> AQS_g.h
      continue
    fi
    
    if [ "@AQOBJECT@" == "$(grep -o '@AQOBJECT@' $i)" ]
    then
      SUT="${i/AQS/}"
      echo "         AQ_REG_CLASS_AQOBJECT(${SUT/_p.h/}); \\" >> AQS_g.h
      continue
    fi
    
    if [ "@AQWIDGET@" == "$(grep -o '@AQWIDGET@' $i)" ]
    then
      SUT="${i/AQS/}"
      echo "         AQ_REG_CLASS_AQOBJECT(${SUT/_p.h/}); \\" >> AQS_g.h
      continue
    fi
    
    SUT="${i/AQS/}"
    echo "         AQ_REG_CLASS(${SUT/_p.h/}); \\" >> AQS_g.h
  fi
done

EXCLUDE="AQSWrapperFactory_p.h AQSObjectFactory_p.h AQSVoidPtr_p.h AQSSProject_p.h AQSSScript_p.h AQSApplication_p.h AQSEventLoop_p.h AQSAbstractMemArray_p.h"
EXCLUDE="$EXCLUDE AQSSqlDatabase_p.h AQSManager_p.h AQSManagerModules_p.h AQSSInterpreter_p.h"

echo "" >> AQS_g.h
echo "#define  AQ_GEN_CRE_CLASS \\" >> AQS_g.h
for i in $(ls AQS?*_p.h)
do
  if [ "$EXCLUDE" == "${EXCLUDE/$i/}" ]
  then
    if [ "@AQSVOIDPTR@" == "$(grep -o '@AQSVOIDPTR@' $i)" ]
    then
      SUT="${i/AQS/}"
      echo "         AQ_CRE_VOIDPTR(${SUT/_p.h/}); \\" >> AQS_g.h
      continue
    fi
    
    if [ "@AQOBJECT_VOID@" == "$(grep -o '@AQOBJECT_VOID@' $i)" ]
    then
      SUT="${i/AQS/}"
      echo "         AQ_CRE_VOIDPTR_AQOBJECT(${SUT/_p.h/}); \\" >> AQS_g.h
      continue
    fi
    
    if [ "@AQOBJECT@" == "$(grep -o '@AQOBJECT@' $i)" ]
    then
      SUT="${i/AQS/}"
      echo "         AQ_CRE_AQOBJECT(${SUT/_p.h/}); \\" >> AQS_g.h
      continue
    fi
    
    if [ "@AQSWIDGET@" == "$(grep -o '@AQSWIDGET@' $i)" ]
    then
      SUT="${i/AQS/}"
      echo "         AQ_CRE_WIDGET(${SUT/_p.h/}); \\" >> AQS_g.h
      continue
    fi

    if [ "@AQWIDGET@" == "$(grep -o '@AQWIDGET@' $i)" ]
    then
      SUT="${i/AQS/}"
      echo "         AQ_CRE_AQWIDGET(${SUT/_p.h/}); \\" >> AQS_g.h
      continue
    fi
    
    SUT="${i/AQS/}"
    echo "         AQ_CRE_OBJECT(${SUT/_p.h/}); \\" >> AQS_g.h
  fi
done

echo "" >> AQS_g.h
echo "#endif /*AQS_G_H_*/" >> AQS_g.h

