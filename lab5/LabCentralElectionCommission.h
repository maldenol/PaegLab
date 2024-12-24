#ifndef LAB5_LABCENTRALELECTIONCOMMISSION_H
#define LAB5_LABCENTRALELECTIONCOMMISSION_H

#include "LabCandidate.h"
#include "LabElectionCommission.h"
#include "LabVoter.h"

#include <vector>

class LabCentralElectionCommission final
{
public:
    explicit LabCentralElectionCommission(const size_t VoterNum, const size_t CandidateNum);

    const std::vector<LabVoter>& GetVoters() const;
    const std::vector<LabCandidate>& GetCandidates() const;
    const LabCrypto::LabPublicKeyRSA& GetEncryptKey() const;

    bool ReceiveBallots(const std::vector<LabElectionCommission>& ElectionCommissions, const bool bIgnoreCorruption = false);
    bool CountVotes(std::vector<LabBallot>& OutBallots);

private:
    std::vector<LabVoter> Voters;
    std::vector<LabCandidate> Candidates;

    std::vector<LabBallot> Ballots;

    LabCrypto::LabPublicKeyRSA EncryptKey;
    LabCrypto::LabPrivateKeyRSA DecryptKey;

    friend class LabCorruptCentralElectionCommission;
};

#endif //LAB5_LABCENTRALELECTIONCOMMISSION_H
