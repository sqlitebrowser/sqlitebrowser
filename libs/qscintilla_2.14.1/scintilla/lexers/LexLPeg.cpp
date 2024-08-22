/**
 * Copyright 2006-2018 Mitchell mitchell.att.foicica.com. See License.txt.
 *
 * Lua-powered dynamic language lexer for Scintilla.
 *
 * For documentation on writing lexers, see *../doc/LPegLexer.html*.
 */

#if LPEG_LEXER

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#if CURSES
#include <curses.h>
#endif

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "PropSetSimple.h"
#include "LexAccessor.h"
#include "LexerModule.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
LUALIB_API int luaopen_lpeg(lua_State *L);
}

#if _WIN32
#define strcasecmp _stricmp
#endif
#define streq(s1, s2) (strcasecmp((s1), (s2)) == 0)

using namespace Scintilla;

#define l_setmetatable(l, k, mtf) \
	if (luaL_newmetatable(l, k)) { \
		lua_pushcfunction(l, mtf), lua_setfield(l, -2, "__index"); \
		lua_pushcfunction(l, mtf), lua_setfield(l, -2, "__newindex"); \
	} \
	lua_setmetatable(l, -2);
#define l_pushlexerp(l, mtf) do { \
	lua_newtable(l); \
	lua_pushvalue(l, 2), lua_setfield(l, -2, "property"); \
	l_setmetatable(l, "sci_lexerp", mtf); \
} while(0)
#define l_getlexerobj(l) \
	lua_getfield(l, LUA_REGISTRYINDEX, "sci_lexers"); \
	lua_pushlightuserdata(l, reinterpret_cast<void *>(this)); \
	lua_gettable(l, -2), lua_replace(l, -2);
#define l_getlexerfield(l, k) \
	l_getlexerobj(l); \
	lua_getfield(l, -1, k), lua_replace(l, -2);
#if LUA_VERSION_NUM < 502
#define l_openlib(f, s) \
	(lua_pushcfunction(L, f), lua_pushstring(L, s), lua_call(L, 1, 0))
#define LUA_BASELIBNAME ""
#define lua_rawlen lua_objlen
#define LUA_OK 0
#define lua_compare(l, a, b, _) lua_equal(l, a, b)
#define LUA_OPEQ 0
#else
#define l_openlib(f, s) (luaL_requiref(L, s, f, 1), lua_pop(L, 1))
#define LUA_BASELIBNAME "_G"
#endif
#define l_setfunction(l, f, k) (lua_pushcfunction(l, f), lua_setfield(l, -2, k))
#define l_setconstant(l, c, k) (lua_pushinteger(l, c), lua_setfield(l, -2, k))

#if CURSES
#define A_COLORCHAR (A_COLOR | A_CHARTEXT)
#endif

/** The LPeg Scintilla lexer. */
class LexerLPeg : public ILexer {
	/**
	 * The lexer's Lua state.
	 * It is cleared each time the lexer language changes unless `own_lua` is
	 * `true`.
	 */
	lua_State *L;
	/**
	 * The flag indicating whether or not an existing Lua state was supplied as
	 * the lexer's Lua state.
	 */
	bool own_lua;
	/**
	 * The set of properties for the lexer.
	 * The `lexer.name`, `lexer.lpeg.home`, and `lexer.lpeg.color.theme`
	 * properties must be defined before running the lexer.
	 */
	PropSetSimple props;
	/** The function to send Scintilla messages with. */
	SciFnDirect SS;
	/** The Scintilla object the lexer belongs to. */
	sptr_t sci;
	/**
	 * The flag indicating whether or not the lexer needs to be re-initialized.
	 * Re-initialization is required after the lexer language changes.
	 */
	bool reinit;
	/**
	 * The flag indicating whether or not the lexer language has embedded lexers.
	 */
	bool multilang;
	/**
	 * The list of style numbers considered to be whitespace styles.
	 * This is used in multi-language lexers when backtracking to whitespace to
	 * determine which lexer grammar to use.
	 */
	bool ws[STYLE_MAX + 1];

	/**
	 * Logs the given error message or a Lua error message, prints it, and clears
	 * the stack.
	 * Error messages are logged to the "lexer.lpeg.error" property.
	 * @param str The error message to log and print. If `NULL`, logs and prints
	 *   the Lua error message at the top of the stack.
	 */
	static void l_error(lua_State *L, const char *str=NULL) {
		lua_getfield(L, LUA_REGISTRYINDEX, "sci_props");
		PropSetSimple *props = static_cast<PropSetSimple *>(lua_touserdata(L, -1));
		lua_pop(L, 1); // props
		const char *key = "lexer.lpeg.error";
		const char *value = str ? str : lua_tostring(L, -1);
		props->Set(key, value, strlen(key), strlen(value));
		fprintf(stderr, "Lua Error: %s.\n", str ? str : lua_tostring(L, -1));
		lua_settop(L, 0);
	}

	/** The lexer's `line_from_position` Lua function. */
	static int l_line_from_position(lua_State *L) {
		lua_getfield(L, LUA_REGISTRYINDEX, "sci_buffer");
		IDocument *buffer = static_cast<IDocument *>(lua_touserdata(L, -1));
		lua_pushinteger(L, buffer->LineFromPosition(luaL_checkinteger(L, 1) - 1));
		return 1;
	}

	/** The lexer's `__index` Lua metatable. */
	static int llexer_property(lua_State *L) {
		int newindex = (lua_gettop(L) == 3);
		luaL_getmetatable(L, "sci_lexer");
		lua_getmetatable(L, 1); // metatable can be either sci_lexer or sci_lexerp
		int is_lexer = lua_compare(L, -1, -2, LUA_OPEQ);
		lua_pop(L, 2); // metatable, metatable

		lua_getfield(L, LUA_REGISTRYINDEX, "sci_buffer");
		IDocument *buffer = static_cast<IDocument *>(lua_touserdata(L, -1));
		lua_getfield(L, LUA_REGISTRYINDEX, "sci_props");
		PropSetSimple *props = static_cast<PropSetSimple *>(lua_touserdata(L, -1));
		lua_pop(L, 2); // sci_props and sci_buffer

		if (is_lexer)
			lua_pushvalue(L, 2); // key is given
		else
			lua_getfield(L, 1, "property"); // indexible property
		const char *key = lua_tostring(L, -1);
		if (strcmp(key, "fold_level") == 0) {
			luaL_argcheck(L, !newindex, 3, "read-only property");
			if (is_lexer)
				l_pushlexerp(L, llexer_property);
			else
				lua_pushinteger(L, buffer->GetLevel(luaL_checkinteger(L, 2)));
		} else if (strcmp(key, "indent_amount") == 0) {
			luaL_argcheck(L, !newindex, 3, "read-only property");
			if (is_lexer)
				l_pushlexerp(L, llexer_property);
			else
				lua_pushinteger(L, buffer->GetLineIndentation(luaL_checkinteger(L, 2)));
		} else if (strcmp(key, "property") == 0) {
			luaL_argcheck(L, !is_lexer || !newindex, 3, "read-only property");
			if (is_lexer)
				l_pushlexerp(L, llexer_property);
			else if (!newindex)
				lua_pushstring(L, props->Get(luaL_checkstring(L, 2)));
			else
				props->Set(luaL_checkstring(L, 2), luaL_checkstring(L, 3),
				           lua_rawlen(L, 2), lua_rawlen(L, 3));
		} else if (strcmp(key, "property_int") == 0) {
			luaL_argcheck(L, !newindex, 3, "read-only property");
			if (is_lexer)
				l_pushlexerp(L, llexer_property);
			else {
				lua_pushstring(L, props->Get(luaL_checkstring(L, 2)));
				lua_pushinteger(L, lua_tointeger(L, -1));
			}
		} else if (strcmp(key, "style_at") == 0) {
			luaL_argcheck(L, !newindex, 3, "read-only property");
			if (is_lexer)
				l_pushlexerp(L, llexer_property);
			else {
				int style = buffer->StyleAt(luaL_checkinteger(L, 2) - 1);
				lua_getfield(L, LUA_REGISTRYINDEX, "sci_lexer_obj");
				lua_getfield(L, -1, "_TOKENSTYLES"), lua_replace(L, -2);
				lua_pushnil(L);
				while (lua_next(L, -2)) {
					if (luaL_checkinteger(L, -1) == style) break;
					lua_pop(L, 1); // value
				}
				lua_pop(L, 1); // style_num
			}
		} else if (strcmp(key, "line_state") == 0) {
			luaL_argcheck(L, !is_lexer || !newindex, 3, "read-only property");
			if (is_lexer)
				l_pushlexerp(L, llexer_property);
			else if (!newindex)
				lua_pushinteger(L, buffer->GetLineState(luaL_checkinteger(L, 2)));
			else
				buffer->SetLineState(luaL_checkinteger(L, 2),
				                     luaL_checkinteger(L, 3));
		} else return !newindex ? (lua_rawget(L, 1), 1) : (lua_rawset(L, 1), 0);
		return 1;
	}

	/**
	 * Expands value of the string property key at index *index* and pushes the
	 * result onto the stack.
	 * @param L The Lua State.
	 * @param index The index the string property key.
	 */
	void lL_getexpanded(lua_State *L, int index) {
		lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED"), lua_getfield(L, -1, "lexer");
		lua_getfield(L, -1, "property_expanded");
		lua_pushvalue(L, (index > 0) ? index : index - 3), lua_gettable(L, -2);
		lua_replace(L, -4), lua_pop(L, 2); // property_expanded and lexer module
	}

	/**
	 * Parses the given style string to set the properties for the given style
	 * number.
	 * @param num The style number to set properties for.
	 * @param style The style string containing properties to set.
	 */
	void SetStyle(int num, const char *style) {
		char *style_copy = static_cast<char *>(malloc(strlen(style) + 1));
		char *option = strcpy(style_copy, style), *next = NULL, *p = NULL;
		while (option) {
			if ((next = strchr(option, ','))) *next++ = '\0';
			if ((p = strchr(option, ':'))) *p++ = '\0';
			if (streq(option, "font") && p)
				SS(sci, SCI_STYLESETFONT, num, reinterpret_cast<sptr_t>(p));
			else if (streq(option, "size") && p)
				SS(sci, SCI_STYLESETSIZE, num, static_cast<int>(atoi(p)));
			else if (streq(option, "bold") || streq(option, "notbold") ||
			         streq(option, "weight")) {
#if !CURSES
				int weight = SC_WEIGHT_NORMAL;
				if (*option == 'b')
					weight = SC_WEIGHT_BOLD;
				else if (*option == 'w' && p)
					weight = atoi(p);
				SS(sci, SCI_STYLESETWEIGHT, num, weight);
#else
				// Scintilla curses requires font attributes to be stored in the "font
				// weight" style attribute.
				// First, clear any existing SC_WEIGHT_NORMAL, SC_WEIGHT_SEMIBOLD, or
				// SC_WEIGHT_BOLD values stored in the lower 16 bits. Then set the
				// appropriate curses attr.
				sptr_t weight = SS(sci, SCI_STYLEGETWEIGHT, num, 0) & ~A_COLORCHAR;
				int bold = *option == 'b' ||
				           (*option == 'w' && p && atoi(p) > SC_WEIGHT_NORMAL);
				SS(sci, SCI_STYLESETWEIGHT, num,
				   bold ? weight | A_BOLD : weight & ~A_BOLD);
#endif
			} else if (streq(option, "italics") || streq(option, "notitalics"))
				SS(sci, SCI_STYLESETITALIC, num, *option == 'i');
			else if (streq(option, "underlined") || streq(option, "notunderlined")) {
#if !CURSES
				SS(sci, SCI_STYLESETUNDERLINE, num, *option == 'u');
#else
				// Scintilla curses requires font attributes to be stored in the "font
				// weight" style attribute.
				// First, clear any existing SC_WEIGHT_NORMAL, SC_WEIGHT_SEMIBOLD, or
				// SC_WEIGHT_BOLD values stored in the lower 16 bits. Then set the
				// appropriate curses attr.
				sptr_t weight = SS(sci, SCI_STYLEGETWEIGHT, num, 0) & ~A_COLORCHAR;
				SS(sci, SCI_STYLESETWEIGHT, num,
				   (*option == 'u') ? weight | A_UNDERLINE : weight & ~A_UNDERLINE);
#endif
			} else if ((streq(option, "fore") || streq(option, "back")) && p) {
				int msg = (*option == 'f') ? SCI_STYLESETFORE : SCI_STYLESETBACK;
				int color = static_cast<int>(strtol(p, NULL, 0));
				if (*p == '#') { // #RRGGBB format; Scintilla format is 0xBBGGRR
					color = static_cast<int>(strtol(p + 1, NULL, 16));
					color = ((color & 0xFF0000) >> 16) | (color & 0xFF00) |
					        ((color & 0xFF) << 16); // convert to 0xBBGGRR
				}
				SS(sci, msg, num, color);
			} else if (streq(option, "eolfilled") || streq(option, "noteolfilled"))
				SS(sci, SCI_STYLESETEOLFILLED, num, *option == 'e');
			else if (streq(option, "characterset") && p)
				SS(sci, SCI_STYLESETCHARACTERSET, num, static_cast<int>(atoi(p)));
			else if (streq(option, "case") && p) {
				if (*p == 'u')
					SS(sci, SCI_STYLESETCASE, num, SC_CASE_UPPER);
				else if (*p == 'l')
					SS(sci, SCI_STYLESETCASE, num, SC_CASE_LOWER);
			} else if (streq(option, "visible") || streq(option, "notvisible"))
				SS(sci, SCI_STYLESETVISIBLE, num, *option == 'v');
			else if (streq(option, "changeable") || streq(option, "notchangeable"))
				SS(sci, SCI_STYLESETCHANGEABLE, num, *option == 'c');
			else if (streq(option, "hotspot") || streq(option, "nothotspot"))
				SS(sci, SCI_STYLESETHOTSPOT, num, *option == 'h');
			option = next;
		}
		free(style_copy);
	}

	/**
	 * Iterates through the lexer's `_TOKENSTYLES`, setting the style properties
	 * for all defined styles.
	 */
	bool SetStyles() {
		// If the lexer defines additional styles, set their properties first (if
		// the user has not already defined them).
		l_getlexerfield(L, "_EXTRASTYLES");
		lua_pushnil(L);
		while (lua_next(L, -2)) {
			if (lua_isstring(L, -2) && lua_isstring(L, -1)) {
				lua_pushstring(L, "style."), lua_pushvalue(L, -3), lua_concat(L, 2);
				if (!*props.Get(lua_tostring(L, -1)))
					props.Set(lua_tostring(L, -1), lua_tostring(L, -2), lua_rawlen(L, -1),
					          lua_rawlen(L, -2));
				lua_pop(L, 1); // style name
			}
			lua_pop(L, 1); // value
		}
		lua_pop(L, 1); // _EXTRASTYLES

		l_getlexerfield(L, "_TOKENSTYLES");
		if (!SS || !sci) {
			lua_pop(L, 1); // _TOKENSTYLES
			// Skip, but do not report an error since `reinit` would remain `false`
			// and subsequent calls to `Lex()` and `Fold()` would repeatedly call this
			// function and error.
			return true;
		}
		lua_pushstring(L, "style.default"), lL_getexpanded(L, -1);
		SetStyle(STYLE_DEFAULT, lua_tostring(L, -1));
		lua_pop(L, 2); // style and "style.default"
		SS(sci, SCI_STYLECLEARALL, 0, 0); // set default styles
		lua_pushnil(L);
		while (lua_next(L, -2)) {
			if (lua_isstring(L, -2) && lua_isnumber(L, -1) &&
			    lua_tointeger(L, -1) != STYLE_DEFAULT) {
				lua_pushstring(L, "style."), lua_pushvalue(L, -3), lua_concat(L, 2);
				lL_getexpanded(L, -1), lua_replace(L, -2);
				SetStyle(lua_tointeger(L, -2), lua_tostring(L, -1));
				lua_pop(L, 1); // style
			}
			lua_pop(L, 1); // value
		}
		lua_pop(L, 1); // _TOKENSTYLES
		return true;
	}

	/**
	 * Returns the style name for the given style number.
	 * @param style The style number to get the style name for.
	 * @return style name or NULL
	 */
	const char *GetStyleName(int style) {
		if (!L) return NULL;
		const char *name = NULL;
		l_getlexerfield(L, "_TOKENSTYLES");
		lua_pushnil(L);
		while (lua_next(L, -2))
			if (lua_tointeger(L, -1) == style) {
				name = lua_tostring(L, -2);
				lua_pop(L, 2); // value and key
				break;
			} else lua_pop(L, 1); // value
		lua_pop(L, 1); // _TOKENSTYLES
		return name;
	}

	/**
	 * Initializes the lexer once the `lexer.lpeg.home` and `lexer.name`
	 * properties are set.
	 */
	bool Init() {
		char home[FILENAME_MAX], lexer[50], theme[FILENAME_MAX];
		props.GetExpanded("lexer.lpeg.home", home);
		props.GetExpanded("lexer.name", lexer);
		props.GetExpanded("lexer.lpeg.color.theme", theme);
		if (!*home || !*lexer || !L) return false;

		lua_pushlightuserdata(L, reinterpret_cast<void *>(&props));
		lua_setfield(L, LUA_REGISTRYINDEX, "sci_props");

		// If necessary, load the lexer module and theme.
		lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED"), lua_getfield(L, -1, "lexer");
		if (lua_isnil(L, -1)) {
			lua_pop(L, 2); // nil and _LOADED

			// Modify `package.path` to find lexers.
			lua_getglobal(L, "package"), lua_getfield(L, -1, "path");
			int orig_path = luaL_ref(L, LUA_REGISTRYINDEX); // restore later
			lua_pushstring(L, home), lua_pushstring(L, "/?.lua"), lua_concat(L, 2);
			lua_setfield(L, -2, "path"), lua_pop(L, 1); // package

			// Load the lexer module.
			lua_getglobal(L, "require");
			lua_pushstring(L, "lexer");
			if (lua_pcall(L, 1, 1, 0) != LUA_OK) return (l_error(L), false);
			l_setfunction(L, l_line_from_position, "line_from_position");
			l_setconstant(L, SC_FOLDLEVELBASE, "FOLD_BASE");
			l_setconstant(L, SC_FOLDLEVELWHITEFLAG, "FOLD_BLANK");
			l_setconstant(L, SC_FOLDLEVELHEADERFLAG, "FOLD_HEADER");
			l_setmetatable(L, "sci_lexer", llexer_property);
			if (*theme) {
				// Load the theme.
				if (!(strstr(theme, "/") || strstr(theme, "\\"))) { // theme name
					lua_pushstring(L, home);
					lua_pushstring(L, "/themes/");
					lua_pushstring(L, theme);
					lua_pushstring(L, ".lua");
					lua_concat(L, 4);
				} else lua_pushstring(L, theme); // path to theme
				if (luaL_loadfile(L, lua_tostring(L, -1)) != LUA_OK ||
				    lua_pcall(L, 0, 0, 0) != LUA_OK) return (l_error(L), false);
				lua_pop(L, 1); // theme
			}

			// Restore `package.path`.
			lua_getglobal(L, "package");
			lua_getfield(L, -1, "path"), lua_setfield(L, -3, "path"); // lexer.path =
			lua_rawgeti(L, LUA_REGISTRYINDEX, orig_path), lua_setfield(L, -2, "path");
			luaL_unref(L, LUA_REGISTRYINDEX, orig_path), lua_pop(L, 1); // package
		} else lua_remove(L, -2); // _LOADED

		// Load the language lexer.
		lua_getfield(L, -1, "load");
		if (lua_isfunction(L, -1)) {
			lua_pushstring(L, lexer), lua_pushnil(L), lua_pushboolean(L, 1);
			if (lua_pcall(L, 3, 1, 0) != LUA_OK) return (l_error(L), false);
		} else return (l_error(L, "'lexer.load' function not found"), false);
		lua_getfield(L, LUA_REGISTRYINDEX, "sci_lexers");
		lua_pushlightuserdata(L, reinterpret_cast<void *>(this));
		lua_pushvalue(L, -3), lua_settable(L, -3), lua_pop(L, 1); // sci_lexers
		lua_pushvalue(L, -1), lua_setfield(L, LUA_REGISTRYINDEX, "sci_lexer_obj");
		lua_remove(L, -2); // lexer module
		if (!SetStyles()) return false;

		// If the lexer is a parent, it will have children in its _CHILDREN table.
		lua_getfield(L, -1, "_CHILDREN");
		if (lua_istable(L, -1)) {
			multilang = true;
			// Determine which styles are language whitespace styles
			// ([lang]_whitespace). This is necessary for determining which language
			// to start lexing with.
			char style_name[50];
			for (int i = 0; i <= STYLE_MAX; i++) {
				PrivateCall(i, reinterpret_cast<void *>(style_name));
				ws[i] = strstr(style_name, "whitespace") ? true : false;
			}
		}
		lua_pop(L, 2); // _CHILDREN and lexer object

		reinit = false;
		props.Set("lexer.lpeg.error", "", strlen("lexer.lpeg.error"), 0);
		return true;
	}

	/**
	 * When *lparam* is `0`, returns the size of the buffer needed to store the
	 * given string *str* in; otherwise copies *str* into the buffer *lparam* and
	 * returns the number of bytes copied.
	 * @param lparam `0` to get the number of bytes needed to store *str* or a
	 *   pointer to a buffer large enough to copy *str* into.
	 * @param str The string to copy.
	 * @return number of bytes needed to hold *str*
	 */
	void *StringResult(long lparam, const char *str) {
		if (lparam) strcpy(reinterpret_cast<char *>(lparam), str);
		return reinterpret_cast<void *>(strlen(str));
	}

public:
	/** Constructor. */
	LexerLPeg() : own_lua(true), reinit(true), multilang(false) {
		// Initialize the Lua state, load libraries, and set platform variables.
		if ((L = luaL_newstate())) {
			l_openlib(luaopen_base, LUA_BASELIBNAME);
			l_openlib(luaopen_table, LUA_TABLIBNAME);
			l_openlib(luaopen_string, LUA_STRLIBNAME);
#if LUA_VERSION_NUM < 502
			l_openlib(luaopen_io, LUA_IOLIBNAME); // for `package.searchpath()`
#endif
			l_openlib(luaopen_package, LUA_LOADLIBNAME);
			l_openlib(luaopen_lpeg, "lpeg");
#if _WIN32
			lua_pushboolean(L, 1), lua_setglobal(L, "WIN32");
#endif
#if __APPLE__
			lua_pushboolean(L, 1), lua_setglobal(L, "OSX");
#endif
#if GTK
			lua_pushboolean(L, 1), lua_setglobal(L, "GTK");
#endif
#if CURSES
			lua_pushboolean(L, 1), lua_setglobal(L, "CURSES");
#endif
			lua_newtable(L), lua_setfield(L, LUA_REGISTRYINDEX, "sci_lexers");
		} else fprintf(stderr, "Lua failed to initialize.\n");
		SS = NULL, sci = 0;
	}

	/** Destructor. */
	virtual ~LexerLPeg() {}

	/** Destroys the lexer object. */
	virtual void SCI_METHOD Release() {
		if (own_lua && L)
			lua_close(L);
		else if (!own_lua) {
			lua_getfield(L, LUA_REGISTRYINDEX, "sci_lexers");
			lua_pushlightuserdata(L, reinterpret_cast<void *>(this));
			lua_pushnil(L), lua_settable(L, -3), lua_pop(L, 1); // sci_lexers
		}
		L = NULL;
		delete this;
	}

	/**
	 * Lexes the Scintilla document.
	 * @param startPos The position in the document to start lexing at.
	 * @param lengthDoc The number of bytes in the document to lex.
	 * @param initStyle The initial style at position *startPos* in the document.
	 * @param buffer The document interface.
	 */
	virtual void SCI_METHOD Lex(Sci_PositionU startPos, Sci_Position lengthDoc,
	                            int initStyle, IDocument *buffer) {
		LexAccessor styler(buffer);
		if ((reinit && !Init()) || !L) {
			// Style everything in the default style.
			styler.StartAt(startPos);
			styler.StartSegment(startPos);
			styler.ColourTo(startPos + lengthDoc - 1, STYLE_DEFAULT);
			styler.Flush();
			return;
		}
		lua_pushlightuserdata(L, reinterpret_cast<void *>(&props));
		lua_setfield(L, LUA_REGISTRYINDEX, "sci_props");
		lua_pushlightuserdata(L, reinterpret_cast<void *>(buffer));
		lua_setfield(L, LUA_REGISTRYINDEX, "sci_buffer");

		// Ensure the lexer has a grammar.
		// This could be done in the lexer module's `lex()`, but for large files,
		// passing string arguments from C to Lua is expensive.
		l_getlexerfield(L, "_GRAMMAR");
		int has_grammar = !lua_isnil(L, -1);
		lua_pop(L, 1); // _GRAMMAR
		if (!has_grammar) {
			// Style everything in the default style.
			styler.StartAt(startPos);
			styler.StartSegment(startPos);
			styler.ColourTo(startPos + lengthDoc - 1, STYLE_DEFAULT);
			styler.Flush();
			return;
		}

		// Start from the beginning of the current style so LPeg matches it.
		// For multilang lexers, start at whitespace since embedded languages have
		// [lang]_whitespace styles. This is so LPeg can start matching child
		// languages instead of parent ones if necessary.
		if (startPos > 0) {
			Sci_PositionU i = startPos;
			while (i > 0 && styler.StyleAt(i - 1) == initStyle) i--;
			if (multilang)
				while (i > 0 && !ws[static_cast<size_t>(styler.StyleAt(i))]) i--;
			lengthDoc += startPos - i, startPos = i;
		}

		Sci_PositionU startSeg = startPos, endSeg = startPos + lengthDoc;
		int style = 0;
		l_getlexerfield(L, "lex")
		if (lua_isfunction(L, -1)) {
			l_getlexerobj(L);
			lua_pushlstring(L, buffer->BufferPointer() + startPos, lengthDoc);
			lua_pushinteger(L, styler.StyleAt(startPos));
			if (lua_pcall(L, 3, 1, 0) != LUA_OK) l_error(L);
			// Style the text from the token table returned.
			if (lua_istable(L, -1)) {
				int len = lua_rawlen(L, -1);
				if (len > 0) {
					styler.StartAt(startPos);
					styler.StartSegment(startPos);
					l_getlexerfield(L, "_TOKENSTYLES");
					// Loop through token-position pairs.
					for (int i = 1; i < len; i += 2) {
						style = STYLE_DEFAULT;
						lua_rawgeti(L, -2, i), lua_rawget(L, -2); // _TOKENSTYLES[token]
						if (!lua_isnil(L, -1)) style = lua_tointeger(L, -1);
						lua_pop(L, 1); // _TOKENSTYLES[token]
						lua_rawgeti(L, -2, i + 1); // pos
						unsigned int position = lua_tointeger(L, -1) - 1;
						lua_pop(L, 1); // pos
						if (style >= 0 && style <= STYLE_MAX)
							styler.ColourTo(startSeg + position - 1, style);
						else
							l_error(L, "Bad style number");
						if (position > endSeg) break;
					}
					lua_pop(L, 2); // _TOKENSTYLES and token table returned
					styler.ColourTo(endSeg - 1, style);
					styler.Flush();
				}
			} else l_error(L, "Table of tokens expected from 'lexer.lex'");
		} else l_error(L, "'lexer.lex' function not found");
	}

	/**
	 * Folds the Scintilla document.
	 * @param startPos The position in the document to start folding at.
	 * @param lengthDoc The number of bytes in the document to fold.
	 * @param initStyle The initial style at position *startPos* in the document.
	 * @param buffer The document interface.
	 */
	virtual void SCI_METHOD Fold(Sci_PositionU startPos, Sci_Position lengthDoc,
	                             int, IDocument *buffer) {
		if ((reinit && !Init()) || !L) return;
		lua_pushlightuserdata(L, reinterpret_cast<void *>(&props));
		lua_setfield(L, LUA_REGISTRYINDEX, "sci_props");
		lua_pushlightuserdata(L, reinterpret_cast<void *>(buffer));
		lua_setfield(L, LUA_REGISTRYINDEX, "sci_buffer");
		LexAccessor styler(buffer);

		l_getlexerfield(L, "fold");
		if (lua_isfunction(L, -1)) {
			l_getlexerobj(L);
			Sci_Position currentLine = styler.GetLine(startPos);
			lua_pushlstring(L, buffer->BufferPointer() + startPos, lengthDoc);
			lua_pushinteger(L, startPos);
			lua_pushinteger(L, currentLine);
			lua_pushinteger(L, styler.LevelAt(currentLine) & SC_FOLDLEVELNUMBERMASK);
			if (lua_pcall(L, 5, 1, 0) != LUA_OK) l_error(L);
			// Fold the text from the fold table returned.
			if (lua_istable(L, -1)) {
				lua_pushnil(L);
				while (lua_next(L, -2)) { // line = level
					styler.SetLevel(lua_tointeger(L, -2), lua_tointeger(L, -1));
					lua_pop(L, 1); // level
				}
				lua_pop(L, 1); // fold table returned
			} else l_error(L, "Table of folds expected from 'lexer.fold'");
		} else l_error(L, "'lexer.fold' function not found");
	}

	/** This lexer implements the original lexer interface. */
	virtual int SCI_METHOD Version() const { return lvOriginal; }
	/** Returning property names is not implemented. */
	virtual const char * SCI_METHOD PropertyNames() { return ""; }
	/** Returning property types is not implemented. */
	virtual int SCI_METHOD PropertyType(const char *) { return 0; }
	/** Returning property descriptions is not implemented. */
	virtual const char * SCI_METHOD DescribeProperty(const char *) {
		return "";
	}

	/**
	 * Sets the *key* lexer property to *value*.
	 * If *key* starts with "style.", also set the style for the token.
	 * @param key The string keyword.
	 * @param val The string value.
	 */
	virtual Sci_Position SCI_METHOD PropertySet(const char *key,
	                                            const char *value) {
		props.Set(key, value, strlen(key), strlen(value));
		if (reinit)
			Init();
		else if (L && SS && sci && strncmp(key, "style.", 6) == 0) {
			lua_pushlightuserdata(L, reinterpret_cast<void *>(&props));
			lua_setfield(L, LUA_REGISTRYINDEX, "sci_props");
			l_getlexerfield(L, "_TOKENSTYLES");
			lua_pushstring(L, key + 6), lua_rawget(L, -2);
			lua_pushstring(L, key), lL_getexpanded(L, -1), lua_replace(L, -2);
			if (lua_isnumber(L, -2)) {
				int style_num = lua_tointeger(L, -2);
				SetStyle(style_num, lua_tostring(L, -1));
				if (style_num == STYLE_DEFAULT)
					// Assume a theme change, with the default style being set first.
					// Subsequent style settings will be based on the default.
					SS(sci, SCI_STYLECLEARALL, 0, 0);
			}
			lua_pop(L, 3); // style, style number, _TOKENSTYLES
		}
		return -1; // no need to re-lex
	}

	/** Returning keyword list descriptions is not implemented. */
	virtual const char * SCI_METHOD DescribeWordListSets() { return ""; }
	/** Setting keyword lists is not applicable. */
	virtual Sci_Position SCI_METHOD WordListSet(int, const char *) {
		return -1;
	}

	/**
	 * Allows for direct communication between the application and the lexer.
	 * The application uses this to set `SS`, `sci`, `L`, and lexer properties,
	 * and to retrieve style names.
	 * @param code The communication code.
	 * @param arg The argument.
	 * @return void *data
	 */
	virtual void * SCI_METHOD PrivateCall(int code, void *arg) {
		sptr_t lParam = reinterpret_cast<sptr_t>(arg);
		const char *val = NULL;
		switch(code) {
		case SCI_GETDIRECTFUNCTION:
			SS = reinterpret_cast<SciFnDirect>(lParam);
			return NULL;
		case SCI_SETDOCPOINTER:
			sci = lParam;
			return NULL;
		case SCI_CHANGELEXERSTATE:
			if (own_lua) lua_close(L);
			L = reinterpret_cast<lua_State *>(lParam);
			lua_getfield(L, LUA_REGISTRYINDEX, "sci_lexers");
			if (lua_isnil(L, -1))
				lua_newtable(L), lua_setfield(L, LUA_REGISTRYINDEX, "sci_lexers");
			lua_pop(L, 1); // sci_lexers or nil
			own_lua = false;
			return NULL;
		case SCI_SETLEXERLANGUAGE:
			char lexer_name[50];
			props.GetExpanded("lexer.name", lexer_name);
			if (strcmp(lexer_name, reinterpret_cast<const char *>(arg)) != 0) {
				reinit = true;
				props.Set("lexer.lpeg.error", "", strlen("lexer.lpeg.error"), 0);
				PropertySet("lexer.name", reinterpret_cast<const char *>(arg));
			} else if (L)
				own_lua ? SetStyles() : Init();
			return NULL;
		case SCI_GETLEXERLANGUAGE:
			if (L) {
				l_getlexerfield(L, "_NAME");
				if (SS && sci && multilang) {
					int pos = SS(sci, SCI_GETCURRENTPOS, 0, 0);
					while (pos >= 0 && !ws[SS(sci, SCI_GETSTYLEAT, pos, 0)]) pos--;
					const char *name = NULL, *p = NULL;
					if (pos >= 0) {
						name = GetStyleName(SS(sci, SCI_GETSTYLEAT, pos, 0));
						if (name) p = strstr(name, "_whitespace");
					}
					if (!name) name = lua_tostring(L, -1); // "lexer:lexer" fallback
					if (!p) p = name + strlen(name); // "lexer:lexer" fallback
					lua_pushstring(L, "/");
					lua_pushlstring(L, name, p - name);
					lua_concat(L, 3);
				}
				val = lua_tostring(L, -1);
				lua_pop(L, 1); // lexer_name or lexer language string
			}
			return StringResult(lParam, val ? val : "null");
		case SCI_GETSTATUS:
			return StringResult(lParam, props.Get("lexer.lpeg.error"));
		default: // style-related
			if (code >= 0 && code <= STYLE_MAX) { // retrieve style names
				val = GetStyleName(code);
				return StringResult(lParam, val ? val : "Not Available");
			} else return NULL;
		}
	}

	/** Constructs a new instance of the lexer. */
	static ILexer *LexerFactoryLPeg() { return new LexerLPeg(); }
};

LexerModule lmLPeg(SCLEX_LPEG, LexerLPeg::LexerFactoryLPeg, "lpeg");

#else

#include <stdlib.h>
#include <assert.h>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "LexerModule.h"

using namespace Scintilla;

static void LPegLex(Sci_PositionU, Sci_Position, int, WordList*[], Accessor&) {
	return;
}

LexerModule lmLPeg(SCLEX_LPEG, LPegLex, "lpeg");

#endif // LPEG_LEXER
