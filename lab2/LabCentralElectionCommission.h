#ifndef LAB2_LABCENTRALELECTIONCOMMISSION_H
#define LAB2_LABCENTRALELECTIONCOMMISSION_H

#include "LabBallot.h"
#include "LabMaskedBallotSetArray.h"
#include "LabCandidate.h"
#include "LabVoter.h"

#include <string>
#include <vector>

enum class LabSignMaskedBallotSetResult
{
    Success,
    DuplicateBallotId,
    OtherError,
};

class LabCentralElectionCommission final
{
public:
    explicit LabCentralElectionCommission(const size_t CandidateNum, const size_t InVoterNum);

    const std::vector<LabCandidate>& GetCandidates() const;
    const std::vector<LabVoter>& GetVoters() const;
    const std::vector<LabBallot>& GetSignedBallots() const;

    const LabRSA::LabPublicKey& GetPublicKey() const;

    LabSignMaskedBallotSetResult SignMaskedBallotSet(const unsigned VoterId, LabMaskedBallotSetArray& MaskedBallotSetArray, std::vector<LabBallot>& SignedMaskedBallotSet);
    void Vote(const LabBallot& EncryptedSignedBallot);

    bool CountVotes();

private:
    std::vector<LabCandidate> Candidates;
    std::vector<LabVoter> Voters;

    std::vector<LabBallot> SignedBallots;

    LabRSA::LabPublicKey PublicKey;
    LabRSA::LabPrivateKey PrivateKey;

    friend class LabCorruptCentralElectionCommission;
};

#endif //LAB2_LABCENTRALELECTIONCOMMISSION_H
