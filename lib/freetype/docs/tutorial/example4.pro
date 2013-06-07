# example4.pro

QT += core gui
TARGET = example4
TEMPLATE = app
SOURCES += example4.cpp
MOC_DIR = ./moc
LIBS += -lz -lfreetype
INCLUDEPATH += $$MOC_DIR \
               /usr/local/include \
               /usr/local/include/freetype2

# EOF
