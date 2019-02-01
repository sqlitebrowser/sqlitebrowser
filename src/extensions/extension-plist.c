/***  Extension-plist
 *
 *    This file adds Plist format support to SQLite.
 *
 *    Compile using:
 *
 *       gcc -g -fPIC -shared extension-plist.c -o extension-plist.so
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>

#define COMPILE_SQLITE_EXTENSIONS_AS_LOADABLE_MODULE 1
#include "sqlite3ext.h"
SQLITE_EXTENSION_INIT1

#define INDENT_INCREMENT 4

#define ERROR_NONE                   0
#define ERROR_INSUFFICIENT_MEMORY    1
#define ERROR_INVALID_HEADER         2
#define ERROR_INVALID_TRAILER        3
#define ERROR_INVALID_OFFSET         4
#define ERROR_INVALID_OBJECT_LENGTH  5
#define ERROR_INVALID_REFERENCE      6

typedef struct KEY {
  struct OBJECT *key;
  struct OBJECT *value;
} KEY;

typedef struct OBJECT {
  int type;
  int length;
  union {
    time_t date;
    double real;
    long integer;
    unsigned long uid;
    unsigned char binary[1];
    char text[1];
    short int utf16[1];
    int refs[1];
    KEY keys[1];
    struct OBJECT *objects[1];
  } data;
} OBJECT;

typedef struct CONFIG {
  int bytesPerOffset;
  int bytesPerReference;
  int objectCount;
  int rootObjectReference;
  int offsetTableOffset;
  size_t bufferLength;
  size_t outputBufferLength;
  size_t outputBufferIn;
  long *offsetTable;
  unsigned char *buffer;
  unsigned char *outputBuffer;
} CONFIG;

int createObject(int type, int length, int extra, OBJECT **ptr2);
int readObject(CONFIG *cfg, long offset, OBJECT **ptr2);

static int indent = 2;

int outputText(CONFIG *cfg, const char *text)
{
  size_t textLength = strlen(text);
  size_t availableSpace = cfg->outputBufferLength - cfg->outputBufferIn;
  while (textLength >= availableSpace) {
    unsigned char *tmp = cfg->outputBuffer;
    cfg->outputBufferLength += textLength + 1024;
    cfg->outputBuffer = (unsigned char *)realloc(cfg->outputBuffer, cfg->outputBufferLength);
    if (cfg->outputBuffer == NULL) {
      if (tmp != NULL)
        free(tmp);
      return ERROR_INSUFFICIENT_MEMORY;
    }
    availableSpace = cfg->outputBufferLength - cfg->outputBufferIn;
  }
  strcpy((char *)(cfg->outputBuffer+cfg->outputBufferIn), text);
  cfg->outputBufferIn += textLength;
  return ERROR_NONE;
}

int printWithIndent(CONFIG *cfg, const char *text, int newline)
{
  int err = ERROR_NONE;
  char spaces[9] = "        ";
  int n = indent;
  while ((n > 8) && (err == ERROR_NONE)) {
    err = outputText(cfg, spaces);
    n -= 8;
  }
  if ((n > 0) && (err == ERROR_NONE))
    err = outputText(cfg, spaces + 8 - n);
  if (err == ERROR_NONE)
    err = outputText(cfg, text);
  if (newline && (err == ERROR_NONE))
    err = outputText(cfg, "\n");
  return err;
}

int readHeader(CONFIG *cfg)
{
  if ((cfg->bufferLength > 40) &&
     ((strncmp((const char *)(cfg->buffer), "bplist0", 7) != 0) ||
     ((cfg->buffer[7] != '0') && (cfg->buffer[7] != '1'))))
    return ERROR_INVALID_HEADER;
  return ERROR_NONE;
}

int readTrailer(CONFIG *cfg)
{
  int i, j;
  int objectCount = 0;
  int rootObjectReference = 0;
  int offsetTableOffset = 0;
  unsigned char *ptr;
  unsigned char *buffer = cfg->buffer + cfg->bufferLength - 32;

  //  Extract the relevant fields
  for (i=12; i < 16; i++)
    objectCount = (objectCount << 8) + buffer[i];
  for (i=20; i < 24; i++)
    rootObjectReference = (rootObjectReference << 8) + buffer[i];
  for (i=28; i < 32; i++)
    offsetTableOffset = (offsetTableOffset << 8) + buffer[i];
   
  //  Populate the configurartion structure
  cfg->bytesPerOffset = buffer[6];
  cfg->bytesPerReference = buffer[7];
  cfg->objectCount = objectCount;
  cfg->rootObjectReference = rootObjectReference;
  cfg->offsetTableOffset = offsetTableOffset;
  cfg->offsetTable = (long *)malloc((size_t)objectCount * sizeof(long));
  if (cfg->offsetTable == NULL)
    return ERROR_INSUFFICIENT_MEMORY;
  ptr = cfg->buffer + offsetTableOffset;
  for (i=0; i < objectCount; i++) {
    long n = 0;
    for (j=0; j < cfg->bytesPerOffset; j++)
       n = (n << 8) | *(ptr++);
    cfg->offsetTable[i] = n;
  }
  return ERROR_NONE;
}
  
int createObject(int type, int length, int extra, OBJECT **ptr2)
{
  *ptr2 = NULL;
  OBJECT *ptr = (OBJECT *)malloc(sizeof(OBJECT) + (size_t)extra);
  if (ptr == NULL)
    return ERROR_INSUFFICIENT_MEMORY;
  ptr->type = type;
  ptr->length = length;
  *ptr2 = ptr;
  return ERROR_NONE;
}

long readInteger(unsigned char *ptr, int desc)
{
  long value = 0L;
  int n = 1 << (desc & 0x03);
  while (n--)
    value = (value << 8) | (long)(*(ptr++));
  return value;
}

unsigned long readUid(unsigned char *ptr, int desc)
{
  unsigned long value = 0L;
  int n = 1 << (desc & 0x03);
  while (n--)
    value = (value << 8) | (unsigned long)(*(ptr++));
  return value;
}

double readDouble(unsigned char *ptr, int desc)
{
  union {
    double v;
    float f[2];
    unsigned char b[8];
  } value;
  int n = 1 << (desc & 0x03);
  for (int i=0; i < n; i++)
    value.b[7-i] = *(ptr++);
  if (n == 4)
    value.v = (double)(value.f[1]);
  return value.v;
}

int readArray(CONFIG *cfg, unsigned char *ptr, int type, int length, OBJECT **array2)
{
  int i;
  long offset;
  OBJECT *array;
  int err = createObject(type, length, ((length-1) * (int)sizeof(OBJECT)), &array);
  *array2 = NULL;
  for (i=0; (i < length) && (err == ERROR_NONE); i++) {
    offset = 0L;
    for (int j=0; j < cfg->bytesPerReference; j++)
      offset = (offset << 8) | (long)(*(ptr++));
    if (offset >= cfg->objectCount)
      return ERROR_INVALID_REFERENCE;
    offset = cfg->offsetTable[offset];
    err = readObject(cfg, offset, &(array->data.objects[i]));
  }
  if (err == ERROR_NONE)
    *array2 = array;
  return err;
}

int readDictionary(CONFIG *cfg, unsigned char *ptr, int type, int length, OBJECT **dict2)
{
  int i;
  long offset;
  OBJECT *dict;
  int err  = createObject(type, length, ((length-1) * (int)sizeof(KEY)), &dict);
  *dict2 = NULL;
  if (err != ERROR_NONE)
    return err;
  for (i=0; (i < length) && (err == ERROR_NONE); i++) {
    offset = 0L;
    for (int j=0; j < cfg->bytesPerReference; j++)
      offset = (offset << 8) | (long)(*(ptr++));
    if (offset >= cfg->objectCount)
      return ERROR_INVALID_REFERENCE;
    offset = cfg->offsetTable[offset];
    err = readObject(cfg, offset, &(dict->data.keys[i].key));
  }
  for (i=0; (i < length) && (err == ERROR_NONE); i++) {
    offset = 0L;
    for (int j=0; j < cfg->bytesPerReference; j++)
      offset = (offset << 8) | (long)(*(ptr++));
    if (offset >= cfg->objectCount)
      return ERROR_INVALID_REFERENCE;
    offset = cfg->offsetTable[offset];
    err = readObject(cfg, offset, &(dict->data.keys[i].value));
  }
  if (err == ERROR_NONE)
    *dict2 = dict;
  return err;
}

int readObject(CONFIG *cfg, long offset, OBJECT **ptr2)
{
  int i;
  int length;
  int type;
  int err = ERROR_NONE;
  OBJECT *obj;
  unsigned char *ptr;

  *ptr2 = NULL;
  if ((size_t)offset >= cfg->bufferLength)
    return ERROR_INVALID_OFFSET;
  ptr = cfg->buffer + offset;
  type = *(ptr++);
  length = (type & 0x0F);
  type >>= 4;
  if ((type != 0) && (length == 0x0F)) {
    length = *(ptr++);
    if ((length >> 4) != 0x01)
      return ERROR_INVALID_OBJECT_LENGTH;
    i = (int)readInteger(ptr, length);
    ptr += (1 << (length & 0x03));
    length = i;
  }
  switch (type) {
    case 0x00:               //  Singleton
              err = createObject(type, length, 5, &obj);
              if (err != ERROR_NONE)
                return err;
              switch(length) {
                case 0:             //  NULL
                       strcpy(obj->data.text, "<Null/>");
                       break;
                case 8:             //  False
                       strcpy(obj->data.text, "<False/>");
                       break;
                case 9:             //  True
                       strcpy(obj->data.text, "<True/>");
                       break;
                case 15:             //  Fill
                       strcpy(obj->data.text, "<Fill/>");
                       break;
                default:             //  Illegal
                       strcpy(obj->data.text, "***Error***");
                       break;
              }
              break;
    case 0x01:               //  Integer
              err = createObject(type, length, 0, &obj);
              if (err != ERROR_NONE)
                return err;
              obj->data.integer = readInteger(ptr, length);
              break;
    case 0x02:               //  Float
              err = createObject(type, length, 0, &obj);
              if (err != ERROR_NONE)
                return err;
              obj->data.real = readDouble(ptr, length);
              break;
    case 0x03:               //  Date in Elapsed seconds
              err = createObject(type, length, 0, &obj);
              if (err != ERROR_NONE)
                return err;
              obj->data.date = (time_t)readDouble(ptr, length);
              break;
    case 0x04:               //  binary data
              err = createObject(type, length, length, &obj);
              if (err != ERROR_NONE)
                return err;
              for (i=0; i < length; i++)
                obj->data.binary[i] = *(ptr++);
              break;
    case 0x05:               //  ASCII string
              err = createObject(type, length, length, &obj);
              if (err != ERROR_NONE)
                return err;
              for (i=0; i < length; i++)
                obj->data.text[i] = (char)*(ptr++);
              obj->data.text[length] = '\0';
              break;
    case 0x06:               //  UTF16 string
              err = createObject(type, length, 2 * length, &obj);
              if (err != ERROR_NONE)
                return err;
              for (i=0; i < length; i++) {
                short int d = *(ptr++);
                obj->data.utf16[i] = (short int)((d << 8) | *(ptr++));
              }
              obj->data.utf16[length] = '\0';
              break;
    case 0x08:               //  UID
              err = createObject(type, length, 0, &obj);
              if (err != ERROR_NONE)
                return err;
              obj->data.uid = readUid(ptr, length);
              break;
    case 0x0A:               //  Array
              err = readArray(cfg, ptr, type, length, &obj);
              break;
    case 0x0D:               //  Dictionary
              err = readDictionary(cfg, ptr, type, length, &obj);
              break;
    default:
              fprintf(stderr, "Unknown object type: %d\n", type);
              err = createObject(type, length, 0, &obj);
              break;
  }
  if (err == ERROR_NONE)
    *ptr2 = obj;
  return err;
}

void displayHex(CONFIG *cfg, int data)
{
  static int in = 0;
  static char buffer[76];
  static long offset = 0L;
  static char hex[] = "0123456789ABCDEF";
  char *tmp;

  //  If -ve then flush buffer
  if (data < 0) {
    if (in > 0) {
      tmp = buffer + in * 3 + 8 + 2 * (in / 8);
      while (tmp < buffer+59)
        *(tmp++) = ' ';
      buffer[in+59] = '\0';
      printWithIndent(cfg, buffer, 1);
    }
    offset = 0L;
    in = 0;
    return;
  }

  //  If start of line add offset
  if (in == 0) {
    sprintf((char *)buffer, "%06lX  ", offset);
    offset += 16;
    buffer[8] = '\0';
  }

  //  Add Byte in hex
  tmp = buffer + in * 3 + 8 + 2 * (in / 8);
  *(tmp++) = hex[(data >> 4) & 0x0F];
  *(tmp++) = hex[data & 0x0F];
  *(tmp++) = ' ';

  //  Add to character buffer
  buffer[in+59] = (char)data;

  //  Check if midpoint of buffer
  if (++in == 8) {
    *(tmp++) = '-';
    *(tmp++) = ' ';
  }

  //  Check if buffer full
  if (in == 16) {
    buffer[75] = '\0';
    printWithIndent(cfg, buffer, 1);
    in = 0;
  }
  return;
}

int displayObject(CONFIG *cfg, OBJECT *obj, int raw)
{
  char text[32];
  switch (obj->type) {
    case 0x00:                 //  Singleton
              printWithIndent(cfg, obj->data.text, 1);
              break;
    case 0x01:                 //  Integer
              if (raw == 0)
                printWithIndent(cfg, "<number>", 0);
              sprintf(text, "%ld", obj->data.integer);
              outputText(cfg, text);
              if (raw == 0)
                outputText(cfg, "</number>\n");
              break;
    case 0x02:                 //  Float
              if (raw == 0)
                printWithIndent(cfg, "<float>", 0);
              sprintf(text, "%f", obj->data.real);
              outputText(cfg, text);
              if (raw == 0)
                outputText(cfg, "</float>\n");
              break;
    case 0x03:                 //  Date
              if (raw == 0)
                printWithIndent(cfg, "<date>", 0);
              outputText(cfg, ctime(&(obj->data.date)));
              if (raw == 0)
                outputText(cfg, "</date>\n");
              break;
    case 0x04:                 //  Binary data
              if (raw == 0)
                printWithIndent(cfg, "<data>", 1);
              indent += INDENT_INCREMENT;
              for (int i=0; i < obj->length; i++)
                displayHex(cfg, obj->data.binary[i]);
              displayHex(cfg, -1);
              indent -= INDENT_INCREMENT;
              if (raw == 0)
                printWithIndent(cfg, "</data>", 1);
              break;
    case 0x05:                 //  String
              if (raw == 0)
                printWithIndent(cfg, "<string>", 0);
              outputText(cfg, (obj->data.text));
              if (raw == 0)
                outputText(cfg, "</string>\n");
              break;
    case 0x06:                 //  UTF16 string
              if (raw == 0)
                printWithIndent(cfg, "<utf16>", 1);
              indent += INDENT_INCREMENT;
              for (int i=0; i < obj->length; i++)
                displayHex(cfg, obj->data.binary[i]);
              displayHex(cfg, -1);
              indent -= INDENT_INCREMENT;
              if (raw == 0)
                printWithIndent(cfg, "</utf16>", 1);
              break;
    case 0x08:                 //  UID
              if (raw == 0)
                printWithIndent(cfg, "<uid>", 0);
              sprintf(text, "%lu", obj->data.uid);
              outputText(cfg, text);
              if (raw == 0)
                outputText(cfg, "</uid>\n");
              break;
    case 0x0A:                 //  Array
              printWithIndent(cfg, "<array>", 1);
              indent += INDENT_INCREMENT;
              for (int i=0; i < obj->length; i++)
                displayObject(cfg, obj->data.objects[i], 0);
              indent -= INDENT_INCREMENT;
              printWithIndent(cfg, "</array>", 1);
              break;
    case 0x0D:                 //  Dictionary
              printWithIndent(cfg, "<dict>", 1);
              indent += INDENT_INCREMENT;
              for (int i=0; i < obj->length; i++) {
                printWithIndent(cfg, "<key>", 0);
                displayObject(cfg, obj->data.keys[i].key, 1);
                outputText(cfg, "</key>\n");
                displayObject(cfg, obj->data.keys[i].value, 0);
              }
              indent -= INDENT_INCREMENT;
              printWithIndent(cfg, "</dict>", 1);
              break;
    default:
              sprintf(text, "Cannot display type: %d\n", obj->type);
              outputText(cfg, text);
              break;
  }
  return ERROR_NONE;
}

int releaseObject(OBJECT *obj)
{
  int i;
  switch (obj->type) {
    case 0x00:                        //  Singleton
    case 0x01:                        //  Int
    case 0x02:                        //  Float
    case 0x03:                        //  Date
    case 0x04:                        //  Binary
    case 0x05:                        //  ASCII
    case 0x06:                        //  UTF16
    case 0x08:                        //  UID
              break;
    case 0x0A:                        //  Array
              for (i=0; i < obj->length; i++)
                releaseObject(obj->data.objects[i]);
              break;
    case 0x0D:                        //  Dictionary
              for (i=0; i < obj->length; i++) {
                releaseObject(obj->data.keys[i].key);
                releaseObject(obj->data.keys[i].value);
              }
              break;
  }
  free(obj);
  return ERROR_NONE;
}

int parsePlist(char **result, const char *data, int dataLength)
{
  CONFIG cfg;
  char   *ptr;
  OBJECT *obj;
  int err = ERROR_NONE;
  int version;
  long offset;
  long length;
  char text[32];

  //  Determine the file size and save
  cfg.buffer = (unsigned char *)data;
  cfg.bufferLength = dataLength;

  //  Preset the output buffer
  cfg.outputBufferLength = 0;
  cfg.outputBufferIn  = 0;
  cfg.outputBuffer = NULL;

  //  Read the header
  err = readHeader(&cfg);
  if (err != ERROR_NONE)
    return err;
  version = (int)(cfg.buffer[7] - '0');

  //  Read the trailer
  err = readTrailer(&cfg);
  if (err != ERROR_NONE)
    return err; //  ERROR_INVALID_TRAILER;

  //  Locate and read the root object
  offset = cfg.offsetTable[cfg.rootObjectReference];
  err = readObject(&cfg, offset, &obj);

  //  If no error display the root object and hence the whole object tree
  if (err != ERROR_NONE)
    return err;

  sprintf(text, "<plist version = \"1.%d\">\n", version);
  outputText(&cfg, text);
  displayObject(&cfg, obj, 0);
  outputText(&cfg, "</plist>\n");

  //  Create return data
  length = strlen((const char *)(cfg.outputBuffer));
  ptr = malloc(length + 1);
  *result = ptr;
  if (ptr != NULL) {
    for (int i=0; i < length; i++)
      *(ptr++) = cfg.outputBuffer[i];
    *ptr = '\0';
  }
  else
    err = ERROR_INSUFFICIENT_MEMORY;

  //  Release assigned memory
  releaseObject(obj);
  free(cfg.offsetTable);
  free(cfg.outputBuffer);
  return err;
}

/**  Wrapper functions
 *
 */

void freeResult(void *ptr)
{
  if (ptr != NULL)
    free(ptr);
  return;
}

static void plistFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  int resultLength;
  int errno = 0;
  char *result = NULL;
  printf("plistFunc, argc = %d", argc);
  assert( argc==1 );
  switch( sqlite3_value_type(argv[0]) ){
    case SQLITE_TEXT:
    case SQLITE_BLOB: {
      const char *data = sqlite3_value_text(argv[0]);
      const int dataLength = sqlite3_value_bytes(argv[0]);
      errno = parsePlist(&result, data, dataLength);
      if (errno == ERROR_NONE) {
        resultLength = strlen(result);
        sqlite3_result_text(context, result, resultLength, &freeResult);
      } else {
        sqlite3_result_text(context, data, dataLength, NULL);
      }
      break;
    }
    default: {
      sqlite3_result_null(context);
      break;
    }
  }
}

/**  RegisterExtensionPlist
 *
 *   Register the parsing functions with sqlite
 */

int RegisterExtensionPlist(sqlite3 *db)
{
  sqlite3_create_function(db, "plist", 1, 0, db, plistFunc, 0, 0);
}

#ifdef COMPILE_SQLITE_EXTENSIONS_AS_LOADABLE_MODULE

#ifdef _WIN32
__declspec(dllexport)
#endif

int sqlite3_sqliteplist_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);
  RegisterExtensionPlist(db);
  return rc;
}

#endif

