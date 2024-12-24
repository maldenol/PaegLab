#include "LabVoter.h"

#include "LabTestCase.h"

#include <algorithm>
#include <iostream>
#include <random>

LabVoter::LabVoter(const unsigned InVoterId)
    : VoterId{InVoterId}
{
    LabCrypto::GenerateKeysRSA(EncryptKey, DecryptKey);
    LabCrypto::GenerateKeysEG(VerifyKey, SignKey);
}

void LabVoter::SetVoters(const std::vector<LabVoter*>& InVoters)
{
    Voters = InVoters;

    const unsigned ThisVoterId = VoterId;
    VoterIndex = std::find_if(Voters.begin(), Voters.end(),
                              [ThisVoterId](const LabVoter* Voter) -> bool
                              {
                                  return Voter->VoterId == ThisVoterId;
                              }) - Voters.begin();
}

LabBallot LabVoter::CreateBallot(const unsigned CandidateId)
{
    BallotHistory.clear();

    LabBallot Ballot;
    Ballot.SetCandidateId(CandidateId);
    BallotHistory.push_back(Ballot);

    AddRandomAddition(Ballot);
    BallotHistory.push_back(Ballot);

    for (auto Voter = Voters.rbegin(); Voter != Voters.rend(); ++Voter)
    {
        Ballot = Ballot.Encrypt((*Voter)->EncryptKey);
        BallotHistory.push_back(Ballot);
    }

    for (auto Voter = Voters.rbegin(); Voter != Voters.rend(); ++Voter)
    {
        AddRandomAddition(Ballot);
        Ballot = Ballot.Encrypt((*Voter)->EncryptKey);
        BallotHistory.push_back(Ballot);
    }

    std::cout << "Voter " << VoterId << ": Voting for candidate " << CandidateId << "\n";

    return Ballot;
}

bool LabVoter::RunElection(std::vector<LabBallot>& ShuffledBallots) const
{
    if (VoterIndex != 0)
    {
        std::cout << "Voter " << VoterId << ": Error: The first voter in the list should initiate the election\n";
        return false;
    }

    std::cout << "Voter " << VoterId << ": Initiating the election\n";
    std::cout << "Voter " << VoterId << ": Begin stage 1\n";
    return StageOne(ShuffledBallots);
}

bool LabVoter::StageOne(std::vector<LabBallot>& Ballots) const
{
    if (Ballots.size() != Voters.size())
    {
        std::cout << "Voter " << VoterId << ": Error: Wrong number of ballots on stage 1\n";
        return false;
    }

    if (!CheckOwnBallotPresence(Ballots))
    {
        std::cout << "Voter " << VoterId << ": Error: Own ballot is not present on stage 1\n";
        return false;
    }

    for (LabBallot& Ballot : Ballots)
    {
        Ballot = Ballot.Decrypt(DecryptKey);

        RemoveAddition(Ballot);
    }

    if (GTestCase == LabTestCase::StageOneDropVote)
    {
        GTestCase = LabTestCase::Normal;
        Ballots.erase(Ballots.end() - 1);
        std::cout << "Voter " << VoterId << ": Drop some vote\n";
    }
    else if (GTestCase == LabTestCase::StageOneDuplicateVote)
    {
        GTestCase = LabTestCase::Normal;
        Ballots.push_back(Ballots.back());
        std::cout << "Voter " << VoterId << ": Duplicate some vote\n";
    }
    else if (GTestCase == LabTestCase::StageOneSubstituteVotePrevious)
    {
        if (SubstituteBallot(Ballots, true))
        {
            GTestCase = LabTestCase::Normal;
            std::cout << "Voter " << VoterId << ": Substitute some previous vote\n";
        }
    }
    else if (GTestCase == LabTestCase::StageOneSubstituteVoteNext)
    {
        if (SubstituteBallot(Ballots, false))
        {
            GTestCase = LabTestCase::Normal;
            std::cout << "Voter " << VoterId << ": Substitute some next vote\n";
        }
    }

    ShuffleBallots(Ballots);

    std::cout << "Voter " << VoterId << ": Succeeded on stage 1\n";

    const bool bIsLast = (VoterIndex == Voters.size() - 1);
    if (bIsLast)
    {
        std::cout << "Voter " << VoterId << ": Begin stage 2\n";
        return Voters[0]->StageTwo(Ballots, -1);
    }

    return Voters[VoterIndex + 1]->StageOne(Ballots);
}

bool LabVoter::StageTwo(std::vector<LabBallot>& Ballots, const size_t PreviousVoterIndex) const
{
    if (Ballots.size() != Voters.size())
    {
        std::cout << "Voter " << VoterId << ": Error: Wrong number of ballots on stage 2\n";
        return false;
    }

    if (!CheckOwnBallotPresence(Ballots))
    {
        std::cout << "Voter " << VoterId << ": Error: Own ballot is not present on stage 2\n";
        return false;
    }

    const bool bIsNotFirst = (PreviousVoterIndex != -1);
    if (bIsNotFirst)
    {
        for (const LabBallot& Ballot : Ballots)
        {
            if (!Ballot.Verify(Voters[PreviousVoterIndex]->VerifyKey))
            {
                std::cout << "Voter " << VoterId << ": Error: ";
                std::cout << "Voter " << Voters[PreviousVoterIndex]->VoterId << "'s signature is invalid\n";
                return false;
            }
        }

        std::cout << "Voter " << VoterId << ": ";
        std::cout << "Voter " << Voters[PreviousVoterIndex]->VoterId << "'s signatures are valid\n";
    }

    const bool bCalledFromPreviousVoterInList = (PreviousVoterIndex == VoterIndex - 1);
    const bool bCalledFromLastVoterInList = (PreviousVoterIndex == Voters.size() - 1);
    if (bCalledFromPreviousVoterInList)
    {
        const bool bIsLast = (VoterIndex == Voters.size() - 1);

        for (LabBallot& Ballot : Ballots)
        {
            Ballot = Ballot.Decrypt(DecryptKey);

            if (bIsLast)
            {
                RemoveAddition(Ballot);
            }

            Ballot = Ballot.Sign(SignKey);
        }

        if (GTestCase == LabTestCase::StageTwoDropVote)
        {
            GTestCase = LabTestCase::Normal;
            Ballots.erase(Ballots.end() - 1);
            std::cout << "Voter " << VoterId << ": Drop some vote\n";
        }
        else if (GTestCase == LabTestCase::StageTwoSubstituteVotePrevious)
        {
            if (SubstituteBallot(Ballots, true))
            {
                GTestCase = LabTestCase::Normal;
                std::cout << "Voter " << VoterId << ": Substitute some previous vote\n";
            }
        }
        else if (GTestCase == LabTestCase::StageTwoSubstituteVoteNext)
        {
            if (SubstituteBallot(Ballots, false))
            {
                GTestCase = LabTestCase::Normal;
                std::cout << "Voter " << VoterId << ": Substitute some next vote\n";
            }
        }

        ShuffleBallots(Ballots);

        std::cout << "Voter " << VoterId << ": Succeeded on stage 2\n";

        if (bIsLast)
        {
            AnnounceResults();
        }

        const size_t NextVoterIndex = VoterIndex + 1;

        for (size_t Index = 0; Index < Voters.size(); ++Index)
        {
            if (Index == VoterIndex || Index == NextVoterIndex)
            {
                continue;
            }

            const bool bSuccess = Voters[Index]->StageTwo(Ballots, VoterIndex);
            if (!bSuccess)
            {
                return false;
            }
        }

        if (!bIsLast)
        {
            const bool bSuccess = Voters[NextVoterIndex]->StageTwo(Ballots, VoterIndex);
            if (!bSuccess)
            {
                return false;
            }
        }
    }
    else if (bCalledFromLastVoterInList)
    {
        AnnounceResults();
    }

    return true;
}

void LabVoter::AnnounceResults() const
{
    std::cout << "Voter " << VoterId << ": Election succeeded, viewing the results";

    const bool bIsLast = (VoterIndex == Voters.size() - 1);
    if (bIsLast)
    {
        std::cout << " (before others)";
    }

    std::cout << "\n";
}

void LabVoter::ShuffleBallots(std::vector<LabBallot>& Ballots)
{
    std::random_device RD;
    std::mt19937 Gen{RD()};
    std::ranges::shuffle(Ballots, Gen);
}

void LabVoter::AddRandomAddition(LabBallot& Ballot)
{
    std::vector<LabBigInt>& CandidateId = Ballot.GetCandidateIdMut();
    CandidateId.reserve(CandidateId.size() + 1);
    const char Addition = std::rand() % ('9' - '0' + 1) + '0';
    CandidateId.push_back(Addition);
}

void LabVoter::RemoveAddition(LabBallot& Ballot)
{
    std::vector<LabBigInt>& CandidateId = Ballot.GetCandidateIdMut();
    CandidateId.erase(CandidateId.end() - 1);
}

bool LabVoter::CheckOwnBallotPresence(const std::vector<LabBallot>& Ballots) const
{
    return std::find_if(BallotHistory.begin(), BallotHistory.end(),
                     [Ballots](const LabBallot& OwnBallot) -> bool
                     {
                         return std::find_if(Ballots.begin(), Ballots.end(),
                                             [OwnBallot](const LabBallot& Ballot) -> bool
                                             {
                                                 return Ballot == OwnBallot;
                                             }) != Ballots.end();
                     }) != BallotHistory.end();
}

bool LabVoter::SubstituteBallot(std::vector<LabBallot>& Ballots, const bool bPrevious) const
{
    if (bPrevious && VoterIndex != Voters.size() - 1 || !bPrevious && VoterIndex != 0)
    {
        return false;
    }

    for (LabBallot& VictimBallot : Ballots)
    {
        if (!CheckOwnBallotPresence({VictimBallot}))
        {
            for (const LabBallot& MaliciousBallot : Ballots)
            {
                if (CheckOwnBallotPresence({MaliciousBallot}))
                {
                    VictimBallot = MaliciousBallot;
                    break;
                }
            }
            break;
        }
    }

    return true;
}
