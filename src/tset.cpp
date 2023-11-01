#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : MaxPower(mp), BitField(mp) {}

// конструктор копирования
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField) {}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf) {}

TSet::operator TBitField()
{
    TBitField tbf(BitField);
    return tbf;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // проверить наличие элемента в множестве
{
    if (Elem<0 || Elem>MaxPower) throw - 1;
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem<0 || Elem>MaxPower) throw - 1;
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem<0 || Elem>MaxPower) throw - 1;
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    if (MaxPower != s.MaxPower) return 0;
    else for (int i = 0; i < BitField.GetLength(); i++) {
        if (BitField.GetBit(i) != s.BitField.GetBit(i)) return 0;
    }
    return 1;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    if (MaxPower == s.MaxPower) return 0;
    else for (int i = 0; i < BitField.GetLength(); i++) {
        if (BitField.GetBit(i) == s.BitField.GetBit(i)) return 0;
    }
    return 1;
}
TSet TSet::operator+(const TSet& s) // объединение
{
    TSet ts(BitField | s.BitField);
    return ts;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet ts(MaxPower);
    if ((Elem < MaxPower) && (Elem > -1))
        ts.BitField.SetBit(Elem);
    return ts;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet ts(MaxPower);
    if ((Elem < MaxPower) && (Elem > -1))
        ts.BitField.ClrBit(Elem);
    return ts;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    TSet ts(BitField & s.BitField);
    return ts;
}

TSet TSet::operator~(void) // дополнение
{
    TSet ts(MaxPower);
    ts.BitField = ~BitField;
    return ts;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    int num;
    while (istr >> num) {
        if (num >= 0 && num < s.MaxPower) s.BitField.SetBit(num);
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    for (int i = 0; i < s.GetMaxPower(); i++) {
        if (s.IsMember(i))
            ostr << i << " ";
    }
    return ostr;
}