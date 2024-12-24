#include "LabCorruptCentralElectionCommission.h"

#include "LabCentralElectionCommission.h"

#include <iostream>

void LabCorruptCentralElectionCommission::VoteInsteadOfAnotherVoter(LabCentralElectionCommission& CEC, std::vector<LabElectionCommission>& ECs)
{
    const LabVoter& VictimVoter = CEC.Voters[0];
    const unsigned VoterId = VictimVoter.GetVoterId();
    const unsigned CandidateId = CEC.Candidates[0].CandidateId;

    std::cout << "CEC: Voting instead of voter " << VoterId << " for candidate " << CandidateId << "\n";

    const std::vector<LabBallot> Ballots = LabVoter::CreateBallotParts(VoterId, CandidateId, ECs.size(), CEC.EncryptKey, VictimVoter.SignKey);

    for (size_t Index = 0; Index < Ballots.size(); ++Index)
    {
        ECs[Index].Vote(Ballots[Index], true);
    }
}

void LabCorruptCentralElectionCommission::DropVote(LabCentralElectionCommission& CEC)
{
    const auto Ballot = CEC.Ballots.begin();
    std::cout << "CEC: Dropping vote from voter " << Ballot->GetVoterId() << "\n";
    CEC.Ballots.erase(Ballot);
}

void LabCorruptCentralElectionCommission::DuplicateVote(LabCentralElectionCommission& CEC)
{
    const auto Ballot = CEC.Ballots.begin();
    std::cout << "CEC: Duplicating vote from voter " << Ballot->GetVoterId() << "\n";
    CEC.Ballots.push_back(*Ballot);
}

void LabCorruptCentralElectionCommission::SubstituteVote(LabCentralElectionCommission& CEC)
{
    const LabVoter& VictimVoter = CEC.Voters[0];
    const unsigned VoterId = VictimVoter.GetVoterId();
    const unsigned CandidateId = CEC.Candidates[0].CandidateId;

    std::cout << "CEC: Substituting vote from " << VoterId << " ";
    std::cout << " to candidate " << CandidateId << "\n";

    const LabBallot WholeBallot = LabVoter::CreateWholeBallot(VoterId, CandidateId);
    const LabBallot EncryptedWholeBallot = WholeBallot.Encrypt(CEC.EncryptKey);

    CEC.Ballots.erase(CEC.Ballots.begin());
    CEC.Ballots.insert(CEC.Ballots.begin(), EncryptedWholeBallot);
}
