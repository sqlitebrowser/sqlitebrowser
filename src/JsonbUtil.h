#ifndef JSONBUTIL_H
#define JSONBUTIL_H

#include <QByteArray>
#include <QString>

// Checks whether the data is a SQLite JSONB array or object (JSONB needs SQLite 3.45.0 or newer) and, if so, converts it to JSON text.
// Only arrays and objects are detected because almost any other short blob is valid JSONB too (e.g. a single 0x01 byte is 'true').
bool jsonbToText(const QByteArray& data, QString& text);

#endif
