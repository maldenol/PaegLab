#include "LabRegistrationBureau.h"

#include <iostream>

LabRegistrationBureau::LabRegistrationBureau(const size_t VoterNum)
{
    RegistrationIds.reserve(VoterNum);
    for (size_t Index = 0; Index < VoterNum; ++Index)
    {
        unsigned RegistrationId;
        do
        {
            RegistrationId = LabBigInt::RandomRange(100, 200);
        }
        while (std::find(RegistrationIds.begin(), RegistrationIds.end(), RegistrationId) != RegistrationIds.end());

        RegistrationIds.push_back(RegistrationId);
    }
}

void LabRegistrationBureau::Register(const unsigned VoterId)
{
    if (std::find(VotersWithoutToken.begin(), VotersWithoutToken.end(), VoterId) != VotersWithoutToken.end())
    {
        std::cout << "RB: Error: Voter " << VoterId << " is already registered\n";
        return;
    }

    std::cout << "RB: Voter " << VoterId << " has registered\n";
    VotersWithoutToken.push_back(VoterId);
}

const std::vector<unsigned>& LabRegistrationBureau::GetRegistrationIds() const
{
    return RegistrationIds;
}

void LabRegistrationBureau::SetVoterTokens(const std::vector<LabVoterToken>& InVoterTokens)
{
    VoterTokens = InVoterTokens;
}

LabVoterToken LabRegistrationBureau::GetToken(const unsigned VoterId)
{
    const auto VoterIter = std::find(VotersWithoutToken.begin(), VotersWithoutToken.end(), VoterId);
    if (VoterIter == VotersWithoutToken.end())
    {
        std::cout << "RB: Error: Voter " << VoterId << " is not registered or already received the token\n";
        return {};
    }

    VotersWithoutToken.erase(VoterIter);

    const LabVoterToken VoterToken = VoterTokens.back();
    VoterTokens.pop_back();

    VoterRegistrationIds.emplace(VoterToken.RegistrationId, VoterId);
    DuplicateTokens.emplace(VoterToken.RegistrationId, VoterToken);

    std::cout << "RB: Voter " << VoterId << " has received his token\n";
    return VoterToken;
}
