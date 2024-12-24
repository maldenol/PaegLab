#ifndef LAB2_LABMASKEDBALLOTSETARRAY_H
#define LAB2_LABMASKEDBALLOTSETARRAY_H

#include "LabBallot.h"

#include <array>
#include <vector>

class LabMaskedBallotSetArray final
{
public:
    static constexpr size_t Size = 10;

    void SetBallotSet(const size_t Index, std::vector<LabBallot> MaskedBallotSet, const unsigned R);

    const std::array<std::vector<LabBallot>, Size>& GetMaskedBallotSets() const;
    std::vector<unsigned> ReceiveRsForUnmaskingUnsignedBallotSets(const size_t InSignIndex);
    unsigned GetRForUnmaskingSignedBallotSet() const;

private:
    std::array<std::vector<LabBallot>, Size> MaskedBallotSets;
    std::array<unsigned, Size> Rs;
    unsigned SignIndex = -1;
};

#endif //LAB2_LABMASKEDBALLOTSETARRAY_H
