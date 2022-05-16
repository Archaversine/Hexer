#ifndef __HEXFILE_H__
#define __HEXFILE_H__

#include <bitset>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "Color.hpp"

typedef struct
{
    unsigned char value;
    const char* prefix;
    const char* invert;
} HexFileByte;

std::string _hfByte_b2str(const HexFileByte& b);
const char* _hfByte_desc(const HexFileByte& b);

class HexFile
{

friend std::ostream& operator<<(std::ostream&, const HexFile&);

private:
    std::ifstream _in;
    std::vector<HexFileByte> _bytes;

    int _selection = -1;
    
    bool _valid_pos(const int pos);
    void _scroll_if_needed();

public:
    int rows = 10;
    int rowOffset = 0;
    int maxRows = 20;
    int cols = 20;

    const std::string& Filename;

	HexFile(const std::string& filename);

    void autoSizeRows();

    void writeTo(const std::string& filename);

    void scrollUp();
    void scrollDown();

    void select(const int pos);
    void selectNext();
    void selectNextRow();
    void selectPrev();
    void selectPrevRow();
    void deselect();

    void set(const int pos, const unsigned char value);
    void setCurrent(const unsigned char value);

    void insert(const unsigned char value);
    void insert(const int pos, const unsigned char value);
    void insertc(const unsigned char* values, const int c);
    void insertc(const int pos, const unsigned char* values, const int c);

    void del();
    void del(const int pos);
    void delc(const int c);
    void delc(const int pos, const int c);

    void push_back(const unsigned char value);
    void push_front(const unsigned char value);

    void putSelDetails(std::ostream& out) const;

    const unsigned char& operator[](const int index) const;
    const size_t Bytes() const { return _bytes.size(); }

    const unsigned char* const Selection() const;
    const int SelectionAsInt() const { return _selection; }
};

std::ostream& operator<<(std::ostream& os, const HexFileByte& b);
std::ostream& operator<<(std::ostream& os, const HexFile& hf);

#endif // __HEXFILE_H__
