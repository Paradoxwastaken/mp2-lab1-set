#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) { throw - 1; }
    BitLen = len;
    MemLen = BitLen / (sizeof(TELEM) * 8);
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;


}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[]pMem;
    BitLen = 0;
    MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n<0 || n>BitLen) { throw - 1; }
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ((n < 0) || (n > BitLen)) { throw - 1; }
    return 1 << n % (sizeof(TELEM) * 8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n > BitLen) { throw - 1; }
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen) { throw - 1; }
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > BitLen) { throw - 1; }
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf)
        delete[]pMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];

    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (bf.BitLen != BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i]) { return 0; }
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int Max = max(BitLen, bf.BitLen);
    TBitField tbf(Max);
    if (MemLen > bf.MemLen)
        for (int i = 0; i < MemLen; i++)
        {

            tbf.pMem[i] = pMem[i];
            tbf.pMem[i] |= bf.pMem[i];
        }
    else
        for (int i = 0; i < bf.MemLen; i++)
        {
            tbf.pMem[i] = bf.pMem[i];
            tbf.pMem[i] |= pMem[i];
        }
    return tbf;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int Max = max(BitLen, bf.BitLen);
    TBitField tbf(Max);
    if (MemLen > bf.MemLen)
        for (int i = 0; i < MemLen; i++)
        {

            tbf.pMem[i] = pMem[i];
            tbf.pMem[i] &= bf.pMem[i];
        }
    else 
        for (int i = 0; i < bf.MemLen; i++)
        {
            tbf.pMem[i] = bf.pMem[i];
            tbf.pMem[i] &= pMem[i];
        }
    return tbf;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tbf(BitLen);
    for (int i = 0; i < tbf.GetLength(); i++)
        if (GetBit(i) == 0) tbf.SetBit(i);
        //if (GetBit(i)!=0) tbf.ClrBit(i);
    return tbf;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    string str;
    istr >> str;
    if (str.size() != bf.GetLength()) { throw - 1; }
    for (int i = 0; i < bf.GetLength(); i++)
    {
        if (str[i] == '0') { bf.ClrBit(i); }
        if (str[i] == '1') { bf.SetBit(i); }
        else throw - 1;
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{

    for (int i = 0; i < bf.GetLength(); i++) {
        if (bf.GetBit(i))
            ostr << '1';
        else
            ostr << '0';
    }
    return ostr;
}