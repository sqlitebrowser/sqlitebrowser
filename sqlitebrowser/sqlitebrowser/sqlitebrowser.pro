SOURCES	+= browsermain.cpp \
	sqlitedb.cpp
HEADERS	+= sqlitedb.h
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  LIBS += ./sqlite_source/libsqlite_source.a
}
win32 {
  RC_FILE = winapp.rc
  LIBS	+= sqlite_source/sqlite_source.lib
}
mac {
  RC_FILE = macapp.icns
  LIBS	+= -framework Carbon -framework Quicktime -lz ./sqlite_source/libsqlite_source.a
}

	
FORMS	= form1.ui \
	findform.ui \
	createtableform.ui \
	createindexform.ui \
	deletetableform.ui \
	deleteindexform.ui \
	aboutform.ui
IMAGES	= images/whatsthis.png \
	images/filenew.png \
	images/editpaste.png \
	images/fileopen.png \
	images/icone16.png \
	images/searchfind.png \
	images/editcopy.png
	
TEMPLATE	=app
CONFIG	+= qt warn_on release
LANGUAGE	= C++
