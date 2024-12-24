#include "LabCorruptElectionCommission.h"

#include "LabElectionCommission.h"

#include <iostream>

void LabCorruptElectionCommission::VoteInsteadOfAnotherVoter(LabElectionCommission& EC)
{
    const unsigned RegistrationId = EC.ECTokens[0].RegistrationId;
    const unsigned CandidateId = EC.GetCandidates()[0].CandidateId;

    LabBallot Ballot;
    Ballot.SetRegistrationId(RegistrationId);
    Ballot.SetCandidateId(CandidateId);

    const LabCrypto::LabPublicKeyBBS& PublicKeyBBS = EC.ECTokens[0].PublicKeyBBS;
    const std::vector<LabBigInt> BallotCipher = Ballot.Encrypt(PublicKeyBBS, EC.GetPublicKeyEG());

    std::cout << "EC: Voting instead of voter with RegistrationId " << RegistrationId << " ";
    std::cout << "for candidate " << CandidateId << "\n";

    EC.Vote(BallotCipher);
}

void LabCorruptElectionCommission::SubstituteVote(LabElectionCommission& EC)
{
    const unsigned CandidateId = EC.Candidates[0].CandidateId;
    std::cout << "EC: Substituting ballots in favor of candidate " << CandidateId << "\n";

    LabBallot Ballot;
    Ballot.SetCandidateId(CandidateId);

    EC.Ballots = {EC.Ballots.size(), Ballot};
}

void LabCorruptElectionCommission::RevealVoter(LabElectionCommission& EC)
{
    std::cout << "EC: Trying to reveal voters... There is no way.\n";
}
