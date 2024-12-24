#ifndef LAB5_LABELECTIONCOMMISSION_H
#define LAB5_LABELECTIONCOMMISSION_H

#include "LabBallot.h"
#include "LabCandidate.h"
#include "LabVoter.h"

#include <vector>

class LabElectionCommission final
{
public:
    explicit LabElectionCommission(const unsigned InElectionCommissionId, const std::vector<LabVoter>& InVoters, const std::vector<LabCandidate>& InCandidates);

    void Vote(const LabBallot& SignedEncryptedBallot, const bool bIgnoreSignature = false);

    unsigned GetElectionCommissionId() const;
    std::vector<LabBallot> GetBallots(const bool bIgnoreNum = false) const;

private:
    unsigned ElectionCommissionId;

    std::vector<LabVoter> Voters;
    std::vector<LabCandidate> Candidates;

    std::vector<LabBallot> Ballots;

    friend class LabCorruptElectionCommission;
};

#endif //LAB5_LABELECTIONCOMMISSION_H
