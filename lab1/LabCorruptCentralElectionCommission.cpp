#include "LabCorruptCentralElectionCommission.h"

#include "LabCentralElectionCommission.h"

#include <iostream>

void LabCorruptCentralElectionCommission::RevealVoter(LabCentralElectionCommission& CentralElectionCommission, const unsigned VoterId)
{
    for (const LabBallotPaper& BallotPaper : CentralElectionCommission.BallotPapers)
    {
        if (BallotPaper.GetVoterId() == VoterId)
        {
            std::cout << "CEC: Voter " << VoterId << " voted for candidate " << BallotPaper.GetCandidateId() << "\n";

            break;
        }
    }
}

void LabCorruptCentralElectionCommission::VoteInsteadOfVoter(LabCentralElectionCommission& CentralElectionCommission, const unsigned VoterId, const unsigned CandidateId)
{
    for (const LabVoter& Voter : CentralElectionCommission.Voters)
    {
        if (Voter.VoterId == VoterId)
        {
            LabBallotPaper BallotPaper{VoterId};
            BallotPaper.SetCandidateId(CandidateId);
            BallotPaper.Sign(Voter.PrivateKey);
            const LabBallotPaper EncryptedBallotPaper = BallotPaper.Encrypt(Voter.Gamma);
            std::cout << "CEC: Voting instead of voter " << VoterId << ": for candidate " << CandidateId << "\n";
            CentralElectionCommission.Vote(VoterId, EncryptedBallotPaper);

            break;
        }
    }
}

void LabCorruptCentralElectionCommission::ChangeVote(LabCentralElectionCommission& CentralElectionCommission, const unsigned VoterId, const unsigned CandidateId)
{
    for (LabBallotPaper& BallotPaper : CentralElectionCommission.BallotPapers)
    {
        if (BallotPaper.GetVoterId() == VoterId)
        {
            BallotPaper.SetCandidateId(CandidateId);
            std::cout << "CEC: Changing voter " << VoterId << "'s vote to candidate " << CandidateId << "\n";

            break;
        }
    }
}
