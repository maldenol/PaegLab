#include "LabCentralElectionCommission.h"

#include <cstdlib>
#include <iostream>

LabCentralElectionCommission::LabCentralElectionCommission(const size_t CandidateNum, const size_t VoterNum)
{
    for (size_t Index = 0; Index < CandidateNum; ++Index)
    {
        LabCandidate Candidate;
        Candidate.CandidateId = Index + 1;
        Candidates.push_back(Candidate);
    }

    for (size_t Index = 0; Index < VoterNum; ++Index)
    {
        LabVoter Voter;
        Voter.VoterId = Index + 1;
        LabRSA::GenerateKeys(Voter.PublicKey, Voter.PrivateKey);
        Voter.Gamma = std::rand();
        Voters.push_back(Voter);
    }
}

const std::vector<LabCandidate>& LabCentralElectionCommission::GetCandidates() const
{
    return Candidates;
}

const std::vector<LabVoter>& LabCentralElectionCommission::GetVoters() const
{
    return Voters;
}

void LabCentralElectionCommission::Vote(const unsigned VoterId, const LabBallotPaper& EncryptedBallotPaper)
{
    const LabVoter* Voter = nullptr;
    for (const LabVoter& InnerVoter : Voters)
    {
        if (InnerVoter.VoterId == VoterId)
        {
            Voter = &InnerVoter;
            break;
        }
    }

    if (!Voter)
    {
        std::cout << "CEC: Error: Invalid VoterId\n";
        return;
    }

    const LabBallotPaper BallotPaper = EncryptedBallotPaper.Decrypt(Voter->Gamma);

    if (!BallotPaper.Verify(Voter->PublicKey))
    {
        std::cout << "CEC: Error: Invalid Signature\n";
        return;
    }

    bool bIsValidCandidateId = false;
    for (const LabCandidate& Candidate : Candidates)
    {
        if (Candidate.CandidateId == BallotPaper.GetCandidateId())
        {
            bIsValidCandidateId = true;
            break;
        }
    }
    if (!bIsValidCandidateId)
    {
        std::cout << "CEC: Error: Invalid CandidateId\n";
        return;
    }

    for (const LabBallotPaper& InnerBallotPaper : BallotPapers)
    {
        if (InnerBallotPaper.GetVoterId() == Voter->VoterId)
        {
            std::cout << "CEC: Error: Voter have already voted\n";
            return;
        }
    }

    BallotPapers.push_back(BallotPaper);
    std::cout << "CEC: Vote submitted\n";
}

bool LabCentralElectionCommission::CountVotes()
{
    for (LabCandidate& Candidate : Candidates)
    {
        Candidate.Votes = 0;
    }

    if (BallotPapers.size() != Voters.size())
    {
        std::cout << "CEC: Error: Some voters did not vote\n";
        return false;
    }

    for (const LabBallotPaper& BallotPaper : BallotPapers)
    {
        for (LabCandidate& Candidate : Candidates)
        {
            if (Candidate.CandidateId == BallotPaper.GetCandidateId())
            {
                ++Candidate.Votes;
                break;
            }
        }
    }

    std::cout << "CEC: Votes counted\n";
    return true;
}
