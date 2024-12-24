#include "LabCorruptRegistrationBureau.h"

#include "LabElectionCommission.h"
#include "LabRegistrationBureau.h"

#include <iostream>

void LabCorruptRegistrationBureau::VoteInsteadOfAnotherVoter(LabRegistrationBureau& RB, LabElectionCommission& EC)
{
    const unsigned RegistrationId = RB.RegistrationIds[0];
    const unsigned CandidateId = EC.GetCandidates()[0].CandidateId;

    LabBallot Ballot;
    Ballot.SetRegistrationId(RegistrationId);
    Ballot.SetCandidateId(CandidateId);

    const LabCrypto::LabPublicKeyBBS& PublicKeyBBS = RB.DuplicateTokens[RegistrationId].PublicKeyBBS;
    const std::vector<LabBigInt> BallotCipher = Ballot.Encrypt(PublicKeyBBS, EC.GetPublicKeyEG());

    std::cout << "RB: Voting instead of voter with RegistrationId " << RegistrationId << " ";
    std::cout << "for candidate " << CandidateId << "\n";

    EC.Vote(BallotCipher);
}

void LabCorruptRegistrationBureau::RevealVoter(LabRegistrationBureau& RB)
{
    std::cout << "RB: Revealing voters:\n";
    for (const std::pair<unsigned, unsigned> VoterRegistrationId : RB.VoterRegistrationIds)
    {
        std::cout << "\tVoterId, RegistrationId: " << VoterRegistrationId.second << " " << VoterRegistrationId.first << "\n";
    }
}
