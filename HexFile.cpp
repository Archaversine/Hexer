#include "HexFile.hpp"

constexpr const char* HFB_DESCRIPTIONS[] =
{
    "NULL",
    "START OF HEADING",
    "START OF TEXT",
    "END OF TEXT",
    "END OF TRANSMISSION",
    "ENQUIRY",
    "ACKNOWLEDGE",
    "BELL",
    "BACKSPACE",
    "HORIZONTAL TAB",
    "LINE FEED",
    "VERTICAL TAB",
    "FORM FEED",
    "CARRIAGE RETURN",
    "SHIFT OUT",
    "SHIFT IN",
    "DATA LINK ESCAPE",
    "DEVICE CONTROL 1",
    "DEVICE CONTROL 2",
    "DEVICE CONTROL 3",
    "DEVICE CONTROL 4",
    "NEGATIVE ACKNOWLEDGE",
    "SYNCHRONOUS IDLE",
    "ENG OF TRANS. BLOCK",
    "CANCEL",
    "END OF MEDIUM",
    "SUBSTITUTE",
    "ESCAPE",
    "FILE SEPARATOR",
    "GROUP SEPARATOR",
    "RECORD SEPARATOR",
    "UNIT SEPARATOR",
    "SPACE"
};

std::string _hfByte_b2str(const HexFileByte& b)
{
    std::string output = b.invert;
    output += b.prefix;

    if (b.value < 0x20 || b.value > 0x7e)
    {
        output += YELLOW;
        output += '.';
    }
    else
    {
        output += (char) b.value;
    }

    return output + RESET;
}

const char* _hfByte_desc(const HexFileByte& b)
{
    if (b.value == 0x7f) { return "DEL"; }
    if (b.value > 0x20) { return ""; }

    return HFB_DESCRIPTIONS[b.value];
}

HexFile::HexFile(const std::string& filename) : Filename(filename)
{
    _in.open(filename);

    if (!_in.is_open())
    {
        std::cerr << "File '" << filename << "' could not be loaded.\n";
        exit(EXIT_FAILURE);
    }

    unsigned char temp;

    _in >> std::noskipws;

    while (_in >> temp)
    {
        _bytes.push_back({temp, "", ""});
    }       

    _in.close();
    autoSizeRows();
}

bool HexFile::_valid_pos(const int pos)
{
    return (pos > -1 && pos < _bytes.size());
}

void HexFile::_scroll_if_needed()
{
    if (!_valid_pos(_selection)) { return; }

    while (_selection / cols < rowOffset)
    {
        scrollUp();
    }

    while (_selection / cols >= rowOffset + maxRows)
    {
        scrollDown();
    }
}

void HexFile::autoSizeRows()
{
    rows = _bytes.size() / cols + (_bytes.size() % cols != 0);
    rows = std::min(rows, maxRows);
}

void HexFile::writeTo(const std::string& filename)
{
    std::ofstream out(filename);

    for (int i = 0; i < _bytes.size(); ++i)
    {
        out << _bytes[i].value;
        _bytes[i].prefix = "";
    }

    out.close();
}

void HexFile::scrollUp()
{
    if (rowOffset > 0) { --rowOffset; }
}

void HexFile::scrollDown()
{
    if (rowOffset < _bytes.size() - 5) { ++rowOffset; }
}

void HexFile::select(const int pos)
{
    if (!_valid_pos(pos)) { return; }
    if (_selection != -1) { _bytes[_selection].invert = ""; }

    _selection = pos;
    _bytes[_selection].invert = INVERT;
    _scroll_if_needed();
}

void HexFile::selectNext()
{
    if (_selection == _bytes.size() - 1) { return; }
    if (_selection != -1) { _bytes[_selection].invert = ""; }

    _bytes[++_selection].invert = INVERT;
    _scroll_if_needed();
}

void HexFile::selectNextRow()
{
    if (_selection >= _bytes.size() - cols || _selection == -1) { return; }
    if (_selection != -1) { _bytes[_selection].invert = ""; }

    _selection += cols;
    _bytes[_selection].invert = INVERT;
    _scroll_if_needed();
}

void HexFile::selectPrev()
{
    if (_selection <= 0) { return; }
    if (_selection != -1) { _bytes[_selection].invert = ""; }

    _bytes[--_selection].invert = INVERT;
    _scroll_if_needed();
}

void HexFile::selectPrevRow()
{
    if (_selection < cols || _selection == -1) { return; }
    if (_selection != -1) { _bytes[_selection].invert = ""; }

    _selection -= cols;
    _bytes[_selection].invert = INVERT;
    _scroll_if_needed();
}

void HexFile::deselect()
{
    if (_selection != -1) { _bytes[_selection].invert = ""; }
    _selection = -1;
}

void HexFile::set(const int pos, const unsigned char value)
{
    _bytes[pos].value = value;
    _bytes[pos].prefix = BLUE;
}

void HexFile::setCurrent(const unsigned char value)
{
    if (_selection == -1 || _selection >= _bytes.size()) { return; }

    _bytes[_selection].value = value;
    _bytes[_selection].prefix = BLUE;
}

void HexFile::insert(const unsigned char value)
{
    if (_selection == -1 || _selection >= _bytes.size()) { return; }
    _bytes.insert(_bytes.begin() + _selection, {value, GREEN, ""});
    selectNext();
}

void HexFile::insert(const int pos, const unsigned char value)
{
    _bytes.insert(_bytes.begin() + pos, {value, GREEN, ""});
    if (_selection >= pos) { selectNext(); }
}

void HexFile::insertc(const unsigned char* values, const int c)
{
    for (int i = 0; i < c; ++i)
    {
        if (_selection == -1 || _selection + i >= _bytes.size()) { return; }
        _bytes.insert(_bytes.begin() + _selection + i, {values[i], GREEN, ""});
        selectNext();
    }
}

void HexFile::insertc(const int pos, const unsigned char* values, const int c)
{
    for (int i = 0; i < c; ++i)
    {
        _bytes.insert(_bytes.begin() + pos + i, {values[i], GREEN, ""});
        if (_selection >= pos) { selectNext(); }
    }
}

void HexFile::del()
{
    if (_selection == -1 || _selection >= _bytes.size()) { return; }
    _bytes.erase(_bytes.begin() + _selection, _bytes.begin() + _selection + 1);

    if (_selection == -1 || _selection >= _bytes.size())
    {
        _selection = _bytes.size() - 1;
        _bytes[_selection].invert = INVERT;
        return;
    }

    _bytes[_selection].invert = INVERT;
}

void HexFile::del(const int pos)
{
    _bytes.erase(_bytes.begin() + pos, _bytes.begin() + pos + 1);
}

void HexFile::delc(const int c)
{
    for (int i = 0; i < c; ++i)
    {
        if (_selection == -1 || _selection + i >= _bytes.size()) { return; }
        _bytes.erase(_bytes.begin() + _selection, _bytes.begin() + _selection + c);

        if (_selection == -1 || _selection + i >= _bytes.size())
        {
            _selection = _bytes.size() - 1;
            _bytes[_selection].invert = INVERT;
            return;
        }

        _bytes[_selection].invert = INVERT;
    }
}

void HexFile::delc(const int pos, const int c)
{
    _bytes.erase(_bytes.begin() + pos, _bytes.begin() + pos + c);
}

void HexFile::push_back(const unsigned char value)
{
    _bytes.push_back({value, GREEN, ""});
}

void HexFile::push_front(const unsigned char value)
{
    _bytes.insert(_bytes.begin(), {value, GREEN, ""});
    selectNext();
}

void HexFile::putSelDetails(std::ostream& out) const
{
    if (_selection == -1 || _selection >= _bytes.size()) { return; }

    out << "Selection (0x";
    out << std::setw(8) << std::setfill('0') << std::hex;
    
    if (_selection == 0) { out << 0; }
    else
    {
        out << _selection;
    }

    int value = (int) *Selection();

    out << "): [HEX: ";
    out << std::right << std::setw(2) << std::setfill('0') << std::hex;
    out << value << "], [DEC: " << std::dec << value << "], [OCT: ";
    out << std::oct << value << "], [BIN: ";
    out << std::bitset<8>(value) << "] ";
    out << std::left << std::setw(20) << std::setfill(' ');
    out << _hfByte_desc(_bytes[_selection]) << '\n';

    out << std::dec;
}

const unsigned char* const HexFile::Selection() const
{
    if (_selection == -1 || _selection >= _bytes.size()) { return nullptr; }
    return &_bytes[_selection].value;
}

const unsigned char& HexFile::operator[](const int index) const
{
    return _bytes[index].value;
}

std::ostream& operator<<(std::ostream& os, const HexFileByte& b)
{
    os << b.invert << b.prefix;
    os << std::right << std::setw(2) << std::setfill('0');
    os << std::hex << (int) b.value << RESET;
    return os;
}

std::ostream& operator<<(std::ostream& os, const HexFile& hf)
{
    int c = hf.rowOffset * hf.cols;

    auto horzLine = [&]()
    {
        os << '|' << std::string(4 * hf.cols + 17, '-') << "|\n";
    };

    horzLine();

    for (int i = hf.rowOffset; i < hf.rowOffset + hf.maxRows; ++i)
    {
        if (i >= hf._bytes.size()) { break; }

        int temp = c;

        os << "| 0x";
        os << std::setw(8) << std::setfill('0') << std::hex;
        os << (i * hf.cols) << " | ";

        for (int j = 0; j < hf.cols; ++j)
        {
            if (c == hf._bytes.size())
            {
                os << DIM << "-- " << RESET;
                continue;
            }

            if (c % 2 == 1) { os << DIM; } // Alternating colors

            os << hf._bytes[c++] << " ";
        }

        c = temp;
        os << "| ";

        for (int j = 0; j < hf.cols; ++j)
        {
            if (c == hf._bytes.size())
            {
                os << " ";
                continue;
            }

            os << _hfByte_b2str(hf._bytes[c++]);
        }

        os << " |\n";
    }

    horzLine();

    if (hf._selection != -1 && hf._selection < hf._bytes.size())
    {
        os << " ";
        hf.putSelDetails(os);
    }

    os << std::dec;
    return os;
}
