#include "xbytearray.h"

XByteArray::XByteArray()
{
    _oldSize = -99;
    _addressNumbers = 4;
    _addressOffset = 0;

}

int XByteArray::addressOffset()
{
    return _addressOffset;
}

void XByteArray::setAddressOffset(int offset)
{
    _addressOffset = offset;
}

int XByteArray::addressWidth()
{
    return _addressNumbers;
}

void XByteArray::setAddressWidth(int width)
{
    if ((width >= 0) && (width<=6))
    {
        _addressNumbers = width;
    }
}

QByteArray & XByteArray::data()
{
    return _data;
}

void XByteArray::setData(QByteArray data)
{
    _data = data;
    _changedData = QByteArray(data.length(), char(0));
}

bool XByteArray::dataChanged(int i)
{
    return bool(_changedData[i]);
}

QByteArray XByteArray::dataChanged(int i, int len)
{
    return _changedData.mid(i, len);
}

void XByteArray::setDataChanged(int i, bool state)
{
    _changedData[i] = char(state);
}

void XByteArray::setDataChanged(int i, const QByteArray & state)
{
    int length = state.length();
    int len;
    if ((i + length) > _changedData.length())
        len = _changedData.length() - i;
    else
        len = length;
    _changedData.replace(i, len, state);
}

int XByteArray::realAddressNumbers()
{
    if (_oldSize != _data.size())
    {
        // is addressNumbers wide enought?
        QString test = QString("%1")
                      .arg(_data.size() + _addressOffset, _addressNumbers, 16, QChar('0'));
        _realAddressNumbers = test.size();
    }
    return _realAddressNumbers;
}

int XByteArray::size()
{
    return _data.size();
}

QByteArray & XByteArray::insert(int i, char ch)
{
    _data.insert(i, ch);
    _changedData.insert(i, char(1));
    return _data;
}

QByteArray & XByteArray::insert(int i, const QByteArray & ba)
{
    _data.insert(i, ba);
    _changedData.insert(i, QByteArray(ba.length(), char(1)));
    return _data;
}

QByteArray & XByteArray::remove(int i, int len)
{
    _data.remove(i, len);
    _changedData.remove(i, len);
    return _data;
}

QByteArray & XByteArray::replace(int index, char ch)
{
    _data[index] = ch;
    _changedData[index] = char(1);
    return _data;
}

QByteArray & XByteArray::replace(int index, const QByteArray & ba)
{
    int len = ba.length();
    return replace(index, len, ba);
}

QByteArray & XByteArray::replace(int index, int length, const QByteArray & ba)
{
    int len;
    if ((index + length) > _data.length())
        len = _data.length() - index;
    else
        len = length;
    _data.replace(index, len, ba.mid(0, len));
    _changedData.replace(index, len, QByteArray(len, char(1)));
    return _data;
}

QChar XByteArray::asciiChar(int index)
{
    char ch = _data[index];
    if ((ch < 0x20) || (ch > 0x7e))
            ch = '.';
    return QChar(ch);
}

QString XByteArray::toRedableString(int start, int end)
{
    int adrWidth = realAddressNumbers();
    if (_addressNumbers > adrWidth)
        adrWidth = _addressNumbers;
    if (end < 0)
        end = _data.size();

    QString result;
    for (int i=start; i < end; i += 16)
    {
        QString adrStr = QString("%1").arg(_addressOffset + i, adrWidth, 16, QChar('0'));
        QString hexStr;
        QString ascStr;
        for (int j=0; j<16; j++)
        {
            if ((i + j) < _data.size())
            {
                hexStr.append(" ").append(_data.mid(i+j, 1).toHex());
                ascStr.append(asciiChar(i+j));
            }
        }
        result += adrStr + " " + QString("%1").arg(hexStr, -48) + "  " + QString("%1").arg(ascStr, -17) + "\n";
    }
    return result;
}
