#include "EditDialog.h"
#include "ui_EditDialog.h"
#include "sqlitedb.h"
#include "Settings.h"
#include "src/qhexedit.h"
#include "docktextedit.h"
#include "FileDialog.h"
#include "Data.h"

#include <QMainWindow>
#include <QKeySequence>
#include <QShortcut>
#include <QImageReader>
#include <QBuffer>
#include <QModelIndex>
#include <QJsonDocument>
#include <QtXml/QDomDocument>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QClipboard>

//  The following section of code should really be in a separate class
//  but this is only a test!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define INCREMENT 4

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
  size_t textLength = strlen((const char *)text);
  size_t availableSpace = cfg->outputBufferLength - cfg->outputBufferIn;
  while (textLength >= availableSpace) {
    cfg->outputBufferLength += textLength + 1024;
    cfg->outputBuffer = (unsigned char *)realloc(cfg->outputBuffer, cfg->outputBufferLength);
    if (cfg->outputBuffer == NULL)
      return 1;
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
  return ERROR_NONE;
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

void displayObject(CONFIG *cfg, OBJECT *obj, int raw)
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
              indent += INCREMENT;
              for (int i=0; i < obj->length; i++)
                displayHex(cfg, obj->data.binary[i]);
              displayHex(cfg, -1);
              indent -= INCREMENT;
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
              indent += INCREMENT;
              for (int i=0; i < obj->length; i++)
                displayHex(cfg, obj->data.binary[i]);
              displayHex(cfg, -1);
              indent -= INCREMENT;
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
              indent += INCREMENT;
              for (int i=0; i < obj->length; i++)
                displayObject(cfg, obj->data.objects[i], 0);
              indent -= INCREMENT;
              printWithIndent(cfg, "</array>", 1);
              break;
    case 0x0D:                 //  Dictionary
              printWithIndent(cfg, "<dict>", 1);
              indent += INCREMENT;
              for (int i=0; i < obj->length; i++) {
                printWithIndent(cfg, "<key>", 0);
                displayObject(cfg, obj->data.keys[i].key, 1);
                outputText(cfg, "</key>\n");
                displayObject(cfg, obj->data.keys[i].value, 0);
              }
              indent -= INCREMENT;
              printWithIndent(cfg, "</dict>", 1);
              break;
    default:
              sprintf(text, "Cannot display type: %d\n", obj->type);
              outputText(cfg, text);
              break;
  }
  return;
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

QByteArray parsePlist(QByteArray data)
{
  CONFIG cfg;
  OBJECT *obj;
  int err = ERROR_NONE;
  int version;
  long offset;
  long length;
  char text[32];
  QByteArray result;

  //  Determine the file size and save
  cfg.bufferLength = data.length();
  cfg.buffer = (unsigned char *)data.data();

  //  Preset the output buffer
  cfg.outputBufferLength = 0;
  cfg.outputBufferIn  = 0;
  cfg.outputBuffer = NULL;

  //  Read the header
  err = readHeader(&cfg);
  if (err != ERROR_NONE)
    return NULL;  //  err;
  version = (int)(cfg.buffer[7] - '0');

  //  Read the trailer
  err = readTrailer(&cfg);
  if (err != ERROR_NONE)
    return NULL; //  ERROR_INVALID_TRAILER;

  //  Locate and read the root object
  offset = cfg.offsetTable[cfg.rootObjectReference];
  err = readObject(&cfg, offset, &obj);

  //  If no error display the root object and hence the whole object tree
  if (err != ERROR_NONE)
    return NULL;

  sprintf(text, "<plist version = \"1.%d\">\n", version);
  outputText(&cfg, text);
  displayObject(&cfg, obj, 0);
  outputText(&cfg, "</plist>\n");

  //  Create return data
  length = strlen((const char *)(cfg.outputBuffer));
  result.resize(length);
  for (int i=0; i < length; i++)
    result[i] = cfg.outputBuffer[i];

  //  Release assigned memory
  releaseObject(obj);
  free(cfg.offsetTable);
  free(cfg.outputBuffer);

  return result;
}

//  End of extra data

EditDialog::EditDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::EditDialog),
      currentIndex(QModelIndex()),
      dataSource(TextBuffer),
      dataType(Null),
      isReadOnly(true)
{
    ui->setupUi(this);

    // Add Ctrl-Enter (Cmd-Enter on OSX) as a shortcut for the Apply button
    ui->buttonApply->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return));
    ui->buttonApply->setToolTip(ui->buttonApply->toolTip() + " [" + ui->buttonApply->shortcut().toString(QKeySequence::NativeText) + "]");

    QHBoxLayout* hexLayout = new QHBoxLayout(ui->editorBinary);
    hexEdit = new QHexEdit(this);
    hexLayout->addWidget(hexEdit);
    hexEdit->setOverwriteMode(false);

    QHBoxLayout* sciLayout = new QHBoxLayout(ui->editorSci);
    sciEdit = new DockTextEdit(this);
    sciLayout->addWidget(sciEdit);

    QShortcut* ins = new QShortcut(QKeySequence(Qt::Key_Insert), this);
    connect(ins, SIGNAL(activated()), this, SLOT(toggleOverwriteMode()));

    connect(ui->editorText, SIGNAL(textChanged()), this, SLOT(updateApplyButton()));
    connect(hexEdit, SIGNAL(dataChanged()), this, SLOT(updateApplyButton()));

    connect(sciEdit, SIGNAL(textChanged()), this, SLOT(updateApplyButton()));
    connect(sciEdit, SIGNAL(textChanged()), this, SLOT(editTextChanged()));

    // Create shortcuts for the widgets that doesn't have its own print action or printing mechanism.
    QShortcut* shortcutPrintText = new QShortcut(QKeySequence::Print, ui->editorText, nullptr, nullptr, Qt::WidgetShortcut);
    connect(shortcutPrintText, &QShortcut::activated, this, &EditDialog::openPrintDialog);

    // Add actions to editors that have a context menu based on actions. This also activates the shortcuts.
    ui->editorImage->addAction(ui->actionPrintImage);
    ui->editorBinary->addAction(ui->actionPrint);
    ui->editorBinary->addAction(ui->actionCopyHexAscii);

    mustIndentAndCompact = Settings::getValue("databrowser", "indent_compact").toBool();
    ui->buttonIndent->setChecked(mustIndentAndCompact);

    ui->buttonAutoSwitchMode->setChecked(Settings::getValue("databrowser", "auto_switch_mode").toBool());

    reloadSettings();
}

EditDialog::~EditDialog()
{
    Settings::setValue("databrowser", "indent_compact",  mustIndentAndCompact);
    Settings::setValue("databrowser", "auto_switch_mode", ui->buttonAutoSwitchMode->isChecked());
    delete ui;
}

void EditDialog::setCurrentIndex(const QModelIndex& idx)
{
    currentIndex = QPersistentModelIndex(idx);

    QByteArray data = idx.data(Qt::EditRole).toByteArray();
    loadData(data);
    updateCellInfoAndMode(data);

    ui->buttonApply->setDisabled(true);
}

void EditDialog::showEvent(QShowEvent*)
{
    // Whenever the dialog is shown, position it at the center of the parent dialog
    QMainWindow* parentDialog = qobject_cast<QMainWindow*>(parent());
    if(parentDialog)
    {
        move(parentDialog->x() + parentDialog->width() / 2 - width() / 2,
            parentDialog->y() + parentDialog->height() / 2 - height() / 2);
    }
}

void EditDialog::reject()
{
    // We override this, to ensure the Escape key doesn't make the Edit Cell
    // dock go away
    return;
}

// Loads data from a cell into the Edit Cell window
void EditDialog::loadData(const QByteArray& data)
{
    QImage img;
    QString textData;
    QByteArray data2;

    // Clear previously removed BOM
    removedBom.clear();

    // Determine the data type, saving that info in the class variable
    dataType = checkDataType(data);

    // Get the current editor mode (eg text, hex, image, base64, json or xml mode)
    int editMode = ui->comboMode->currentIndex();

    // Data type specific handling
    switch (dataType) {
    case Null:
        // Set enabled any of the text widgets
        ui->editorText->setEnabled(true);
        sciEdit->setEnabled(true);
        switch (editMode) {
        case Base64Editor:
            // No conversion is required
            // No 'break' is intentional
        case TextEditor:
            // The text widget buffer is now the main data source
            dataSource = TextBuffer;

            // Empty the text editor contents, then enable text editing
            ui->editorText->clear();

            break;

        case JsonEditor:
        case XmlEditor:

            // The JSON widget buffer is now the main data source
            dataSource = SciBuffer;

            // Empty the text editor contents, then enable text editing
            sciEdit->clear();

            break;

        case HexEditor:
            // The hex widget buffer is now the main data source
            dataSource = HexBuffer;

            // Load the Null into the hex editor
            hexEdit->setData(data);

            break;

        case ImageViewer:
            // The hex widget buffer is now the main data source
            dataSource = HexBuffer;

            // Clear any image from the image viewing widget
            ui->editorImage->setPixmap(QPixmap(0,0));

            // Load the Null into the hex editor
            hexEdit->setData(data);

            break;
        }
        break;

    case Text:
    case JSON:
        // Can be stored in any widget, except the ImageViewer

        switch (editMode) {
        case TextEditor:
            setDataInBuffer(data, TextBuffer);
            break;
        case JsonEditor:
        case XmlEditor:
            setDataInBuffer(data, SciBuffer);
            break;
        case Base64Editor:
            data2 = QByteArray::fromBase64(data);
            setDataInBuffer(data2, TextBuffer);
            break;
        case HexEditor:
            setDataInBuffer(data, HexBuffer);
            break;
        case ImageViewer:
            // The image viewer cannot hold data nor display text.

            // Clear any image from the image viewing widget
            ui->editorImage->setPixmap(QPixmap(0,0));

            // Load the text into the text editor
            setDataInBuffer(data, TextBuffer);

            break;
        }
        break;

    case Plist:
        //  Convert data to XML format
        data2 = parsePlist(data);

        switch (editMode) {
        case TextEditor:
        case Base64Editor:
            setDataInBuffer(data2, TextBuffer);
            break;
        case JsonEditor:
        case XmlEditor:
            setDataInBuffer(data, SciBuffer);
            break;
        case HexEditor:
            setDataInBuffer(data, HexBuffer);
            break;
        case ImageViewer:
            // The image viewer cannot hold data nor display text.

            // Clear any image from the image viewing widget
            ui->editorImage->setPixmap(QPixmap(0,0));

            // Load the text into the text editor
            setDataInBuffer(data2, TextBuffer);
            break;
        }
        break;

    case Image:
        // Image data is kept in the hex widget, mainly for safety.  If we
        // stored it in the editorImage widget instead, it would be a pixmap
        // and there's no good way to restore that back to the original
        // (pristine) image data.  eg image metadata would be lost
        setDataInBuffer(data, HexBuffer);

        // Update the display if in text edit or image viewer mode
        switch (editMode) {
        case TextEditor:
            // Disable text editing, and use a warning message as the contents
            ui->editorText->setText(QString("<i>" %
                    tr("Image data can't be viewed in this mode.") % "<br/>" %
                    tr("Try switching to Image or Binary mode.") %
                    "</i>"));
            ui->editorText->setEnabled(false);
            break;

        case XmlEditor:
        case JsonEditor:
            // Disable text editing, and use a warning message as the contents
            sciEdit->setText(tr("Image data can't be viewed in this mode.") % '\n' %
                             tr("Try switching to Image or Binary mode."));
            sciEdit->setEnabled(false);

            break;

        case ImageViewer:
            // Load the image into the image viewing widget
            if (img.loadFromData(data)) {
                ui->editorImage->setPixmap(QPixmap::fromImage(img));
            }
            break;
        }
        break;
    case SVG:
        // Set the XML data in any buffer or update image in image viewer mode
        switch (editMode) {
        case TextEditor:
            setDataInBuffer(data, TextBuffer);
            break;

        case JsonEditor:
        case XmlEditor:

            setDataInBuffer(data, SciBuffer);
            break;

        case HexEditor:

            setDataInBuffer(data, HexBuffer);
            break;

        case ImageViewer:
            // Set data in the XML (Sci) Buffer and load the SVG Image
            setDataInBuffer(data, SciBuffer);
            sciEdit->setLanguage(DockTextEdit::XML);

            // Load the image into the image viewing widget
            if (img.loadFromData(data)) {
                ui->editorImage->setPixmap(QPixmap::fromImage(img));
            }
            break;
        }
        break;

    default:

        // The data seems to be general binary data, which is always loaded
        // into the hex widget (the only safe place for it)

        // Load the data into the hex buffer
        setDataInBuffer(data, HexBuffer);

        switch (editMode) {
        case TextEditor:
            // Disable text editing, and use a warning message as the contents
            ui->editorText->setText(QString("<i>" %
                    tr("Binary data can't be viewed in this mode.") % "<br/>" %
                    tr("Try switching to Binary mode.") %
                    "</i>"));
            ui->editorText->setEnabled(false);
            break;

         case JsonEditor:
         case XmlEditor:
            // Disable text editing, and use a warning message as the contents
             sciEdit->setText(QString(tr("Binary data can't be viewed in this mode.") % '\n' %
                                      tr("Try switching to Binary mode.")));
             sciEdit->setEnabled(false);
            break;

        case ImageViewer:
            // Clear any image from the image viewing widget
            ui->editorImage->setPixmap(QPixmap(0,0));
            break;

        case Base64Editor:
            data2 = QByteArray::fromBase64(data);
            setDataInBuffer(data2, TextBuffer);
            break;

        }
    }
}

void EditDialog::importData()
{
    // Get list of supported image file formats to include them in the file dialog filter
    QString image_formats;
    QList<QByteArray> image_formats_list = QImageReader::supportedImageFormats();
    for(int i=0;i<image_formats_list.size();++i)
        image_formats.append(QString("*.%1 ").arg(QString::fromUtf8(image_formats_list.at(i))));
    // Chop last space
    image_formats.chop(1);

    QStringList filters;
    filters << tr("Text files (*.txt)") <<
        tr("JSON files (*.json)") <<
        tr("XML files (*.xml)") <<
        tr("Image files (%1)").arg(image_formats) <<
        tr("Binary files (*.bin)") << tr("All files (*)");

    QString selectedFilter;

    // Get the current editor mode (eg text, hex, image, json or xml mode)
    int mode = ui->comboMode->currentIndex();

    // Set as selected filter the appropriate for the current mode.
    switch (mode) {
    case TextEditor:
        selectedFilter = tr("Text files (*.txt)");
        break;
    case HexEditor:
        selectedFilter = tr("Binary files (*.bin)");
        break;
    case ImageViewer:
        selectedFilter = tr("Image files (%1)").arg(image_formats);
        break;
    case JsonEditor:
        selectedFilter = tr("JSON files (*.json)");
        break;
    case XmlEditor:
        selectedFilter = tr("XML files (*.xml)");
        break;
    }
    QString fileName = FileDialog::getOpenFileName(
                OpenDataFile,
                this,
                tr("Choose a file to import")
#ifndef Q_OS_MAC // Filters on OS X are buggy
                , filters.join(";;")
                , &selectedFilter
#endif
                );
    if(QFile::exists(fileName))
    {
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray d = file.readAll();
            loadData(d);
            file.close();

            // Update the cell data info in the bottom left of the Edit Cell
            // and update mode (if required) to the just imported data type.
            updateCellInfoAndMode(d);
        }
    }
}

void EditDialog::exportData()
{
    QStringList filters;
    switch (dataType) {
    case Image: {
        // Images get special treatment.
        // Determine the likely filename extension.
        QByteArray cellData = hexEdit->data();
        QBuffer imageBuffer(&cellData);
        QImageReader imageReader(&imageBuffer);
        QString imageFormat = imageReader.format();
        filters << tr("%1 Image").arg(imageFormat.toUpper()) % " (*." % imageFormat.toLower() % ")";
        break;
    }
    case Binary:
        filters << tr("Binary files (*.bin)");
        break;
    case Text:
        // Base the XML case on the mode, not the data type since XML detection is currently not implemented.
        if (ui->comboMode->currentIndex() == XmlEditor)
            filters << tr("XML files (*.xml)") << tr("Text files (*.txt)");
        else
            filters << tr("Text files (*.txt)") << tr("XML files (*.xml)");
        break;
    case JSON:
        filters << tr("JSON files (*.json)");
        break;
    case SVG:
        filters << tr("SVG files (*.svg)");
        break;
    case Null:
        return;
    }

    if (dataSource == HexBuffer)
        filters << tr("Hex dump files (*.txt)");

    filters << tr("All files (*)");

    QString selectedFilter = filters.first();
    QString fileName = FileDialog::getSaveFileName(
                CreateDataFile,
                this,
                tr("Choose a filename to export data"),
                filters.join(";;"),
                /* defaultFileName */ QString(),
                &selectedFilter);
    if(fileName.size() > 0)
    {
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly))
        {
          switch (dataSource) {
          case HexBuffer:
              // Data source is the hex buffer
              // If text option has been selected, the readable representation of the content is saved to file.
              if (selectedFilter == tr("Hex dump files (*.txt)"))
                  file.write(hexEdit->toReadableString().toUtf8());
              else
                  file.write(hexEdit->data());
              break;
          case TextBuffer:
              // Data source is the text buffer
              file.write(ui->editorText->toPlainText().toUtf8());
              break;
          case SciBuffer:
              // Data source is the Scintilla buffer
              file.write(sciEdit->text().toUtf8());
              break;
            }
            file.close();
        }
    }
}

void EditDialog::setNull()
{
    ui->editorText->clear();
    ui->editorImage->clear();
    hexEdit->setData(QByteArray());
    sciEdit->clear();
    dataType = Null;
    removedBom.clear();

    // The text editors don't know the difference between an empty string
    // and a NULL, so we need to record NULL outside of that
    dataType = Null;

    // Ensure the text (plain and Scintilla) editors are enabled
    ui->editorText->setEnabled(true);
    sciEdit->setEnabled(true);

    // Update the cell data info in the bottom left of the Edit Cell
    // The mode is also (if required) updated to text since it gives
    // the better visual clue of containing a NULL value (placeholder).
    updateCellInfoAndMode(hexEdit->data());

    ui->editorText->setFocus();
}

void EditDialog::updateApplyButton()
{
    if (!isReadOnly)
        ui->buttonApply->setEnabled(true);
}

bool EditDialog::promptInvalidData(const QString& dataType, const QString& errorString)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
      this,
      tr("Invalid data for this mode"),
      tr("The cell contains invalid %1 data. Reason: %2. Do you really want to apply it to the cell?").arg(dataType, errorString),
      QMessageBox::Apply | QMessageBox::Cancel);
    return (reply == QMessageBox::Apply);
}

void EditDialog::accept()
{
    if(!currentIndex.isValid())
        return;

    if (dataType == Null) {
        emit recordTextUpdated(currentIndex, hexEdit->data(), true);
        return;
    }

    switch (dataSource) {
    case TextBuffer:
    {
        QString oldData = currentIndex.data(Qt::EditRole).toString();
        QString newData = removedBom + ui->editorText->toPlainText();
        // Check first for null case, otherwise empty strings cannot overwrite NULL values
        if ((currentIndex.data(Qt::EditRole).isNull() && dataType != Null) || oldData != newData)
            // The data is different, so commit it back to the database
            emit recordTextUpdated(currentIndex, removedBom + newData.toUtf8(), false);
        break;
    }
    case SciBuffer:
        switch (sciEdit->language()) {
        case DockTextEdit::JSON:
        {
            QString oldData = currentIndex.data(Qt::EditRole).toString();

            QString newData;
            QJsonParseError parseError;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(sciEdit->text().toUtf8(), &parseError);
            bool proceed;

            sciEdit->clearErrorIndicators();
            if (parseError.error != QJsonParseError::NoError)
                sciEdit->setErrorIndicator(parseError.offset-1);

            if (!jsonDoc.isNull()) {
                if (mustIndentAndCompact)
                    // Compact the JSON data before storing
                    newData = QString(jsonDoc.toJson(QJsonDocument::Compact));
                else
                    newData = sciEdit->text();
                proceed = (oldData != newData);

            } else {
                newData = sciEdit->text();
                proceed = (oldData != newData && promptInvalidData("JSON", parseError.errorString()));
            }
            if (proceed)
                // The data is different, so commit it back to the database
                emit recordTextUpdated(currentIndex, newData.toUtf8(), false);
        }
        break;
        case DockTextEdit::XML:
        {
            QString oldData = currentIndex.data(Qt::EditRole).toString();

            QString newData;
            QDomDocument xmlDoc;
            QString errorMsg;
            int errorLine, errorColumn;

            bool isValid = xmlDoc.setContent(sciEdit->text().toUtf8(), true, &errorMsg, &errorLine, &errorColumn);
            bool proceed;

            sciEdit->clearErrorIndicators();
            if (!isValid) {
                sciEdit->setErrorIndicator(errorLine-1, errorColumn-1, errorLine, 0);
                newData = sciEdit->text();
                proceed = (oldData != newData && promptInvalidData("XML", errorMsg));
            } else {
                if (mustIndentAndCompact)
                    // Compact the XML data before storing. If indent is -1, no whitespace at all is added.
                    newData = xmlDoc.toString(-1);
                else
                    newData = sciEdit->text();
                proceed = (oldData != newData);
            }
            if (proceed)
                // The data is different, so commit it back to the database
                emit recordTextUpdated(currentIndex, newData.toUtf8(), false);
        }
        break;
        }
        break;
    case HexBuffer:
        // The data source is the hex widget buffer, thus binary data
        QByteArray oldData = currentIndex.data(Qt::EditRole).toByteArray();
        QByteArray newData = hexEdit->data();
        if (newData != oldData)
            emit recordTextUpdated(currentIndex, newData, true);
        break;
    }
}

void EditDialog::setDataInBuffer(const QByteArray& data, DataSources source)
{
    dataSource = source;
    QString textData;

    // 1) Perform validation and text formatting (if applicable).
    // 2) Set the text in the corresponding editor widget (the text widget for the Image case).
    // 3) Enable the widget.
    switch (dataSource) {
    case TextBuffer:
    {
        // Load the text into the text editor, remove BOM first if there is one
        QByteArray dataWithoutBom = data;
        removedBom = removeBom(dataWithoutBom);

        textData = QString::fromUtf8(dataWithoutBom.constData(), dataWithoutBom.size());
        ui->editorText->setPlainText(textData);

        // Select all of the text by default (this is useful for simple text data that we usually edit as a whole)
        ui->editorText->selectAll();
        ui->editorText->setEnabled(true);
        break;
    }
    case SciBuffer:
        switch (sciEdit->language()) {
        case DockTextEdit::JSON:
        {
            QJsonParseError parseError;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray(data.constData(), data.size()), &parseError);

            if (mustIndentAndCompact && !jsonDoc.isNull()) {
                // Load indented JSON into the JSON editor
                textData = QString(jsonDoc.toJson(QJsonDocument::Indented));
            } else {
                // Fallback case. The data is not yet valid JSON or no auto-formatting applied.
                textData = QString::fromUtf8(data.constData(), data.size());
            }
            sciEdit->setText(textData);

            sciEdit->clearErrorIndicators();
            if (parseError.error != QJsonParseError::NoError)
                sciEdit->setErrorIndicator(parseError.offset-1);
            sciEdit->setEnabled(true);

        }

        break;

        case DockTextEdit::XML:
        {
            QString errorMsg;
            int errorLine, errorColumn;
            QDomDocument xmlDoc;
            bool isValid = xmlDoc.setContent(data, true, &errorMsg, &errorLine, &errorColumn);

            if (mustIndentAndCompact && isValid) {
                // Load indented XML into the XML editor
                textData = xmlDoc.toString(Settings::getValue("editor", "tabsize").toInt());
            } else {
                // Fallback case. The data is not yet valid JSON or no auto-formatting applied.
                textData = QString::fromUtf8(data.constData(), data.size());
            }
            sciEdit->setText(textData);

            sciEdit->clearErrorIndicators();
            if (!isValid)
                // Adjust line and column by one (Scintilla starts at 1 and QDomDocument at 0)
                sciEdit->setErrorIndicator(errorLine-1, errorColumn-1, errorLine, 0);
            sciEdit->setEnabled(true);

        }
        break;
        }
        break;
    case HexBuffer:
        hexEdit->setData(data);
        hexEdit->setEnabled(true);

        break;
    }

}

// Called when the user manually changes the "Mode" drop down combobox
void EditDialog::editModeChanged(int newMode)
{
    QByteArray data2;
    ui->buttonIndent->setEnabled(newMode == JsonEditor || newMode == XmlEditor);
    setStackCurrentIndex(newMode);

    // * If the dataSource is the text buffer, the data is always text *
    switch (dataSource) {
    case TextBuffer:
        switch (newMode) {
        case TextEditor: // Switching to the text editor
            // Nothing to do, as the text is already in the text buffer
            break;

        case JsonEditor: // Switching to one of the Scintilla editor modes
        case XmlEditor:

            setDataInBuffer(ui->editorText->toPlainText().toUtf8(), SciBuffer);
            break;

        case HexEditor: // Switching to the hex editor
            // Convert the text widget buffer for the hex widget
            // The hex widget buffer is now the main data source
            setDataInBuffer(removedBom + ui->editorText->toPlainText().toUtf8(), HexBuffer);
            break;

        case Base64Editor:
            data2 = QByteArray::fromBase64(ui->editorText->toPlainText().toUtf8());
            setDataInBuffer(data2, TextBuffer);
            break;

        case ImageViewer:
            // Clear any image from the image viewing widget
            ui->editorImage->setPixmap(QPixmap(0,0));
            break;
        }
        break;
    case HexBuffer:

        // * If the dataSource is the hex buffer, the contents could be anything
        //   so we just pass it to our loadData() function to handle *
        // Note that we have already set the editor, as loadData() relies on it
        // being current

        // Load the data into the appropriate widget, as done by loadData()
        loadData(hexEdit->data());
        break;
    case SciBuffer:
        switch (newMode) {
        case TextEditor: // Switching to the text editor
            // Convert the text widget buffer for the JSON widget
            setDataInBuffer(sciEdit->text().toUtf8(), TextBuffer);
            break;

        case HexEditor: // Switching to the hex editor
            // Convert the text widget buffer for the hex widget
            setDataInBuffer(sciEdit->text().toUtf8(), HexBuffer);
            break;
        case ImageViewer:
        {
            // When SVG format, load the image, else clear it.
            QByteArray data = sciEdit->text().toUtf8();
            dataType = checkDataType(data);
            if (dataType == SVG) {
                QImage img;

                if (img.loadFromData(data))
                    ui->editorImage->setPixmap(QPixmap::fromImage(img));
                else
                    // Clear any image from the image viewing widget
                    ui->editorImage->setPixmap(QPixmap(0,0));
            }
        }
        break;

        case JsonEditor: // Switching to the JSON editor
        case XmlEditor: // Switching to the XML editor
            // The text is already in the Sci buffer but we need to perform the necessary formatting.
            setDataInBuffer(sciEdit->text().toUtf8(), SciBuffer);

            break;
        }
    }
}

// Called for every keystroke in the text editor (only)
void EditDialog::editTextChanged()
{
    if (dataSource == TextBuffer || dataSource == SciBuffer) {

        // Update the cell info in the bottom left manually.  This is because
        // updateCellInfoAndMode() only works with QByteArray's (for now)
        int dataLength;
        bool isModified;

        if(dataSource == TextBuffer) {
            // TextBuffer
            dataLength = ui->editorText->toPlainText().length();
            isModified = ui->editorText->document()->isModified();
        } else {
            // SciBuffer
            dataLength = sciEdit->text().length();
            isModified = sciEdit->isModified();
        }
        // Data has been changed in the text editor, so it can't be a NULL
        // any more. It hasn't been validated yet, so it cannot be JSON nor XML.
        if (dataType == Null && isModified)
            dataType = Text;

        if (dataType != Null) {
            ui->editorText->setStyleSheet("");
            ui->editorText->setPlaceholderText("");
            ui->labelType->setText(tr("Type of data currently in cell: Text / Numeric"));
        }
        ui->labelSize->setText(tr("%n char(s)", "", dataLength));
    }
}

void EditDialog::setMustIndentAndCompact(bool enable)
{
    mustIndentAndCompact = enable;

    // Indent or compact if necessary. If data has changed (button Apply indicates so), reload from the widget, else from the table.
    if (ui->buttonApply->isEnabled()) {
        setDataInBuffer(sciEdit->text().toUtf8(), SciBuffer);
    } else
        setCurrentIndex(currentIndex);
}

// Determine the type of data in the cell
int EditDialog::checkDataType(const QByteArray& data)
{
    QByteArray cellData = data;

    // Check for NULL data type
    if (cellData.isNull()) {
        return Null;
    }

    // Check if it's an image. First do a quick test by calling canRead() which only checks the first couple of bytes or so. Only if
    // that returned true, do a more sophisticated test of the data. This way we get both, good performance and proper data checking.
    QBuffer imageBuffer(&cellData);
    QImageReader readerBuffer(&imageBuffer);
    QString imageFormat = readerBuffer.format();
    if(readerBuffer.canRead() && !readerBuffer.read().isNull())
        return imageFormat == "svg" ? SVG : Image;

    // Check if it's text only
    if(isTextOnly(cellData))
    {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(cellData);
        if (!jsonDoc.isNull())
            return JSON;
        else
            return Text;
    }

    //  Check if it is a 'PLIST' cell (starts "bplist\0\0")
    if ((data[0] == 'b') && (data[1] == 'p')) {
      return Plist;
    }

    // It's none of the above, so treat it as general binary data
    return Binary;
}

void EditDialog::toggleOverwriteMode()
{
    static bool currentMode = false;
    currentMode = !currentMode;

    hexEdit->setOverwriteMode(currentMode);
    ui->editorText->setOverwriteMode(currentMode);
    sciEdit->setOverwriteMode(currentMode);
}

void EditDialog::setFocus()
{
    QDialog::setFocus();

    // Set the focus to the editor widget. The idea here is that setting focus
    // to the dock itself doesn't make much sense as it's just a frame; you'd
    // have to tab to the editor which is what you most likely want to use. So
    // in order to save the user from doing this we explicitly set the focus
    // to the current editor.
    int editMode = ui->editorStack->currentIndex();

    switch (editMode) {
    case TextEditor:
        ui->editorText->setFocus();
        ui->editorText->selectAll();
        break;
    case HexEditor:
        hexEdit->setFocus();
        break;
    case SciEditor:
        sciEdit->setFocus();
        break;
    case ImageViewer:
        // Nothing to do
        break;
    }

}

// Enables or disables the Apply, Null, & Import buttons in the Edit Cell dock.
// Sets or unsets read-only properties for the editors.
void EditDialog::setReadOnly(bool ro)
{
    isReadOnly = ro;
    QPalette textEditPalette = ui->editorText->palette();

    ui->buttonApply->setEnabled(!ro);
    ui->buttonNull->setEnabled(!ro);
    ui->buttonImport->setEnabled(!ro);

    ui->editorText->setReadOnly(ro);
    sciEdit->setReadOnly(ro);
    hexEdit->setReadOnly(ro);

    // This makes the caret being visible for selection, although the editor is read-only.
    Qt::TextInteractionFlags textFlags = ro? Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard : Qt::TextEditorInteraction;
    ui->editorText->setTextInteractionFlags(textFlags);

    // If read-only, set the Disabled palette settings for the (in)active groups, so the user gets a hint about the text being read-only.
    // This should be set also for the Scintilla widget, but it isn't working for that.
    if (ro) {
        textEditPalette.setColor(QPalette::Active, QPalette::Base, textEditPalette.color(QPalette::Disabled, QPalette::Base));
        textEditPalette.setColor(QPalette::Inactive, QPalette::Base, textEditPalette.color(QPalette::Disabled, QPalette::Base));
        textEditPalette.setColor(QPalette::Active, QPalette::Highlight, textEditPalette.color(QPalette::Disabled, QPalette::Highlight));
        textEditPalette.setColor(QPalette::Inactive, QPalette::Highlight, textEditPalette.color(QPalette::Disabled, QPalette::Highlight));
        textEditPalette.setColor(QPalette::Active, QPalette::HighlightedText, textEditPalette.color(QPalette::Disabled, QPalette::HighlightedText));
        textEditPalette.setColor(QPalette::Inactive, QPalette::HighlightedText, textEditPalette.color(QPalette::Disabled, QPalette::HighlightedText));
        ui->editorText->setPalette(textEditPalette);
    } else {
        // Restore default palette
        ui->editorText->setPalette(QPalette());
    }
}

void EditDialog::switchEditorMode(bool autoSwitchForType)
{
    if (autoSwitchForType) {
        // Switch automatically the editing mode according to the detected data.
        switch (dataType) {
        case Image:
            ui->comboMode->setCurrentIndex(ImageViewer);
            break;
        case Binary:
            ui->comboMode->setCurrentIndex(HexEditor);
            break;
        case Null:
        case Text:
            ui->comboMode->setCurrentIndex(TextEditor);
            break;
        case JSON:
            ui->comboMode->setCurrentIndex(JsonEditor);
            break;
        case SVG:
            ui->comboMode->setCurrentIndex(XmlEditor);
            break;
        }
    }
}

// Update the information labels in the bottom left corner of the dialog
// and switches the editor mode, if required, according to the detected data type.
void EditDialog::updateCellInfoAndMode(const QByteArray& data)
{
    QByteArray cellData = data;

    switchEditorMode(ui->buttonAutoSwitchMode->isChecked());

    // Image data needs special treatment
    if (dataType == Image || dataType == SVG) {
        QBuffer imageBuffer(&cellData);
        QImageReader imageReader(&imageBuffer);

        // Display the image format
        QString imageFormat = imageReader.format();

        ui->labelType->setText(tr("Type of data currently in cell: %1 Image").arg(imageFormat.toUpper()));

        // Display the image dimensions and size
        QSize imageDimensions = imageReader.size();
        int imageSize = cellData.size();

        QString labelSizeText = tr("%1x%2 pixel(s)").arg(imageDimensions.width()).arg(imageDimensions.height()) + ", " + humanReadableSize(imageSize);

        ui->labelSize->setText(labelSizeText);

        return;
    }

    // Use a switch statement for the other data types to keep things neat :)
    switch (dataType) {
    case Null:
        // NULL data type
        ui->labelType->setText(tr("Type of data currently in cell: NULL"));
        ui->labelSize->setText(tr("%n byte(s)", "", 0));
        ui->editorText->setStyleSheet("QTextEdit{ font-style: italic; }");
        ui->editorText->setPlaceholderText(Settings::getValue("databrowser", "null_text").toString());
        break;

    case Text: {
        // Text only
        // Determine the length of the cell text in characters (possibly different to number of bytes).
        int textLength = QString(cellData).length();
        ui->labelType->setText(tr("Type of data currently in cell: Text / Numeric"));
        ui->labelSize->setText(tr("%n char(s)", "", textLength));
        break;
    }
    case JSON: {
        // Valid JSON
        // Determine the length of the cell text in characters (possibly different to number of bytes).
        int jsonLength = QString(cellData).length();
        ui->labelType->setText(tr("Type of data currently in cell: Valid JSON"));
        ui->labelSize->setText(tr("%n char(s)", "", jsonLength));
        break;
    }
    default:

        // Determine the length of the cell data
        int dataLength = cellData.length();
        // If none of the above data types, consider it general binary data
        ui->labelType->setText(tr("Type of data currently in cell: Binary"));
        ui->labelSize->setText(tr("%n byte(s)", "", dataLength));
        break;
    }
}

QString EditDialog::humanReadableSize(double byteCount) const
{
    QStringList units;
    units << "" << "Ki" << "Mi" << "Gi" << "Ti" << "Pi" << "Ei" << "Zi";

    for(const QString& unit : units)
    {
        if(fabs(byteCount) < 1024.0)
        {
            QString size = QString::number(byteCount, 'f', 2);
            return size + " " + unit + "B";
        }

        byteCount /= 1024.0;
    }

    QString yiUnit = "Yi";
    QString size = QString::number(byteCount, 'f', 2);

    return size + " " + yiUnit + "B";
}

void EditDialog::reloadSettings()
{
    // Set the databrowser font for the text editor but the (SQL) editor
    // font for hex editor, since it needs a Monospace font and the
    // databrowser font would be usually of variable width.
    QFont textFont(Settings::getValue("databrowser", "font").toString());
    textFont.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());
    ui->editorText->setFont(textFont);

    QFont hexFont(Settings::getValue("editor", "font").toString());
    hexFont.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());
    hexEdit->setFont(hexFont);

    sciEdit->reloadSettings();
}

void EditDialog::setStackCurrentIndex(int editMode)
{
    switch (editMode) {
    case TextEditor:
    case HexEditor:
    case ImageViewer:
        // General case: switch to the selected editor
        ui->editorStack->setCurrentIndex(editMode);
        break;
    case JsonEditor:
        // Scintilla case: switch to the single Scintilla editor and set language
        ui->editorStack->setCurrentIndex(SciEditor);
        sciEdit->setLanguage(DockTextEdit::JSON);
        break;
    case XmlEditor:
        // Scintilla case: switch to the single Scintilla editor and set language
        ui->editorStack->setCurrentIndex(SciEditor);
        sciEdit->setLanguage(DockTextEdit::XML);
        break;
    }
}

void EditDialog::openPrintDialog()
{
    QPrinter printer;
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(&printer);

    QTextDocument *document = new QTextDocument();
    switch (dataSource) {
    case TextBuffer:
        document->setPlainText(ui->editorText->toPlainText());
        break;
    case SciBuffer:
        // This case isn't really expected because the Scintilla widget has it's own printing slot
        document->setPlainText(sciEdit->text());
        break;
    case HexBuffer:
        document->setPlainText(hexEdit->toReadableString());
        document->setDefaultFont(hexEdit->font());
        break;
    }

    connect(dialog, &QPrintPreviewDialog::paintRequested, [&](QPrinter *previewPrinter) {
        document->print(previewPrinter);
    });

    dialog->exec();

    delete dialog;
    delete document;
}

void EditDialog::openPrintImageDialog()
{
    QPrinter printer;
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(&printer);

    connect(dialog, &QPrintPreviewDialog::paintRequested, [&](QPrinter *previewPrinter) {
            QPainter painter(previewPrinter);
            QRect rect = painter.viewport();
            QSize size = ui->editorImage->pixmap()->size();
            size.scale(rect.size(), Qt::KeepAspectRatio);
            painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
            painter.setWindow(ui->editorImage->pixmap()->rect());
            painter.drawPixmap(0, 0, *ui->editorImage->pixmap());
        });

    dialog->exec();

    delete dialog;
}

void EditDialog::copyHexAscii()
{
    QApplication::clipboard()->setText(hexEdit->selectionToReadableString());
}
