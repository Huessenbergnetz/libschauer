#!/bin/sh

if [ ! -d translations ]; then
    mkdir translations
fi

if [ ! $QTLUPDATE ]; then
    for name in lupdate-qt6 lupdate6 lupdate-qt5 lupdate5 lupdate; do
        QTLUPDATE=`which ${name} 2> /dev/null`
        if [ -x "${QTLUPDATE}" ]; then
            echo "Found lupdate at ${QTLUPDATE}"
            break
        fi
    done
fi

if [ ! -x "${QTLUPDATE}" ]; then
    echo "Can not find lupdate executable"
    exit 1
fi

$QTLUPDATE -no-obsolete \
           -source-language en \
           -target-language en \
           -locations none \
           Schauer \
           -ts "translations/libschauer.ts"
