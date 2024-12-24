#include "LabCorruptElectionCommission.h"

#include "LabCorruptRegistrationBureau.h"
#include "LabElectionCommission.h"
#include "LabRegistrationBureau.h"

#include <iostream>

void LabCorruptElectionCommission::VoteInsteadOfVoter(LabElectionCommission& EC)
{
    const LabPublicVoter& Voter = EC.Voters[0];
    LabBallot Ballot{Voter.RegistrationId};
    const unsigned CandidateId = 2;
    Ballot.SetCandidateId(CandidateId);
    const LabBallot SignedBallot = Ballot.Sign(LabCrypto::LabPrivateKeyDSA{});
    const LabBallotCipher SignedBallotCipher = SignedBallot.Encrypt(EC.GetPublicKey());

    std::cout << "CEC: Voting instead of some voter for candidate " << CandidateId << "\n";
    EC.Vote(SignedBallotCipher);
}

void LabCorruptElectionCommission::ChangeVote(LabElectionCommission& EC)
{
    LabBallot& Ballot = EC.Ballots[0];
    const unsigned OldCandidateId = Ballot.GetCandidateId();
    const unsigned NewCandidateId = EC.Candidates.size() + 1 - OldCandidateId;

    std::cout << "CEC: Changing vote of ballot " << Ballot.GetRegistrationId() << " ";
    std::cout << "from candidate " << OldCandidateId << " to " << NewCandidateId << "\n";
    Ballot.SetCandidateId(NewCandidateId);
    Ballot = Ballot.Sign(LabCrypto::LabPrivateKeyDSA{});
}

void LabCorruptElectionCommission::RevealVoter(LabElectionCommission& EC)
{
    std::cout << "CEC: Trying to reveal voter:\n";
    std::cout << "\tRegistrationId: " << EC.Ballots[0].GetRegistrationId() << "\n";
    std::cout << "\tCandidateId: " << EC.Ballots[0].GetCandidateId() << "\n";
    std::cout << "\tVoterId: Unknown\n";
}

void LabCorruptElectionCommission::ChangeVoteWithRB(LabElectionCommission& EC, LabRegistrationBureau& RB)
{
    LabBallot& Ballot = EC.Ballots[0];
    const unsigned OldCandidateId = Ballot.GetCandidateId();
    const unsigned NewCandidateId = EC.Candidates.size() + 1 - OldCandidateId;

    std::cout << "CEC & RB: Changing vote of ballot " << Ballot.GetRegistrationId() << " ";
    std::cout << "from candidate " << OldCandidateId << " to " << NewCandidateId << "\n";
    Ballot.SetCandidateId(NewCandidateId);

    const unsigned RegistrationId = Ballot.GetRegistrationId();
    const std::vector<LabRBVoter> RBVoters = LabCorruptRegistrationBureau::GetRBVoters(RB);
    const auto Voter = std::find_if(RBVoters.begin(), RBVoters.end(),
                                    [RegistrationId](const LabRBVoter& Voter) -> bool
                                    {
                                        return Voter.RegistrationId == RegistrationId;
                                    });
    Ballot = Ballot.Sign(Voter->PrivateKey);
}
