#include "LabCorruptRegistrationBureau.h"

#include "LabElectionCommission.h"
#include "LabRBVoter.h"
#include "LabRegistrationBureau.h"

#include <iostream>

void LabCorruptRegistrationBureau::VoteInsteadOfVoter(const LabRegistrationBureau& RB, LabElectionCommission& EC)
{
    const LabRBVoter& Voter = RB.Voters[0];
    LabBallot Ballot{Voter.RegistrationId};
    const unsigned CandidateId = 2;
    Ballot.SetCandidateId(CandidateId);
    const LabBallot SignedBallot = Ballot.Sign(Voter.PrivateKey);
    const LabBallotCipher SignedBallotCipher = SignedBallot.Encrypt(EC.GetPublicKey());

    std::cout << "RB: Voting instead of voter " << Voter.VoterId << " for candidate " << CandidateId << "\n";
    EC.Vote(SignedBallotCipher);
}

void LabCorruptRegistrationBureau::RevealVoter(const LabRegistrationBureau& RB, const LabBallot& Ballot)
{
    const unsigned RegistrationId = Ballot.GetRegistrationId();
    const unsigned CandidateId = Ballot.GetCandidateId();

    const auto Voter = std::find_if(RB.Voters.begin(), RB.Voters.end(),
                                    [RegistrationId](const LabRBVoter& Voter) -> bool
                                    {
                                        return Voter.RegistrationId == RegistrationId;
                                    });
    const unsigned VoterId = Voter->VoterId;

    std::cout << "RB: Revealing voter:\n";
    std::cout << "\tRegistrationId: " << RegistrationId << "\n";
    std::cout << "\tCandidateId: " << CandidateId << "\n";
    std::cout << "\tVoterId: " << VoterId << "\n";
}

const std::vector<LabRBVoter>& LabCorruptRegistrationBureau::GetRBVoters(const LabRegistrationBureau& RB)
{
    return RB.Voters;
}
