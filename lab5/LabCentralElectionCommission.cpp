#include "LabCentralElectionCommission.h"

#include "LabBigInt.h"
#include "LabCrypto.h"
#include "LabElectionCommission.h"

#include <cstdlib>
#include <iostream>

LabCentralElectionCommission::LabCentralElectionCommission(const size_t VoterNum, const size_t CandidateNum)
{
    Voters.reserve(VoterNum);
    for (size_t Index = 0; Index < VoterNum; ++Index)
    {
        unsigned VoterId;
        do
        {
            VoterId = std::rand();
        }
        while (std::find_if(Voters.begin(), Voters.end(),
                            [VoterId](const LabVoter& Voter) -> bool
                            {
                                return Voter.GetVoterId() == VoterId;
                            }) != Voters.end());

        Voters.emplace_back(VoterId);
    }

    Candidates.reserve(CandidateNum);
    for (size_t Index = 0; Index < CandidateNum; ++Index)
    {
        unsigned CandidateId;
        do
        {
            do
            {
                CandidateId = LabBigInt::RandomRange(100, 500);
            }
            while (CandidateId % 8 != 0 && CandidateId % 12 != 0 && CandidateId % 18 != 0 && CandidateId % 27 != 0);
        }
        while (std::find_if(Candidates.begin(), Candidates.end(),
                            [CandidateId](const LabCandidate& Candidate) -> bool
                            {
                                return Candidate.CandidateId == CandidateId;
                            }) != Candidates.end());

        Candidates.emplace_back(CandidateId);
    }

    LabCrypto::GenerateKeysRSA(EncryptKey, DecryptKey);
}

const std::vector<LabVoter>& LabCentralElectionCommission::GetVoters() const
{
    return Voters;
}

const std::vector<LabCandidate>& LabCentralElectionCommission::GetCandidates() const
{
    return Candidates;
}

const LabCrypto::LabPublicKeyRSA& LabCentralElectionCommission::GetEncryptKey() const
{
    return EncryptKey;
}

bool LabCentralElectionCommission::ReceiveBallots(const std::vector<LabElectionCommission>& ElectionCommissions, const bool bIgnoreCorruption)
{
    Ballots.clear();
    Ballots.reserve(Voters.size());

    for (size_t Index = 0; Index < Voters.size(); ++Index)
    {
        const unsigned VoterId = Voters[Index].GetVoterId();
        LabBallot Ballot{VoterId};
        Ballot.SetCandidateId(1);
        Ballots.push_back(Ballot);
    }

    for (const LabElectionCommission& ElectionCommission : ElectionCommissions)
    {
        const std::vector<LabBallot>& ECBallots = ElectionCommission.GetBallots(true);

        if (ECBallots.empty())
        {
            return false;
        }

        if (!bIgnoreCorruption && ECBallots.size() != Voters.size())
        {
            std::cout << "CEC: Error: Number of ballots does no correspond to number of voters\n";
            return false;
        }

        for (const LabBallot& SignedEncryptedBallot : ECBallots)
        {
            const unsigned VoterId = SignedEncryptedBallot.GetVoterId();
            const auto Voter = std::find_if(Voters.begin(), Voters.end(),
                                            [VoterId](const LabVoter& Voter) -> bool
                                            {
                                                return Voter.GetVoterId() == VoterId;
                                            });

            if (Voter == Voters.end())
            {
                std::cout << "CEC: Error: Invalid voter\n";
                return false;
            }

            if (!bIgnoreCorruption && !SignedEncryptedBallot.Verify(Voter->GetVerifyKey()))
            {
                std::cout << "CEC: Error: Invalid signature\n";
                return false;
            }

            if (!bIgnoreCorruption && std::find_if(ECBallots.begin(), ECBallots.end(),
                             [VoterId, &SignedEncryptedBallot](const LabBallot& Ballot) -> bool
                             {
                                 return Ballot.GetVoterId() == VoterId && Ballot != SignedEncryptedBallot;
                             }) != ECBallots.end())
            {
                std::cout << "CEC: Error: Duplicate vote found\n";
                return false;
            }

            const auto Ballot = std::find_if(Ballots.begin(), Ballots.end(),
                                             [VoterId](const LabBallot& Ballot) -> bool
                                             {
                                                 return Ballot.GetVoterId() == VoterId;
                                             });

            LabBigInt CandidateId = Ballot->GetCandidateId();
            CandidateId *= SignedEncryptedBallot.GetCandidateId();
            Ballot->SetCandidateId(CandidateId);
        }
    }

    return true;
}

bool LabCentralElectionCommission::CountVotes(std::vector<LabBallot>& OutBallots)
{
    for (LabCandidate& Candidate : Candidates)
    {
        Candidate.Votes = 0;
    }

    for (LabBallot& Ballot : Ballots)
    {
        Ballot = Ballot.Decrypt(DecryptKey);

        const LabBigInt& CandidateId = Ballot.GetCandidateId();
        const auto Candidate = std::find_if(Candidates.begin(), Candidates.end(),
                                            [&CandidateId](const LabCandidate& Candidate) -> bool
                                            {
                                                return Candidate.CandidateId == CandidateId;
                                            });

        if (Candidate == Candidates.end())
        {
            std::cout << "CEC: Error: Invalid candidate\n";
            return false;
        }

        ++Candidate->Votes;
    }

    OutBallots = Ballots;
    return true;
}
