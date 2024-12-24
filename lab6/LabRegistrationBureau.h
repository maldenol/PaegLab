#ifndef LAB6_LABREGISTRATIONBUREAU_H
#define LAB6_LABREGISTRATIONBUREAU_H

#include "LabVoterToken.h"

#include <map>
#include <vector>

class LabRegistrationBureau final
{
public:
    explicit LabRegistrationBureau(const size_t VoterNum);

    void Register(const unsigned VoterId);

    const std::vector<unsigned>& GetRegistrationIds() const;
    void SetVoterTokens(const std::vector<LabVoterToken>& InVoterTokens);

    LabVoterToken GetToken(const unsigned VoterId);

private:
    std::vector<unsigned> RegistrationIds;
    std::vector<LabVoterToken> VoterTokens;
    std::vector<unsigned> VotersWithoutToken;

    std::map<unsigned, unsigned> VoterRegistrationIds;
    std::map<unsigned, LabVoterToken> DuplicateTokens;

    friend class LabCorruptRegistrationBureau;
};

#endif //LAB6_LABREGISTRATIONBUREAU_H
