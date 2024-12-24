#include "LabRegistrationBureau.h"

#include "LabCrypto.h"
#include "LabElectionCommission.h"
#include "LabLocalVoter.h"

#include <algorithm>
#include <iostream>

LabRegistrationBureau::LabRegistrationBureau(const size_t VoterNum)
{
    for (size_t Index = 0; Index < VoterNum; ++Index)
    {
        LabRBVoter Voter;
        Voter.VoterId = Index + 1;
        LabCrypto::GenerateKeysDSA(Voter.PublicKey, Voter.PrivateKey);
        Voters.push_back(Voter);
    }
}

void LabRegistrationBureau::Register(LabLocalVoter& LocalVoter)
{
    const unsigned VoterId = LocalVoter.VoterId;

    auto Voter = std::find_if(Voters.begin(), Voters.end(), [VoterId](const LabRBVoter& Voter) -> bool
                                        {
                                            return Voter.VoterId == VoterId;
                                        });

    if (Voter == Voters.end())
    {
        std::cout << "RB: Error: Unknown voter\n";
        return;
    }

    if (Voter->RegistrationId != 0)
    {
        std::cout << "RB: Error: Voter attempted to register second time\n";
        return;
    }

    unsigned RegistrationId;
    do
    {
        RegistrationId = LabCrypto::RandomRange(1, 100);
    }
    while (std::find_if(Voters.begin(), Voters.end(),
                        [RegistrationId](const LabRBVoter& Voter) -> bool
                        {
                            return Voter.RegistrationId == RegistrationId;
                        }) != Voters.end());
    Voter->RegistrationId = RegistrationId;

    LocalVoter.RegistrationId = Voter->RegistrationId;
    LocalVoter.PrivateKey = Voter->PrivateKey;

    std::cout << "RB: Registered voter " << VoterId << " under RegistrationId " << RegistrationId << "\n";
}

LabElectionCommission LabRegistrationBureau::CreateElectionCommission(const size_t CandidateNum) const
{
    if (std::find_if(Voters.begin(), Voters.end(),
                     [](const LabRBVoter& Voter) -> bool
                     {
                         return Voter.RegistrationId == 0;
                     }) != Voters.end())
    {
        std::cout << "RB: Error: Some voters have not registered\n";
    }

    return LabElectionCommission{GetPublicVoters(), CandidateNum};
}

void LabRegistrationBureau::GetResults(LabElectionCommission& EC,
                                       std::vector<LabCandidate>& Candidates,
                                       std::vector<LabBallot>& Ballots,
                                       std::vector<LabPublicVoter>& PublicVoters) const
{
    if (!EC.CountVotes())
    {
        return;
    }

    Candidates = EC.GetCandidates();
    Ballots = EC.GetBallots();
    PublicVoters = GetPublicVoters();
}

std::vector<LabPublicVoter> LabRegistrationBureau::GetPublicVoters() const
{
    std::vector<LabPublicVoter> PublicVoters{Voters.size()};
    std::transform(Voters.begin(), Voters.end(), PublicVoters.begin(),
                   [](const LabRBVoter& Voter) -> LabPublicVoter
                        {
                            LabPublicVoter PublicVoter;
                            PublicVoter.RegistrationId = Voter.RegistrationId;
                            PublicVoter.PublicKey = Voter.PublicKey;
                            return PublicVoter;
                        });
    return PublicVoters;
}
