#include "LabCentralElectionCommission.h"

#include <cstdlib>
#include <iostream>

LabCentralElectionCommission::LabCentralElectionCommission(const size_t CandidateNum, const size_t VoterNum)
{
    for (size_t Index = 0; Index < CandidateNum; ++Index)
    {
        LabCandidate Candidate;
        Candidate.CandidateId = Index + 1;
        Candidates.push_back(Candidate);
    }

    for (size_t Index = 0; Index < VoterNum; ++Index)
    {
        LabVoter Voter;
        Voter.VoterId = Index + 1;
        Voters.push_back(Voter);
    }

    LabRSA::GenerateKeys(PublicKey, PrivateKey);
}

const std::vector<LabCandidate>& LabCentralElectionCommission::GetCandidates() const
{
    return Candidates;
}

const std::vector<LabVoter>& LabCentralElectionCommission::GetVoters() const
{
    return Voters;
}

const std::vector<LabBallot>& LabCentralElectionCommission::GetSignedBallots() const
{
    return SignedBallots;
}

const LabRSA::LabPublicKey& LabCentralElectionCommission::GetPublicKey() const
{
    return PublicKey;
}

LabSignMaskedBallotSetResult LabCentralElectionCommission::SignMaskedBallotSet(const unsigned VoterId, LabMaskedBallotSetArray& MaskedBallotSetArray, std::vector<LabBallot>& SignedMaskedBallotSet)
{
    auto Voter = std::find_if(Voters.begin(), Voters.end(),
         [VoterId](const LabVoter& Voter) -> bool
         {
             return Voter.VoterId == VoterId;
         });
    if (Voter == Voters.end())
    {
        std::cout << "CEC: Error: Invalid VoterId\n";
        return LabSignMaskedBallotSetResult::OtherError;
    }

    if (Voter->bHasSentMaskedBallotSetArray)
    {
        std::cout << "CEC: Error: Voter have sent masked ballot sets for signing already\n";
        return LabSignMaskedBallotSetResult::OtherError;
    }

    const size_t SignIndex = std::rand() % LabMaskedBallotSetArray::Size;

    const std::vector<unsigned> Rs = MaskedBallotSetArray.ReceiveRsForUnmaskingUnsignedBallotSets(SignIndex);
    for (size_t Index = 0; Index < LabMaskedBallotSetArray::Size; ++Index)
    {
        if (Index == SignIndex)
        {
            continue;
        }

        std::vector<LabBallot> MaskedBallotSet = MaskedBallotSetArray.GetMaskedBallotSets()[Index];
        for (const LabBallot& MaskedBallot : MaskedBallotSet)
        {
            const LabBallot Ballot = MaskedBallot.Unmask(PublicKey, Rs[Index]);

            const unsigned BallotId = Ballot.GetBallotId();
            if (std::find_if(SignedBallots.begin(), SignedBallots.end(),
                          [BallotId](const LabBallot& SignedBallot) -> bool
                          {
                              return SignedBallot.GetBallotId() == BallotId;
                          }) != SignedBallots.end())
            {
                std::cout << "CEC: Error: Duplicate BallotId\n";
                return LabSignMaskedBallotSetResult::DuplicateBallotId;
            }

            const unsigned CandidateId = Ballot.GetCandidateId();
            if (std::find_if(Candidates.begin(), Candidates.end(),
                          [CandidateId](const LabCandidate& Candidate) -> bool
                          {
                              return Candidate.CandidateId == CandidateId;
                          }) == Candidates.end())
            {
                std::cout << "CEC: Error: Invalid CandidateId\n";
                return LabSignMaskedBallotSetResult::OtherError;
            }
        }
    }

    Voter->bHasSentMaskedBallotSetArray = true;

    SignedMaskedBallotSet.reserve(Candidates.size());
    for (LabBallot MaskedBallot : MaskedBallotSetArray.GetMaskedBallotSets()[SignIndex])
    {
        const LabBallot SignedMaskedBallot = MaskedBallot.SignMasked(PrivateKey);
        SignedMaskedBallotSet.push_back(SignedMaskedBallot);
    }

    return LabSignMaskedBallotSetResult::Success;
}

void LabCentralElectionCommission::Vote(const LabBallot& EncryptedSignedBallot)
{
    const LabBallot SignedBallot = EncryptedSignedBallot.Decrypt(PrivateKey);

    if (!SignedBallot.Verify(PublicKey))
    {
        std::cout << "CEC: Error: Invalid signature\n";
        return;
    }

    const unsigned BallotId = SignedBallot.GetBallotId();
    if (std::find_if(SignedBallots.begin(), SignedBallots.end(),
                  [BallotId](const LabBallot& SignedBallot) -> bool
                  {
                      return SignedBallot.GetBallotId() == BallotId;
                  }) != SignedBallots.end())
    {
        std::cout << "CEC: Error: Duplicate BallotId\n";
        return;
    }

    const unsigned CandidateId = SignedBallot.GetCandidateId();
    if (std::find_if(Candidates.begin(), Candidates.end(),
                  [CandidateId](const LabCandidate& Candidate) -> bool
                  {
                      return Candidate.CandidateId == CandidateId;
                  }) == Candidates.end())
    {
        std::cout << "CEC: Error: Invalid CandidateId\n";
        return;
    }

    SignedBallots.push_back(SignedBallot);
    std::cout << "CEC: Vote submitted\n";
}

bool LabCentralElectionCommission::CountVotes()
{
    for (LabCandidate& Candidate : Candidates)
    {
        Candidate.Votes = 0;
    }

    if (SignedBallots.size() != Voters.size())
    {
        std::cout << "CEC: Error: Some voters did not vote\n";
        return false;
    }

    for (const LabBallot& SignedBallot : SignedBallots)
    {
        for (LabCandidate& Candidate : Candidates)
        {
            if (Candidate.CandidateId == SignedBallot.GetCandidateId())
            {
                ++Candidate.Votes;
                break;
            }
        }
    }

    std::cout << "CEC: Votes counted\n";
    return true;
}
