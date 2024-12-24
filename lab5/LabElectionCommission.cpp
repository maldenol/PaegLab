#include "LabElectionCommission.h"

#include <iostream>

LabElectionCommission::LabElectionCommission(const unsigned InElectionCommissionId, const std::vector<LabVoter>& InVoters, const std::vector<LabCandidate>& InCandidates)
    : ElectionCommissionId{InElectionCommissionId}, Voters{InVoters}, Candidates{InCandidates}
{
}

void LabElectionCommission::Vote(const LabBallot& SignedEncryptedBallot, const bool bIgnoreSignature)
{
    const unsigned VoterId = SignedEncryptedBallot.GetVoterId();
    const auto Voter = std::find_if(Voters.begin(), Voters.end(),
                                    [VoterId](const LabVoter& Voter) -> bool
                                    {
                                        return Voter.GetVoterId() == VoterId;
                                    });

    if (Voter == Voters.end())
    {
        std::cout << "EC " << ElectionCommissionId << ": Error: Invalid voter\n";
        return;
    }

    if (!bIgnoreSignature && !SignedEncryptedBallot.Verify(Voter->GetVerifyKey()))
    {
        std::cout << "EC " << ElectionCommissionId << ": Error: Invalid signature\n";
        return;
    }

    if (std::find_if(Ballots.begin(), Ballots.end(),
                     [VoterId](const LabBallot& Ballot) -> bool
                     {
                         return Ballot.GetVoterId() == VoterId;
                     }) != Ballots.end())
    {
        std::cout << "EC " << ElectionCommissionId << ": Error: Already voted\n";
        return;
    }

    Ballots.push_back(SignedEncryptedBallot);
    std::cout << "EC " << ElectionCommissionId << ": Vote submitted\n";
}

unsigned LabElectionCommission::GetElectionCommissionId() const
{
    return ElectionCommissionId;
}

std::vector<LabBallot> LabElectionCommission::GetBallots(const bool bIgnoreNum) const
{
    if (!bIgnoreNum && Ballots.size() != Voters.size())
    {
        std::cout << "EC " << ElectionCommissionId << ": Error: Number of ballots does no correspond to number of voters\n";
        return {};
    }

    return Ballots;
}
