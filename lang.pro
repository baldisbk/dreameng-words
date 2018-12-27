QT += quick widgets
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SOURCES += \
        main.cpp \
    appstate.cpp \
    settings.cpp \
    pagestate.cpp \
    barseries.cpp \
    wordselectors.cpp

RESOURCES += qml.qrc

TRANSLATIONS = lang_ru_RU.ts lang_en_US.ts

HEADERS += \
    appstate.h \
    settings.h \
    pagestate.h \
    word.h \
    barseries.h \
    wordselectors.h \
    constants.h

DISTFILES += \
    sample.js \
    words2.csv \
    lang_en_US.ts \
    lang_ru_RU.ts \
    makebuildstats.sh

HEADER_TEMPLATES = $$PWD/buildstats.gh

PRE_TARGETDEPS += $$HEADER_TEMPLATES
QMAKE_EXTRA_TARGETS += mytarget mytarget2 buildstats

buildstats.target = $$HEADER_TEMPLATES
buildstats.commands = cd $$PWD && ./makebuildstats.sh $$HEADER_TEMPLATES
buildstats.CONFIG += no_link
buildstats.depends = FORCE

isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}
updateqm.input = TRANSLATIONS
updateqm.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE \
    ${QMAKE_FILE_IN} \
    -qm \
    ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link

QMAKE_EXTRA_COMPILERS += updateqm
