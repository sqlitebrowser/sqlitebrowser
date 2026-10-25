#include "JsonbUtil.h"

#include <sqlite3.h>

bool jsonbToText(const QByteArray& data, QString& text)
{
    if(data.isEmpty() || sqlite3_libversion_number() < 3045000)
        return false;

    // The lower nibble of the first byte is the element type: 0xB is an array, 0xC is an object.
    const auto byteAt = [&data](int i) { return static_cast<quint64>(static_cast<unsigned char>(data.at(i))); };
    const unsigned char header = static_cast<unsigned char>(data.at(0));
    const unsigned char type = header & 0x0F;
    if(type != 0x0B && type != 0x0C)
        return false;

    // The upper nibble is either the payload size or, for values above 11, the number of bytes that hold the size. Requiring that the
    // size matches the length of the data rejects most text and binary data which happens to start with a suitable byte.
    quint64 payload_size = header >> 4;
    int header_size = 1;
    if(payload_size > 11)
    {
        const int size_bytes = 1 << (payload_size - 12);    // 1, 2, 4 or 8 bytes in big endian order
        if(data.size() < 1 + size_bytes)
            return false;
        payload_size = 0;
        for(int i=0;i<size_bytes;++i)
            payload_size = (payload_size << 8) | byteAt(1 + i);
        header_size += size_bytes;
    }
    if(payload_size != static_cast<quint64>(data.size() - header_size))
        return false;

    // json() validates the whole blob and fails for anything that is not well-formed JSONB
    sqlite3* db = nullptr;
    if(sqlite3_open(":memory:", &db) != SQLITE_OK)
    {
        sqlite3_close(db);
        return false;
    }

    bool success = false;
    sqlite3_stmt* stmt = nullptr;
    if(sqlite3_prepare_v2(db, "SELECT json(?1)", -1, &stmt, nullptr) == SQLITE_OK)
    {
        if(sqlite3_bind_blob(stmt, 1, data.constData(), data.size(), SQLITE_STATIC) == SQLITE_OK &&
           sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_type(stmt, 0) == SQLITE_TEXT)
        {
            text = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)), sqlite3_column_bytes(stmt, 0));
            success = true;
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return success;
}
