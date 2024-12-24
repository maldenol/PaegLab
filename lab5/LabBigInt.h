#ifndef LAB5_LABBIGINT_H
#define LAB5_LABBIGINT_H

#include <string>

class LabBigInt final
{
public:
    LabBigInt();
    LabBigInt(const LabBigInt& Other) = default;
    LabBigInt(const int Value);
    LabBigInt(const std::string& Value);

    LabBigInt& operator=(const LabBigInt& Other) = default;
    LabBigInt& operator=(const std::string& Value);

    LabBigInt operator+(const LabBigInt& Other) const;
    LabBigInt operator+=(const LabBigInt& Other);

    LabBigInt operator-(const LabBigInt& Other) const;
    LabBigInt operator-=(const LabBigInt& Other);

    LabBigInt operator*(const LabBigInt& Other) const;
    LabBigInt operator*=(const LabBigInt& Other);

    LabBigInt operator/(const LabBigInt& Other) const;
    LabBigInt operator/=(const LabBigInt& Other);

    LabBigInt operator%(const LabBigInt& Other) const;
    LabBigInt operator%=(const LabBigInt& Other);

    LabBigInt operator<<(const unsigned Offset) const;
    LabBigInt operator<<=(const unsigned Offset);

    LabBigInt operator>>(const unsigned Offset) const;
    LabBigInt operator>>=(const unsigned Offset);

    bool operator==(const LabBigInt& Other) const;
    std::strong_ordering operator<=>(const LabBigInt& Other) const;

    int ToInt() const;

    static LabBigInt RandomRange(const LabBigInt& Min, const LabBigInt& Max);
    static int RandomRange(const int Min, const int Max);

private:
    static std::string AddStrings(const std::string& Left, const std::string& Right);
    static std::string SubtractStrings(const std::string& Left, const std::string& Right);
    static std::string MultiplyStrings(const std::string& Left, const std::string& Right);
    static std::pair<std::string, std::string> DivideStrings(const std::string& Left, const std::string& Right);
    static int ComparePositiveStrings(const std::string& Left, const std::string& Right);

    std::string Digits;
    bool bIsNegative;
};

#endif //LAB5_LABBIGINT_H
