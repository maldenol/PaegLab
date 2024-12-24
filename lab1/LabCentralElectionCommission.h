#ifndef LAB1_LABCENTRALELECTIONCOMMISSION_H
#define LAB1_LABCENTRALELECTIONCOMMISSION_H

#include "LabBallotPaper.h"
#include "LabCandidate.h"
#include "LabVoter.h"

#include <string>
#include <vector>

class LabCentralElectionCommission final
{
public:
    explicit LabCentralElectionCommission(const size_t CandidateNum, const size_t InVoterNum);

    const std::vector<LabCandidate>& GetCandidates() const;
    const std::vector<LabVoter>& GetVoters() const;

    void Vote(const unsigned VoterId, const LabBallotPaper& EncryptedBallotPaper);
    bool CountVotes();

private:
    std::vector<LabCandidate> Candidates;
    std::vector<LabVoter> Voters;

    std::vector<LabBallotPaper> BallotPapers;

    friend class LabCorruptCentralElectionCommission;
};

#endif //LAB1_LABCENTRALELECTIONCOMMISSION_H
