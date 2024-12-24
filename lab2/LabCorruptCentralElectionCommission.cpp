#include "LabCorruptCentralElectionCommission.h"

#include "LabCentralElectionCommission.h"
#include "LabMaskedBallotSetArray.h"

#include <iostream>

void LabCorruptCentralElectionCommission::RevealVoterBallotIdDuringUnmasking(LabCentralElectionCommission& CEC, const unsigned VoterId, LabMaskedBallotSetArray MaskedBallotSetArray)
{
    const unsigned SignIndex = 0;
    const unsigned UnmaskIndex = 1;

    std::vector<unsigned> Rs = MaskedBallotSetArray.ReceiveRsForUnmaskingUnsignedBallotSets(SignIndex);
    const unsigned UnmaskR = Rs[UnmaskIndex];
    const LabBallot& UnmaskedBallot = MaskedBallotSetArray.GetMaskedBallotSets()[SignIndex][0].Unmask(CEC.PublicKey, UnmaskR);
    std::cout << "CEC: Trying to reveal voter " << VoterId << "'s BallotId:\n";

    std::cout << "\tReceiving new R array, its size is " << Rs.size() << "\n";
    const unsigned SignR = Rs[SignIndex];
    std::cout << "\tR for masked ballot that needs to be signed: " << SignR << "\n";

    const LabBallot& BallotToSign = MaskedBallotSetArray.GetMaskedBallotSets()[SignIndex][0].Unmask(CEC.PublicKey, SignR);
    std::cout << "\tUnmasked BallotId: " << BallotToSign.GetBallotId() << "\n";

    std::cout << "CEC: Trying to get last R from voter:\n";
    Rs = MaskedBallotSetArray.ReceiveRsForUnmaskingUnsignedBallotSets(UnmaskIndex);
    std::cout << "CEC: Receiving new R array, its size is " << Rs.size() << "\n";
}

void LabCorruptCentralElectionCommission::ChangeVote(LabCentralElectionCommission& CEC, const unsigned BallotId)
{
    for (LabBallot& SignedBallot : CEC.SignedBallots)
    {
        if (SignedBallot.GetBallotId() != BallotId)
        {
            continue;
        }

        const unsigned OldCandidateId = SignedBallot.GetCandidateId();
        const unsigned NewCandidateId = CEC.Candidates.size() + 1 - OldCandidateId;

        std::cout << "CEC: Changing ballot " << BallotId << "'s vote for candidate " << OldCandidateId << " ";
        std::cout << "to " << NewCandidateId << "\n";
        SignedBallot.SetCandidateId(NewCandidateId);

        std::cout << "\tSigning the ballot\n";
        SignedBallot = SignedBallot.Sign(CEC.PrivateKey);

        std::cout << "\tVote changed\n";
        break;
    }
}

void LabCorruptCentralElectionCommission::VoteInsteadOfVoter(LabCentralElectionCommission& CEC, const unsigned VoterId, const unsigned CandidateId)
{
    auto Voter = std::find_if(CEC.Voters.begin(), CEC.Voters.end(),
                              [VoterId](const LabVoter& Voter) -> bool
                              {
                                  return Voter.VoterId == VoterId;
                              });
    Voter->bHasSentMaskedBallotSetArray = true;

    std::cout << "CEC: Voting instead of voter " << VoterId << " for candidate " << CandidateId << "\n";
    const unsigned BallotId = std::rand() % LabBallot::MaxBallotId;
    LabBallot Ballot{BallotId};
    Ballot.SetCandidateId(CandidateId);

    std::cout << "\tSigning the ballot\n";
    const LabBallot SignedBallot = Ballot.Sign(CEC.PrivateKey);

    std::cout << "\tVoting (" << BallotId << ")\n";
    const LabBallot EncryptedSignedBallot = SignedBallot.Encrypt(CEC.PublicKey);
    CEC.Vote(EncryptedSignedBallot);
}
