#ifndef LAB6_LabVoter_H
#define LAB6_LabVoter_H

#include "LabVoterToken.h"

class LabVoter final
{
public:
    explicit LabVoter(const unsigned InVoterId);

    unsigned GetVoterId() const;
    void SetVoterToken(const LabVoterToken& InVoterToken);
    const LabVoterToken& GetVoterToken() const;

private:
    unsigned VoterId;
    LabVoterToken VoterToken;
};

#endif //LAB6_LabVoter_H
