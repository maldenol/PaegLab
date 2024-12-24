#include "LabVoter.h"

LabVoter::LabVoter(const unsigned InVoterId)
    : VoterId{InVoterId}
{
}

unsigned LabVoter::GetVoterId() const
{
    return VoterId;
}

void LabVoter::SetVoterToken(const LabVoterToken& InVoterToken)
{
    VoterToken = InVoterToken;
}

const LabVoterToken& LabVoter::GetVoterToken() const
{
    return VoterToken;
}
