#ifndef LAB6_LABELECTIONCOMMISSION_H
#define LAB6_LABELECTIONCOMMISSION_H

#include "LabBallot.h"
#include "LabBigInt.h"
#include "LabCandidate.h"
#include "LabECToken.h"
#include "LabVoterToken.h"

#include <vector>

class LabElectionCommission final
{
public:
    explicit LabElectionCommission(const std::vector<unsigned>& RegistrationIds, const size_t CandidateNum);

    std::vector<LabVoterToken> GetVoterTokens() const;

    const LabCrypto::LabPublicKeyEG& GetPublicKeyEG() const;

    void Vote(const std::vector<LabBigInt>& BallotCipher);

    bool CountVotes();

    const std::vector<LabCandidate>& GetCandidates() const;

private:
    std::vector<LabECToken> ECTokens;
    std::vector<LabCandidate> Candidates;

    LabCrypto::LabPublicKeyEG PublicKeyEG;
    LabCrypto::LabPrivateKeyEG PrivateKeyEG;

    std::vector<LabBallot> Ballots;

    friend class LabCorruptElectionCommission;
};

#endif //LAB6_LABELECTIONCOMMISSION_H
