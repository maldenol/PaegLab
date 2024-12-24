#include "LabBigInt.h"

#include <cstdlib>
#include <vector>

LabBigInt::LabBigInt()
    : Digits{"0"}, bIsNegative{false}
{
}

LabBigInt::LabBigInt(const int Value)
    : LabBigInt{std::to_string(Value)}
{
}

LabBigInt::LabBigInt(const std::string& Value)
{
    operator=(Value);
}

LabBigInt& LabBigInt::operator=(const std::string& Value)
{
    bIsNegative = (Value[0] == '-');
    Digits = bIsNegative ? Value.substr(1) : Value;
    return *this;
}

LabBigInt LabBigInt::operator+(const LabBigInt& Other) const
{
    if (bIsNegative == Other.bIsNegative)
    {
        return LabBigInt{(bIsNegative ? "-" : "") + AddStrings(Digits, Other.Digits)};
    }

    if (ComparePositiveStrings(Digits, Other.Digits) >= 0)
    {
        return LabBigInt{(bIsNegative ? "-" : "") + SubtractStrings(Digits, Other.Digits)};
    }

    return LabBigInt{(Other.bIsNegative ? "-" : "") + SubtractStrings(Other.Digits, Digits)};
}

LabBigInt LabBigInt::operator+=(const LabBigInt& Other)
{
    *this = *this + Other;
    return *this;
}

LabBigInt LabBigInt::operator-(const LabBigInt& Other) const
{
    LabBigInt NegativeOther = Other;
    NegativeOther.bIsNegative = !Other.bIsNegative;
    return *this + NegativeOther;
}

LabBigInt LabBigInt::operator-=(const LabBigInt& Other)
{
    *this = *this - Other;
    return *this;
}

LabBigInt LabBigInt::operator*(const LabBigInt& Other) const
{
    return LabBigInt{((bIsNegative != Other.bIsNegative) ? "-" : "") + MultiplyStrings(Digits, Other.Digits)};
}

LabBigInt LabBigInt::operator*=(const LabBigInt& Other)
{
    *this = *this * Other;
    return *this;
}

LabBigInt LabBigInt::operator/(const LabBigInt& Other) const
{
    const auto [Quotient, _] = DivideStrings(Digits, Other.Digits);
    return LabBigInt{((bIsNegative != Other.bIsNegative) ? "-" : "") + Quotient};
}

LabBigInt LabBigInt::operator/=(const LabBigInt& Other)
{
    *this = *this / Other;
    return *this;
}

LabBigInt LabBigInt::operator%(const LabBigInt& Other) const
{
    const auto [_, Remainder] = DivideStrings(Digits, Other.Digits);
    return LabBigInt{(bIsNegative ? "-" : "") + Remainder};
}

LabBigInt LabBigInt::operator%=(const LabBigInt& Other)
{
    *this = *this % Other;
    return *this;
}

LabBigInt LabBigInt::operator<<(const unsigned Offset) const
{
    LabBigInt BigInt = *this;
    for (unsigned Index = 0; Index < Offset; ++Index)
    {
        BigInt *= 2;
    }
    return BigInt;
}

LabBigInt LabBigInt::operator<<=(const unsigned Offset)
{
    *this = *this << Offset;
    return *this;
}

LabBigInt LabBigInt::operator>>(const unsigned Offset) const
{
    LabBigInt BigInt = *this;
    for (unsigned Index = 0; Index < Offset; ++Index)
    {
        BigInt /= 2;
    }
    return BigInt;
}

LabBigInt LabBigInt::operator>>=(const unsigned Offset)
{
    *this = *this >> Offset;
    return *this;
}

bool LabBigInt::operator==(const LabBigInt& Other) const
{
    if (Digits == "0")
    {
        return Digits == Other.Digits;
    }

    return bIsNegative == Other.bIsNegative && Digits == Other.Digits;
}

std::strong_ordering LabBigInt::operator<=>(const LabBigInt& Other) const
{
    if (operator==(Other))
    {
        return std::strong_ordering::equal;
    }

    if (bIsNegative && !Other.bIsNegative)
    {
        return std::strong_ordering::less;
    }

    if (!bIsNegative && Other.bIsNegative)
    {
        return std::strong_ordering::greater;
    }

    if (bIsNegative)
    {
        return (ComparePositiveStrings(Digits, Other.Digits) > 0) ? std::strong_ordering::less : std::strong_ordering::greater;
    }

    return (ComparePositiveStrings(Digits, Other.Digits) < 0) ? std::strong_ordering::less : std::strong_ordering::greater;
}

int LabBigInt::ToInt() const
{
    int Value = 0;

    for (const char Digit : Digits) {
        Value = Value * 10 + (Digit - '0');
    }

    return bIsNegative ? -Value : Value;
}

std::vector<std::byte> LabBigInt::ToBytes() const
{
    std::vector<std::byte> Bytes;

    LabBigInt Abs = Digits;
    while (Abs > 0)
    {
        LabBigInt Remainder = Abs % 256;
        Bytes.push_back(static_cast<std::byte>(Remainder.ToInt()));
        Abs /= 256;
    }

    if (Bytes.empty())
    {
        Bytes.push_back(static_cast<std::byte>(0));
    }

    if (bIsNegative)
    {
        Bytes.push_back(static_cast<std::byte>(0xFF));
    }

    return Bytes;
}

LabBigInt LabBigInt::FromBytes(const std::vector<std::byte>& Bytes)
{
    if (Bytes.empty())
    {
        return {};
    }

    LabBigInt Result = 0;

    LabBigInt Base = 1;
    for (const std::byte& Byte : Bytes)
    {
        int Int = static_cast<int>(Byte);
        if (Int < 0)
        {
            Int += 256;
        }

        Result += Base * Int;
        Base *= 256;
    }

    if (static_cast<int>(Bytes.back()) == 0xFF)
    {
        Result.bIsNegative = true;
    }

    return Result;
}

LabBigInt LabBigInt::RandomRange(const LabBigInt& Min, const LabBigInt& Max)
{
    const std::string& MinDigits = Min.Digits;
    const std::string& MaxDigits = Max.Digits;

    std::string Result1;
    for (size_t Index = 0; Index < MaxDigits.size(); ++Index)
    {
        const int MinDigit = (Index < MinDigits.size()) ? (MinDigits[Index] - '0') : 0;
        const int MaxDigit = (Index < MaxDigits.size()) ? (MaxDigits[Index] - '0') : 9;
        if (Result1.empty())
        {
            Result1.push_back(RandomRange(MinDigit, MaxDigit) + '0');
        }
        else
        {
            Result1.push_back(RandomRange(0, 9) + '0');
        }
    }

    LabBigInt Result2{Result1};
    while (Result2 < Min || Result2 > Max)
    {
        Result1.clear();
        for (size_t i = 0; i < MaxDigits.size(); ++i)
        {
            const int MinDigit = (i < MinDigits.size()) ? (MinDigits[i] - '0') : 0;
            const int MaxDigit = (i < MaxDigits.size()) ? (MaxDigits[i] - '0') : 9;
            Result1.push_back(RandomRange(MinDigit, MaxDigit) + '0');
        }
        Result2 = Result1;
    }

    return Result2;
}

int LabBigInt::RandomRange(const int Min, const int Max)
{
    return std::rand() % (Max - Min + 1) + Min;
}

std::string LabBigInt::AddStrings(const std::string& Left, const std::string& Right)
{
    std::string Digits;

    int Carry = 0;

    const size_t LeftLength = Left.size();
    const size_t RightLength = Right.size();
    const size_t MaxLength = std::max(LeftLength, RightLength);

    for (size_t Index = 0; Index < MaxLength; ++Index)
    {
        const int LeftDigit = (Index < LeftLength) ? (Left[LeftLength - 1 - Index] - '0') : 0;
        const int RightDigit = (Index < RightLength) ? (Right[RightLength - 1 - Index] - '0') : 0;

        int Sum = LeftDigit + RightDigit + Carry;

        Carry = Sum / 10;
        Sum = Sum % 10;

        Digits.push_back(Sum + '0');
    }

    if (Carry)
    {
        Digits.push_back(Carry + '0');
    }

    std::reverse(Digits.begin(), Digits.end());

    return Digits;
}

std::string LabBigInt::SubtractStrings(const std::string& Left, const std::string& Right)
{
    std::string Digits;

    int Borrow = 0;

    const size_t LeftLength = Left.size();
    const size_t RightLength = Right.size();

    for (size_t Index = 0; Index < LeftLength; ++Index)
    {
        const int LeftDigit = Left[LeftLength - 1 - Index] - '0';
        const int RightDigit = (Index < RightLength) ? (Right[RightLength - 1 - Index] - '0') : 0;

        int Diff = LeftDigit - RightDigit - Borrow;

        Borrow = 0;
        if (Diff < 0)
        {
            Diff += 10;
            Borrow = 1;
        }

        Digits.push_back(Diff + '0');
    }

    while (Digits.size() > 1 && Digits.back() == '0')
    {
        Digits.pop_back();
    }

    std::reverse(Digits.begin(), Digits.end());

    return Digits;
}

std::string LabBigInt::MultiplyStrings(const std::string& Left, const std::string& Right)
{
    const size_t LeftLength = Left.size();
    const size_t RightLength = Right.size();

    std::vector<int> Product(LeftLength + RightLength, 0);
    for (size_t LeftIndex = 0; LeftIndex < LeftLength; ++LeftIndex)
    {
        const int LeftDigit = Left[LeftLength - 1 - LeftIndex] - '0';
        for (size_t RightIndex = 0; RightIndex < RightLength; ++RightIndex)
        {
            const int RightDigit = Right[RightLength - 1 - RightIndex] - '0';

            Product[LeftIndex + RightIndex] += LeftDigit * RightDigit;

            if (Product[LeftIndex + RightIndex] >= 10)
            {
                Product[LeftIndex + RightIndex + 1] += Product[LeftIndex + RightIndex] / 10;
                Product[LeftIndex + RightIndex] %= 10;
            }
        }
    }

    std::string Digits;
    bool bLeadingZero = true;
    for (size_t Index = Product.size() - 1; Index != -1; --Index)
    {
        if (Product[Index] != 0 || !bLeadingZero)
        {
            bLeadingZero = false;
            Digits.push_back(Product[Index] + '0');
        }
    }

    return Digits.empty() ? "0" : Digits;
}

std::pair<std::string, std::string> LabBigInt::DivideStrings(const std::string& Left, const std::string& Right)
{
    std::string Quotient;
    std::string Remainder = "0";

    for (const char Digit : Left)
    {
        Remainder.push_back(Digit);

        while (Remainder.size() > 1 && Remainder[0] == '0')
        {
            Remainder.erase(0, 1);
        }

        int Count = 0;
        while (ComparePositiveStrings(Remainder, Right) >= 0)
        {
            Remainder = SubtractStrings(Remainder, Right);
            ++Count;
        }

        Quotient.push_back(Count + '0');
    }

    while (Quotient.size() > 1 && Quotient[0] == '0')
    {
        Quotient.erase(0, 1);
    }

    return {Quotient.empty() ? "0" : Quotient, Remainder};
}

int LabBigInt::ComparePositiveStrings(const std::string& Left, const std::string& Right)
{
    if (Left.size() != Right.size())
    {
        return Left.size() > Right.size() ? 1 : -1;
    }

    return Left.compare(Right);
}
