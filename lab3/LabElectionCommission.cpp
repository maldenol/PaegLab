#include "LabElectionCommission.h"

#include <iostream>

LabElectionCommission::LabElectionCommission(const std::vector<LabPublicVoter>& InVoters, const size_t CandidateNum)
    : Voters{InVoters}
{
    for (size_t Index = 0; Index < CandidateNum; ++Index)
    {
        LabCandidate Candidate;
        Candidate.CandidateId = Index + 1;
        Candidates.push_back(Candidate);
    }

    LabCrypto::GenerateKeysEG(PublicKey, PrivateKey);
}

const LabCrypto::LabPublicKeyEG& LabElectionCommission::GetPublicKey() const
{
    return PublicKey;
}

void LabElectionCommission::Vote(const LabBallotCipher& SignedBallotCipher)
{
    const LabBallot SignedBallot = SignedBallotCipher.Decrypt(PrivateKey);

    const unsigned RegistrationId = SignedBallot.GetRegistrationId();
    const auto Voter = std::find_if(Voters.begin(), Voters.end(),
                                                    [RegistrationId](const LabPublicVoter& Voter) -> bool
                                                    {
                                                        return Voter.RegistrationId == RegistrationId;
                                                    });

    if (Voter == Voters.end() || Voter->RegistrationId == 0)
    {
        std::cout << "EC: Error: Unregistered voter\n";
        return;
    }

    if (Voter->bHaveVoted)
    {
        std::cout << "EC: Error: Already voted\n";
        return;
    }

    if (!SignedBallot.Verify(Voter->PublicKey))
    {
        std::cout << "EC: Error: Invalid signature\n";
        return;
    }

    const unsigned CandidateId = SignedBallot.GetCandidateId();
    if (std::find_if(Candidates.begin(), Candidates.end(),
                  [CandidateId](const LabCandidate& Candidate) -> bool
                  {
                      return Candidate.CandidateId == CandidateId;
                  }) == Candidates.end())
    {
        std::cout << "EC: Error: Invalid candidate\n";
        return;
    }

    Ballots.push_back(SignedBallot);
    Voter->bHaveVoted = true;
    std::cout << "EC: Vote submitted\n";
}

bool LabElectionCommission::CountVotes()
{
    for (LabCandidate& Candidate : Candidates)
    {
        Candidate.Votes = 0;
    }

    if (Ballots.size() != Voters.size())
    {
        std::cout << "EC: Error: Some voters did not vote\n";
        return false;
    }

    for (const LabBallot& SignedBallot : Ballots)
    {
        for (LabCandidate& Candidate : Candidates)
        {
            if (Candidate.CandidateId == SignedBallot.GetCandidateId())
            {
                ++Candidate.Votes;
                break;
            }
        }
    }

    std::cout << "EC: Votes counted\n";
    return true;
}

const std::vector<LabCandidate>& LabElectionCommission::GetCandidates() const
{
    return Candidates;
}

const std::vector<LabBallot>& LabElectionCommission::GetBallots() const
{
    return Ballots;
}
