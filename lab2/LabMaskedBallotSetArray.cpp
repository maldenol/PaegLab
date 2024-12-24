#include "LabMaskedBallotSetArray.h"

#include <iostream>

void LabMaskedBallotSetArray::SetBallotSet(const size_t Index, std::vector<LabBallot> MaskedBallotSet, const unsigned R)
{
    MaskedBallotSets[Index] = MaskedBallotSet;
    Rs[Index] = R;

    SignIndex = -1;
}

const std::array<std::vector<LabBallot>, LabMaskedBallotSetArray::Size>& LabMaskedBallotSetArray::GetMaskedBallotSets() const
{
    return MaskedBallotSets;
}

std::vector<unsigned> LabMaskedBallotSetArray::ReceiveRsForUnmaskingUnsignedBallotSets(const size_t InSignIndex)
{
    if (SignIndex != -1)
    {
        std::cout << "Voter: CEC attempted to unmask last masked ballot set\n";
        return {};
    }

    SignIndex = InSignIndex;

    std::vector<unsigned> Result;
    Result.reserve(Size);
    for (size_t Index = 0; Index < Size; ++Index)
    {
        if (Index == SignIndex)
        {
            Result.push_back(0);
        }
        else
        {
            Result.push_back(Rs[Index]);
        }
    }
    return Result;
}

unsigned LabMaskedBallotSetArray::GetRForUnmaskingSignedBallotSet() const
{
    return Rs[SignIndex];
}
