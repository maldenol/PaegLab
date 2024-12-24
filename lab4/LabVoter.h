#ifndef LAB4_LabVoter_H
#define LAB4_LabVoter_H

#include "LabBallot.h"

#include <vector>

class LabVoter final
{
public:
    explicit LabVoter(const unsigned InVoterId);

    void SetVoters(const std::vector<LabVoter*>& InVoters);

    LabBallot CreateBallot(const unsigned CandidateId);

    bool RunElection(std::vector<LabBallot>& ShuffledBallots) const;
    bool StageOne(std::vector<LabBallot>& Ballots) const;
    bool StageTwo(std::vector<LabBallot>& Ballots, const size_t PreviousVoterIndex) const;

    void AnnounceResults() const;

    static void ShuffleBallots(std::vector<LabBallot>& Ballots);

private:
    static void AddRandomAddition(LabBallot& Ballot);
    static void RemoveAddition(LabBallot& Ballot);

    bool CheckOwnBallotPresence(const std::vector<LabBallot>& Ballots) const;

    bool SubstituteBallot(std::vector<LabBallot>& Ballots, const bool bPrevious) const;

    unsigned VoterId;
    std::vector<LabVoter*> Voters;
    size_t VoterIndex;

    LabCrypto::LabPublicKeyRSA EncryptKey;
    LabCrypto::LabPrivateKeyRSA DecryptKey;
    LabCrypto::LabPublicKeyEG VerifyKey;
    LabCrypto::LabPrivateKeyEG SignKey;

    std::vector<LabBallot> BallotHistory;
};

#endif //LAB4_LabVoter_H
