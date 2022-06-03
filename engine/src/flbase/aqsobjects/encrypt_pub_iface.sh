#!/bin/bash

CLASS="AQS$1"
Q_CLASS="Q$1"
INPUT=${CLASS}_p.i
INPUT_H=${CLASS}_p.h
OUT_DEF=${CLASS}_p.i.def
OUT_IMP=${CLASS}_p.i.imp
ID_PATT='[[:alnum:]|_|$]\+'

if [ "@AQOBJECT@" == "$(grep -o '@AQOBJECT@' ${INPUT_H})" ]; then
  Q_CLASS="AQ$1"
fi
if [ "@AQOBJECT_VOID@" == "$(grep -o '@AQOBJECT_VOID@' ${INPUT_H})" ]; then
  Q_CLASS="AQ$1"
fi
if [ "@AQWIDGET@" == "$(grep -o '@AQWIDGET@' ${INPUT_H})" ]; then
  Q_CLASS="AQ$1"
fi

function firstChar() {
  echo "$1" | grep -o '^.'
}

function firstTwoChar() {
  echo "$1" | grep -o '^..'
}

function lastChar() {
  echo "$*" | grep -o '.$'
}

function isTypePtr() {
  if [ "$1" == "${1/\*/}" ]; then
    echo 0
  else
    echo 1
  fi
}

function isTypeEnum() {
  if [ "$1" == "${1/::/}" ]; then
    echo 0
  else
    echo 1
  fi
}

function parsCount() {
  if [ "$1" == "" ]; then
    echo 0
  else
    PARS_COUNT=$(echo $((`echo $1|sed 's/[^,]//g'|wc -m` -1)))
    PARS_COUNT=$(expr $PARS_COUNT + 1)
    echo $PARS_COUNT
  fi
}

function parsToArray() {
  local PARS=$1
  local PARS_COUNT=$(parsCount "$PARS")
  local OUT_ARRAY_NAME=$2

  local -a O_ARRAY=( "$@" )
  local i=0
  for(( i=0; i < $PARS_COUNT; i+=1 )); do
    local PAR=${PARS%%\,*}
    O_ARRAY[$i]="$PAR"
    PARS=${PARS#*\,}
  done

  local output=$(declare -p O_ARRAY | sed -e 's/^declare -a O_ARRAY=//')
  echo "declare -a $OUT_ARRAY_NAME=${output}"
}

function grepParTypes() {
  F_PARS=${1%)*}
  F_PARS=${F_PARS#*\(}
  F_PARS=${F_PARS#const }
  F_PARS=${F_PARS// const /}
  F_PARS=${F_PARS//\,const /}
  F_PARS=${F_PARS//\&/}
  F_PARS=${F_PARS//char\*/QString}
  F_PARS=$(echo $F_PARS | sed "s/\([^,]\) $ID_PATT,/\1,/g")
  F_PARS=$(echo $F_PARS | sed "s/\([^,]\) $ID_PATT=[-|+|*|&|[:alnum:]|_|$|:]\+/\1=/g")
  F_PARS=$(echo $F_PARS | sed "s/\([^,]\) $ID_PATT$/\1/")
  F_PARS=${F_PARS// /}
  echo $F_PARS
}

function grepType() {
  F_TYPE=$(echo $1 | sed "s/\($ID_PATT\)=.*/\1/")
  F_TYPE=${F_TYPE// /}
  echo $F_TYPE
}

function ignoreLine() {
  IG_RET=$(echo $1 | grep -o '^[#|@|~]')
  if [ "$IG_RET" == "" ]; then
    echo 0
  fi
  echo 1
}

function formatArgClassAQ() {
  PAR=${1%\*}
  AQSFILE="AQS${PAR:2}_p.h"
  if [ ! -r $AQSFILE ]; then
    echo "no"
    return 0
  fi
  if [ "@AQ_PTR_INDIRECTION@" == "$(grep -o '@AQ_PTR_INDIRECTION@' ${AQSFILE})" ]; then
    echo "*${2}"
    return 0
  fi
  echo "no"
}

function formatArgClass() {
  if [ "$(firstTwoChar $1)" == "AQ" ]; then
    echo $(formatArgClassAQ $1 $2)
    return 0
  fi
  if [ "$(firstChar $1)" != "Q" ]; then
    echo "no"
    return 0
  fi
  PAR=${1%\*}
  AQSFILE="AQS${PAR:1}_p.h"
  if [ ! -r $AQSFILE ]; then
    echo "no"
    return 0
  fi
  if [ "@AQ_PTR_INDIRECTION@" == "$(grep -o '@AQ_PTR_INDIRECTION@' ${AQSFILE})" ]; then
    echo "*${2}"
    return 0
  fi
  echo "no"
}

function formatParClassAQ() {
  local PAR=${1%\*}
  local HAS_DEFV=$(echo $PAR | grep -o '=')
  if [ "$HAS_DEFV" == "=" ]; then
    PAR=$(echo $PAR | sed "s/\(.*\)=[-|+|*|&|[:alnum:]|_|$|:]\+/\1/g")
  fi
  AQSFILE="AQS${PAR:2}_p.h"
  if [ ! -r $AQSFILE ]; then
    echo "no"
    return 0
  fi
  if [ "@AQOBJECT_VOID@" == "$(grep -o '@AQOBJECT_VOID@' ${AQSFILE})" ]; then
    if [ "$HAS_DEFV" == "=" ]; then
      local DEFV=$(echo $1 | sed "s/.*\(=[-|+|*|&|[:alnum:]|_|$|:]\+\)/\1/g")
      echo "${PAR:2}${DEFV}"
    else
      echo "${PAR:2}*"
    fi
    return 0
  fi
  echo "no"
}

function formatParClass() {
  if [ "$(firstTwoChar $1)" == "AQ" ]; then
    echo $(formatParClassAQ $1)
    return 0
  fi
  local PAR=${1%\*}
  local HAS_DEFV=$(echo $PAR | grep -o '=')
  if [ "$HAS_DEFV" == "=" ]; then
    PAR=$(echo $PAR | sed "s/\(.*\)=[-|+|*|&|[:alnum:]|_|$|:]\+/\1/g")
  fi
  if [ "$(firstChar $PAR)" != "Q" ]; then
    echo "no"
    return 0
  fi
  AQSFILE="AQS${PAR:1}_p.h"
  if [ ! -r $AQSFILE ]; then
    echo "no"
    return 0
  fi
  if [ "@AQSVOIDPTR@" == "$(grep -o '@AQSVOIDPTR@' ${AQSFILE})" ]; then
    if [ "$HAS_DEFV" == "=" ]; then
      local DEFV=$(echo $1 | sed "s/.*\(=[-|+|*|&|[:alnum:]|_|$|:]\+\)/\1/g")
      echo "${PAR:1}${DEFV}"
    else
      echo "${PAR:1}*"
    fi
    return 0
  fi
  echo "no"
}

tr '\r' '\n' < ${INPUT} > ${INPUT}.tmp
mv -f ${INPUT}.tmp ${INPUT}

exec<${INPUT}
while read line; do
  if [ "$(ignoreLine $line)" == "1" ]; then
    echo $line >> ${INPUT}.tmp
    continue
  fi
  if [ "$(lastChar $line)" != ";" ]; then
      echo "${line};" >> ${INPUT}.tmp
  else
      echo ${line} >> ${INPUT}.tmp
  fi
done
mv -f ${INPUT}.tmp ${INPUT}

astyle --align-pointer=type ${INPUT}
rm -f ${INPUT}.orig

sed '/^$/d' ${INPUT} > ${INPUT}.tmp
mv -f ${INPUT}.tmp ${INPUT}

sed 's/^static //' ${INPUT} > ${INPUT}.tmp
mv -f ${INPUT}.tmp ${INPUT}

sed 's/^inline //' ${INPUT} > ${INPUT}.tmp
mv -f ${INPUT}.tmp ${INPUT}

sed 's/[[:space:]]\+/ /' ${INPUT} > ${INPUT}.tmp
mv -f ${INPUT}.tmp ${INPUT}

sed 's/[[:space:]]\+,/,/' ${INPUT} > ${INPUT}.tmp
mv -f ${INPUT}.tmp ${INPUT}

sed 's/[[:space:]]\+=[[:space:]]\+/=/g' ${INPUT} > ${INPUT}.tmp
mv -f ${INPUT}.tmp ${INPUT}

exec<${INPUT}
while read line; do
  if [ "$(ignoreLine $line)" == "1" ]; then
    echo $line >> ${INPUT}.tmp
    continue
  fi
  
  F_VIRTUAL=$(echo $line | grep -o "^virtual ")
  F_CONST=$(echo $line | grep -o "^${F_VIRTUAL}const ")
  if [ "$F_CONST" != "" ]; then
    F_CONST="const "
  fi
  F_TYPE=$(echo $line | sed "s/^${F_VIRTUAL}${F_CONST}\([_]*[[:alpha:]]\+[\*|&]*\) $ID_PATT(.*$/\1/")
  if [ "$F_TYPE" == "$line" ]; then
    echo "@${line}" >> ${INPUT}.tmp
  else
    echo $line >> ${INPUT}.tmp
  fi
done
mv -f ${INPUT}.tmp ${INPUT}


function dumpDef() {
  local F_VIRTUAL=$1
  local F_CONST=$2
  local F_TYPE=$3
  local F_NAME=$4
  local F_PARS=$5
  local F_ENDL=$6
  local F_POS=$7
  local F_PARS_COUNT=$(parsCount "$F_PARS")

  if [ $F_POS -eq $F_PARS_COUNT ]; then
    echo "${F_VIRTUAL}${F_CONST}$F_TYPE $F_NAME($F_PARS)$F_ENDL" >> $OUT_DEF
    return 0
  fi
  
  local -a PARS_ARRAY=()
  eval $(parsToArray "$F_PARS" PARS_ARRAY)

  local F_PAR=${PARS_ARRAY[$F_POS]}
  local PAR_TYPE=$(grepParTypes "$F_PAR")
  
  if [ "$(isTypeEnum "$(grepType "$PAR_TYPE")")" == "1" ]; then
    F_PARS=${F_PARS/$(grepType "$PAR_TYPE")/uint};
    F_POS=$(expr $F_POS + 1)
    dumpDef "$F_VIRTUAL" "$F_CONST" "$F_TYPE" "$F_NAME" "$F_PARS" "$F_ENDL" $F_POS
    return 0
  fi
  
  if [ "$(isTypePtr "$PAR_TYPE")" == "0" ]; then
    F_POS=$(expr $F_POS + 1)
    dumpDef "$F_VIRTUAL" "$F_CONST" "$F_TYPE" "$F_NAME" "$F_PARS" "$F_ENDL" $F_POS
    return 0
  fi
  
  if [ "$(firstChar $PAR_TYPE)" == "_" ]; then 
    PAR_TYPE=${PAR_TYPE:1}
  fi
  
  local PAR_CLASS=$(formatParClass "$PAR_TYPE")
  if [ "$PAR_CLASS" != "no" ]; then
    local AQS_TMP_PARS=""
    local Q_TMP_PARS=""
    local i=0
    for (( i=0 ; i < $F_POS ; i+=1 )); do
      local TMP_PAR=${PARS_ARRAY[$i]}
      if [ "$AQS_TMP_PARS" != "" ]; then
        AQS_TMP_PARS="$AQS_TMP_PARS,"
        Q_TMP_PARS="$Q_TMP_PARS,"
      fi
      AQS_TMP_PARS="${AQS_TMP_PARS}${TMP_PAR}"
      Q_TMP_PARS="${Q_TMP_PARS}${TMP_PAR}"
    done

    if [ "$AQS_TMP_PARS" != "" ]; then
      AQS_TMP_PARS="$AQS_TMP_PARS,"
      Q_TMP_PARS="$Q_TMP_PARS,"
    fi
    AQS_TMP_PARS="${AQS_TMP_PARS}AQS${PAR_CLASS}"
    if [ "$(firstTwoChar $PAR_TYPE)" == "AQ" ]; then
      Q_TMP_PARS="${Q_TMP_PARS}AQ${PAR_CLASS}"
    else
      Q_TMP_PARS="${Q_TMP_PARS}Q${PAR_CLASS}"
    fi
    
    F_POS=$(expr $F_POS + 1)
    for (( i=$F_POS ; i < $F_PARS_COUNT ; i+=1 )); do
      local TMP_PAR=${PARS_ARRAY[$i]}
      if [ "$AQS_TMP_PARS" != "" ]; then
        AQS_TMP_PARS="$AQS_TMP_PARS,"
        Q_TMP_PARS="$Q_TMP_PARS,"
      fi
      AQS_TMP_PARS="${AQS_TMP_PARS}${TMP_PAR}"
      Q_TMP_PARS="${Q_TMP_PARS}${TMP_PAR}"
    done

    dumpDef "$F_VIRTUAL" "$F_CONST" "$F_TYPE" "$F_NAME" "$Q_TMP_PARS" "$F_ENDL" $F_POS
    dumpDef "$F_VIRTUAL" "$F_CONST" "$F_TYPE" "$F_NAME" "$AQS_TMP_PARS" "$F_ENDL" $F_POS
    
    return 0
  fi

  F_POS=$(expr $F_POS + 1)
  dumpDef "$F_VIRTUAL" "$F_CONST" "$F_TYPE" "$F_NAME" "$F_PARS" "$F_ENDL" $F_POS
}

exec<${INPUT}
while read line; do
  if [ "$(ignoreLine $line)" == "1" ]; then
    continue
  fi
      
  F_VIRTUAL=$(echo $line | grep -o '^virtual ')
  F_CONST=$(echo $line | grep -o "^${F_VIRTUAL}const ")
  if [ "$F_CONST" != "" ]; then
    F_CONST="const "
  fi
  F_TYPE=$(echo $line | sed "s/^${F_VIRTUAL}${F_CONST}\([_]*[[:alpha:]]\+[\*|&]*\) $ID_PATT(.*$/\1/")
  F_NAME=$(echo $line | sed "s/^${F_VIRTUAL}${F_CONST}${F_TYPE/\*/}[\*]* \($ID_PATT\)(.*$/\1/") 
  F_PARS=${line%)*}
  F_PARS=${F_PARS#*\(}
  F_PARS=$(echo $F_PARS | sed "s/\([^,]\) $ID_PATT,/\1,/g")
  F_PARS=$(echo $F_PARS | sed "s/\([^,]\) $ID_PATT=/\1 =/g")
  F_PARS=$(echo $F_PARS | sed "s/\([^,]\) $ID_PATT$/\1/")
  F_ENDL=${line#*)}

  if [ "$(firstChar $F_TYPE)" == "_" ]; then 
    F_TYPE=${F_TYPE:1}
  fi
  
  dumpDef "$F_VIRTUAL" "$F_CONST" "$F_TYPE" "$F_NAME" "$F_PARS" "$F_ENDL" 0
done

function candidateTable() {
  local F_PARS=$1
  
  if [ ! -w candidates.table ]; then
    echo $F_PARS > candidates.table
    echo "0"
    return 0
  fi
  
  F_PARS_ESC=${F_PARS//\*/\\*}
  IDX_TABLE=$(grep -n "^${F_PARS_ESC}$" candidates.table)
  if [ "$IDX_TABLE" == "" ]; then
    echo $F_PARS >> candidates.table
  fi
  IDX_TABLE=$(grep -n "^${F_PARS_ESC}$" candidates.table)
  
  IDX_TABLE=$(echo "$IDX_TABLE" | sed 's/\([:digit]*\):.*/\1/')
  IDX_TABLE=$(expr $IDX_TABLE - 1)
  echo "$IDX_TABLE"
}

function dumpConstruct() {
  local F_PARS=$1
  
  if [ "$F_PARS" == "" ]; then
    echo "candidates[0].append(QString::null);" >> ${OUT_DEF}.cdt
    echo "if (sgt.isEmpty())" >> ${OUT_DEF}.swt
    echo "return new ${Q_CLASS};" >> ${OUT_DEF}.swt
    return 0
  fi

  local F_PARS_COUNT=$(parsCount "$F_PARS")
  
  if [ "$(lastChar $F_PARS)" == "=" ]; then
    if [ $F_PARS_COUNT -gt 1 ]; then
      dumpConstruct ${F_PARS%\,*}
    else
      dumpConstruct ""
    fi
    F_PARS=${F_PARS//=/}
  fi  
  
  IDX_CANDIDATE_TABLE=$(candidateTable $F_PARS)
  echo "candidates[${F_PARS_COUNT}].append(QString::fromLatin1(candidatesTable[${IDX_CANDIDATE_TABLE}]));" >> ${OUT_DEF}.cdt
  echo "if (sgt == QString::fromLatin1(candidatesTable[${IDX_CANDIDATE_TABLE}]))" >> ${OUT_DEF}.swt
  
  local -a PARS_ARRAY=()
  eval $(parsToArray "$F_PARS" PARS_ARRAY)
  
  local F_PARS=""
  local F_ARGS=""
  local i=0
  for (( i=0 ; i < $F_PARS_COUNT ; i+=1 )); do
    local F_PAR=${PARS_ARRAY[$i]}

    if [ "$F_ARGS" != "" ]; then
      F_ARGS="$F_ARGS,\n"
    fi
    if [ "$F_PARS" != "" ]; then
      F_PARS="$F_PARS,"
    fi
    if [ "$F_PAR" == "" ]; then
      F_ARGS="${F_ARGS}0"
      continue
    fi

    local F_ARG="args[$i]"
    if [ "$(isTypePtr "$F_PAR")" == "1" ]; then
      local ARG_VALUE="argValue<${F_PAR}>(${F_ARG})"
      local ARG_CLASS=$(formatArgClass ${F_PAR} ${ARG_VALUE})
      if [ "$ARG_CLASS" != "no" ]; then
        F_ARGS="${F_ARGS}${ARG_CLASS}"
      else
        F_ARGS="${F_ARGS}${ARG_VALUE}"
      fi
    else
      case ${F_PAR} in
        double)
        F_ARGS="${F_ARGS}${F_ARG}.variant().toDouble()"
        ;;
        bool)
        F_ARGS="${F_ARGS}${F_ARG}.variant().toBool()"
        ;;
        int)
        F_ARGS="${F_ARGS}${F_ARG}.variant().toInt()"
        ;;
        uint)
        F_ARGS="${F_ARGS}${F_ARG}.variant().toUInt()"
        ;;
        *::* | WFlags)
        F_ARGS="${F_ARGS}static_cast<${F_PAR}>(${F_ARG}.variant().toUInt())"
        ;;
        *)
        F_ARGS="${F_ARGS}*(argValue<${F_PAR}*>(${F_ARG}))"
        ;;
      esac
    fi
    
    F_PARS="${F_PARS}${F_PAR}"
  done

  echo -e "return new ${Q_CLASS}(${F_ARGS});" >> ${OUT_DEF}.swt
}

exec<${INPUT}
while read line; do
  if [ "$(firstChar $line)" != "@" ]; then
    continue
  fi
  F_PARS=$(grepParTypes "$line")
  dumpConstruct $F_PARS
done

echo "" >> $OUT_DEF
echo "protected:" >> $OUT_DEF
echo "static void *construct(const QSArgumentList &args) {" >> $OUT_DEF

if [ -r ${OUT_DEF}.cdt ]; then
  echo "QMap<int, QStringList> candidates;" >> $OUT_DEF
  cat ${OUT_DEF}.cdt >> $OUT_DEF
  echo "QString sgt(castArgsSignature(args, candidates));" >> $OUT_DEF
  cat ${OUT_DEF}.swt >> $OUT_DEF
fi

echo "return 0; }" >> $OUT_DEF

if [ -r ${OUT_DEF}.cdt ]; then
  echo "" >> $OUT_DEF
  echo "public:" >> $OUT_DEF
  echo "static QMap<int, QStringList> candidateConstructors() {" >> $OUT_DEF
  echo "QMap<int, QStringList> candidates;" >> $OUT_DEF
  cat ${OUT_DEF}.cdt >> $OUT_DEF
  echo "return candidates; }" >> $OUT_DEF
fi

rm -f ${OUT_DEF}.cdt
rm -f ${OUT_DEF}.swt

function dumpImp() {
  local F_VIRTUAL=$1
  local F_CONST=$2
  local F_TYPE=$3
  local F_NAME=$4
  local F_PARS=$5
  local F_ENDL=$6
  local F_POS=$7
  local F_PARS_COUNT=$(parsCount "$F_PARS")
  
  if [ $F_POS -eq $F_PARS_COUNT ]; then
    local -a TMP_PARS_ARRAY=()
    eval $(parsToArray "$F_PARS" TMP_PARS_ARRAY)

    local TMP_PARS=""
    local TMP_ARGS=""
    local i=0
    set -f
    for (( i=0 ; i < $F_PARS_COUNT ; i+=1 )); do
      if [ "$TMP_PARS" != "" ]; then
        TMP_PARS="$TMP_PARS, "
        TMP_ARGS="$TMP_ARGS, "
      fi

      local F_PAR=${TMP_PARS_ARRAY[$i]}
      local F_ARG="arg${i}"

      if [ "$(isTypeEnum "$(grepType "$F_PAR")")" == "1" ]; then
        local TMP_PAR_TYPE=$(grepType "$F_PAR")
        F_PAR=${F_PAR/$TMP_PAR_TYPE/uint}
        TMP_ARGS="${TMP_ARGS}static_cast<$TMP_PAR_TYPE>($F_ARG)"
      else
        local ARG_CLASS=$(formatArgClass ${F_PAR} ${F_ARG})
        if [ "$ARG_CLASS" != "no" ]; then
          TMP_ARGS="${TMP_ARGS}${ARG_CLASS}"
        else
          local TMP_PAR="$(grepParTypes "$F_PAR")"
          if [ "${TMP_PAR:0:3}" == "AQS" ]; then
            TMP_ARGS="${TMP_ARGS}*${F_ARG}"
          else
            TMP_ARGS="${TMP_ARGS}${F_ARG}"
          fi
        fi
      fi

      if [ "$(firstChar $F_PAR)" == "_" ]; then
        F_PAR=${F_PAR:2}
      fi
      TMP_PARS="${TMP_PARS}${F_PAR} ${F_ARG}"
    done
    set +f
    
    if [ "$(firstChar $F_TYPE)" == "_" ]; then 
      echo "inline ${F_CONST}${F_TYPE:1} ${CLASS}::$F_NAME($TMP_PARS)${F_ENDL/;/ {}" >> $OUT_IMP
    else
      echo "inline ${F_CONST}$F_TYPE ${CLASS}::$F_NAME($TMP_PARS)${F_ENDL/;/ {}" >> $OUT_IMP  
    fi
    
    if [ "$F_TYPE" == "void" ]; then
      echo "  AQ_CALL_VOID($F_NAME($TMP_ARGS));" >> $OUT_IMP
    else
      if [ "$(isTypePtr "$F_TYPE")" == "0" ]; then
        RET_V="${F_TYPE/&/}"
        echo "  AQ_CALL_RET_V($F_NAME($TMP_ARGS),$RET_V);" >> $OUT_IMP
      else
        if [ "$(formatArgClass ${F_TYPE} void)" != "no" ]; then
          RET_V="${F_TYPE/&/}"
          RET_V="${RET_V/\*/}"
          echo "  AQ_CALL_RET_PTR($F_NAME($TMP_ARGS),$RET_V);" >> $OUT_IMP
        else
          echo "  AQ_CALL_RET($F_NAME($TMP_ARGS));" >> $OUT_IMP
        fi
      fi
    fi
    echo "}" >> $OUT_IMP

    return 0
  fi

  local -a PARS_ARRAY=()
  eval $(parsToArray "$F_PARS" PARS_ARRAY)
  
  local F_PAR=${PARS_ARRAY[$F_POS]}
  local PAR_TYPE=$(grepParTypes "$F_PAR")
  
  if [ "$(isTypePtr "$PAR_TYPE")" == "0" ]; then
    F_POS=$(expr $F_POS + 1)
    dumpImp "$F_VIRTUAL" "$F_CONST" "$F_TYPE" "$F_NAME" "$F_PARS" "$F_ENDL" $F_POS
    return 0
  fi

  if [ "$(firstChar $PAR_TYPE)" == "_" ]; then
    PAR_TYPE=${PAR_TYPE:1}
  fi
  
  local PAR_CLASS=$(formatParClass "$PAR_TYPE")
  if [ "$PAR_CLASS" != "no" ]; then
    local TMP_PARS=""
    local i=0
    for (( i=0 ; i < $F_POS ; i+=1 )); do
      local TMP_PAR=${PARS_ARRAY[$i]}
      if [ "$TMP_PARS" != "" ]; then
        TMP_PARS="$TMP_PARS,"
      fi
      TMP_PARS="${TMP_PARS}${TMP_PAR}"
    done

    if [ "$TMP_PARS" != "" ]; then
      TMP_PARS="$TMP_PARS,"
    fi
    TMP_PARS="${TMP_PARS}AQS${PAR_CLASS}"

    F_POS=$(expr $F_POS + 1)
    for (( i=$F_POS ; i < $F_PARS_COUNT ; i+=1 )); do
      local TMP_PAR=${PARS_ARRAY[$i]}
      if [ "$TMP_PARS" != "" ]; then
        TMP_PARS="$TMP_PARS,"
      fi
      TMP_PARS="${TMP_PARS}${TMP_PAR}"
    done

    dumpImp "$F_VIRTUAL" "$F_CONST" "$F_TYPE" "$F_NAME" "$F_PARS" "$F_ENDL" $F_POS
    dumpImp "$F_VIRTUAL" "$F_CONST" "$F_TYPE" "$F_NAME" "$TMP_PARS" "$F_ENDL" $F_POS

    return 0
  fi

  F_POS=$(expr $F_POS + 1)
  dumpImp "$F_VIRTUAL" "$F_CONST" "$F_TYPE" "$F_NAME" "$F_PARS" "$F_ENDL" $F_POS
}

exec<${INPUT}
while read line; do
  if [ "$(ignoreLine $line)" == "1" ]; then
    continue
  fi

  F_VIRTUAL=$(echo $line | grep -o '^virtual ')
  F_CONST=$(echo $line | grep -o "^${F_VIRTUAL}const ")
  if [ "$F_CONST" != "" ]; then
    F_CONST="const "
  fi
  F_TYPE=$(echo $line | sed "s/^${F_VIRTUAL}${F_CONST}\([_]*[[:alpha:]]\+[\*|&]*\) .*$/\1/")
  F_NAME=$(echo $line | sed "s/^${F_VIRTUAL}${F_CONST}${F_TYPE/\*/}[\*]* \($ID_PATT\)(.*$/\1/") 
  F_PARS=${line%)*}
  F_PARS=${F_PARS#*\(}
  F_PARS=$(echo $F_PARS | sed "s/\([^,]\) $ID_PATT,/\1,/g")
  F_PARS=$(echo $F_PARS | sed "s/\([^,]\) $ID_PATT=[-|+|*|&|[:alnum:]|_|$|:]\+/\1/g")
  F_PARS=$(echo $F_PARS | sed "s/\([^,]\) $ID_PATT$/\1/")
  F_ENDL=${line#*)}

  dumpImp "$F_VIRTUAL" "$F_CONST" "$F_TYPE" "$F_NAME" "$F_PARS" "$F_ENDL" 0
done

LC_BEGIN_DEF=$(grep -n "@AQ_BEGIN_DEF_PUB_SLOTS@" ${INPUT_H} | sed 's/^\([[:digit:]]\+\):.*/\1/')
LC_END_DEF=$(grep -n "@AQ_END_DEF_PUB_SLOTS@" ${INPUT_H} | sed 's/^\([[:digit:]]\+\):.*/\1/')

head -n $LC_BEGIN_DEF ${INPUT_H} > ${INPUT_H}.tmp
echo "public slots:" >> ${INPUT_H}.tmp
if [ -r  $OUT_DEF ]; then
  cat $OUT_DEF >> ${INPUT_H}.tmp
fi
tail -n +$LC_END_DEF ${INPUT_H} >> ${INPUT_H}.tmp
mv -f ${INPUT_H}.tmp ${INPUT_H}

LC_BEGIN_IMP=$(grep -n "@AQ_BEGIN_IMP_PUB_SLOTS@" ${INPUT_H} | sed 's/^\([[:digit:]]\+\):.*/\1/')
LC_END_IMP=$(grep -n "@AQ_END_IMP_PUB_SLOTS@" ${INPUT_H} | sed 's/^\([[:digit:]]\+\):.*/\1/')
head -n $LC_BEGIN_IMP ${INPUT_H} > ${INPUT_H}.tmp
if [ -r  $OUT_IMP ]; then
  cat $OUT_IMP >> ${INPUT_H}.tmp
fi
tail -n +$LC_END_IMP ${INPUT_H} >> ${INPUT_H}.tmp
mv -f ${INPUT_H}.tmp ${INPUT_H}

rm -f $OUT_DEF
rm -f $OUT_IMP

astyle ${INPUT_H}
rm -f ${INPUT_H}.orig

