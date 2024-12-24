#ifndef LAB3_LABELECTIONCOMMISSION_H
#define LAB3_LABELECTIONCOMMISSION_H

#include "LabBallot.h"
#include "LabCandidate.h"
#include "LabCrypto.h"
#include "LabPublicVoter.h"

#include <vector>

class LabElectionCommission final
{
public:
    explicit LabElectionCommission(const std::vector<LabPublicVoter>& InVoters, const size_t CandidateNum);

    const LabCrypto::LabPublicKeyEG& GetPublicKey() const;

    void Vote(const LabBallotCipher& SignedBallotCipher);

    bool CountVotes();

    const std::vector<LabCandidate>& GetCandidates() const;
    const std::vector<LabBallot>& GetBallots() const;

private:
    std::vector<LabPublicVoter> Voters;
    std::vector<LabCandidate> Candidates;

    LabCrypto::LabPublicKeyEG PublicKey;
    LabCrypto::LabPrivateKeyEG PrivateKey;

    std::vector<LabBallot> Ballots;

    friend class LabCorruptElectionCommission;
};

#endif //LAB3_LABELECTIONCOMMISSION_H
