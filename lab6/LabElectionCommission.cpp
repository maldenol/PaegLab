#include "LabElectionCommission.h"

#include <algorithm>
#include <iostream>

LabElectionCommission::LabElectionCommission(const std::vector<unsigned>& RegistrationIds, const size_t CandidateNum)
{
    ECTokens.reserve(RegistrationIds.size());
    for (const unsigned RegistrationId : RegistrationIds)
    {
        LabECToken ECToken;
        ECToken.RegistrationId = RegistrationId;
        LabCrypto::GenerateKeysBBS(ECToken.PublicKeyBBS, ECToken.PrivateKeyBBS);
        ECTokens.push_back(ECToken);
    }

    Candidates.reserve(CandidateNum);
    for (size_t Index = 0; Index < CandidateNum; ++Index)
    {
        unsigned CandidateId;
        do
        {
            CandidateId = LabBigInt::RandomRange(1000, 2000);
        }
        while (std::find_if(Candidates.begin(), Candidates.end(),
                            [CandidateId](const LabCandidate& Candidate) -> bool
                            {
                                return Candidate.CandidateId == CandidateId;
                            }) != Candidates.end());

        LabCandidate Candidate;
        Candidate.CandidateId = CandidateId;
        Candidates.push_back(Candidate);
    }

    LabCrypto::GenerateKeysEG(PublicKeyEG, PrivateKeyEG);
}

std::vector<LabVoterToken> LabElectionCommission::GetVoterTokens() const
{
    std::vector<LabVoterToken> VoterTokens{ECTokens.size()};
    std::transform(ECTokens.begin(), ECTokens.end(), VoterTokens.begin(),
                   [](const LabECToken& ECToken) -> LabVoterToken
                   {
                       LabVoterToken VoterToken;
                       VoterToken.RegistrationId = ECToken.RegistrationId;
                       VoterToken.PublicKeyBBS = ECToken.PublicKeyBBS;
                       return VoterToken;
                   });
    return VoterTokens;
}

const LabCrypto::LabPublicKeyEG& LabElectionCommission::GetPublicKeyEG() const
{
    return PublicKeyEG;
}

void LabElectionCommission::Vote(const std::vector<LabBigInt>& BallotCipher)
{
    const std::vector<LabECToken>& ThisECTokens = ECTokens;
    const auto GetPrivateBBSKey = [&ThisECTokens](const unsigned RegistrationId) -> LabCrypto::LabPrivateKeyBBS
    {
        const auto ECToken = std::find_if(ThisECTokens.begin(), ThisECTokens.end(),
                     [RegistrationId](const LabECToken& ECToken) -> bool
                     {
                         return ECToken.RegistrationId == RegistrationId;
                     });

        if (ECToken == ThisECTokens.end())
        {
            std::cout << "EC: Error: Invalid RegistrationId\n";
            return {};
        }

        return ECToken->PrivateKeyBBS;
    };

    const LabBallot Ballot = LabBallot::Decrypt(BallotCipher, GetPrivateBBSKey, PrivateKeyEG);

    const unsigned RegistrationId = Ballot.GetRegistrationId();
    if (std::find_if(Ballots.begin(), Ballots.end(),
                     [RegistrationId](const LabBallot& SubmittedBallot) -> bool
                     {
                         return SubmittedBallot.GetRegistrationId() == RegistrationId;
                     }) != Ballots.end())
    {
        std::cout << "EC: Error: Already voted\n";
        return;
    }

    const unsigned CandidateId = Ballot.GetCandidateId();
    if (std::find_if(Candidates.begin(), Candidates.end(),
                  [CandidateId](const LabCandidate& Candidate) -> bool
                  {
                      return Candidate.CandidateId == CandidateId;
                  }) == Candidates.end())
    {
        std::cout << "EC: Error: Invalid CandidateId\n";
        return;
    }

    Ballots.push_back(Ballot);
    std::cout << "EC: Vote submitted\n";
}

bool LabElectionCommission::CountVotes()
{
    for (LabCandidate& Candidate : Candidates)
    {
        Candidate.Votes = 0;
    }

    if (Ballots.size() != ECTokens.size())
    {
        std::cout << "EC: Error: Number of ballots does not correspond to number of voters\n";
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
