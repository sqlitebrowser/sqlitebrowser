SOURCES	+= where.c \
	attach.c \
	auth.c \
	btree.c \
	btree_rb.c \
	build.c \
	copy.c \
	delete.c \
	encode.c \
	expr.c \
	func.c \
	hash.c \
	insert.c \
	main.c \
	opcodes.c \
	os.c \
	pager.c \
	parse.c \
	pragma.c \
	printf.c \
	random.c \
	select.c \
	table.c \
	tokenize.c \
	trigger.c \
	update.c \
	util.c \
	vacuum.c \
	vdbe.c
HEADERS	+= btree.h \
	config.h \
	encode.h \
	hash.h \
	opcodes.h \
	os.h \
	pager.h \
	parse.h \
	sqlite.h \
	sqliteInt.h \
	vdbe.h
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
TEMPLATE	=lib
CONFIG	+= qt warn_on release staticlib
LANGUAGE	= C
DEFINES -= UNICODE
DEFINES += NDEBUG
