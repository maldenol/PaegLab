/*
 * Лабораторна робота №4
 * Протоколи і алгоритми електронного голосування
 * Мальований Денис Олегович, ІО-13
 */

#include "LabBallot.h"
#include "LabCandidate.h"
#include "LabVoter.h"
#include "LabTestCase.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

LabTestCase GTestCase;

void Test()
{
    constexpr size_t CandidateNum = 2;
    constexpr size_t VoterNum = 4;

    std::vector<LabVoter> Voters;
    std::vector<LabVoter*> VoterPtrs;
    Voters.reserve(VoterNum + 1);
    for (size_t Index = 0; Index < VoterNum; ++Index)
    {
        const unsigned VoterId = Index + 1;
        LabVoter Voter{VoterId};
        Voters.push_back(Voter);
        VoterPtrs.push_back(&Voters[Index]);
    }
    for (size_t Index = 0; Index < VoterNum; ++Index)
    {
        Voters[Index].SetVoters(VoterPtrs);
    }

    if (GTestCase == LabTestCase::UnregisteredVoter)
    {
        GTestCase = LabTestCase::Normal;
        const unsigned VoterId = VoterNum + 1;
        LabVoter Voter{VoterId};
        Voter.SetVoters(VoterPtrs);
        Voters.push_back(Voter);
    }

    std::vector<LabBallot> Ballots;
    Ballots.reserve(VoterNum + 1);
    for (LabVoter& Voter : Voters)
    {
        if (GTestCase == LabTestCase::AbstainVoting)
        {
            GTestCase = LabTestCase::Normal;
            continue;
        }

        unsigned CandidateId = std::rand() % CandidateNum + 1;
        if (GTestCase == LabTestCase::InvalidCandidate)
        {
            GTestCase = LabTestCase::Normal;
            CandidateId = CandidateNum + 1;
        }

        const LabBallot Ballot = Voter.CreateBallot(CandidateId);
        Ballots.push_back(Ballot);

        if (GTestCase == LabTestCase::VoteAgain)
        {
            GTestCase = LabTestCase::Normal;
            const LabBallot AnotherBallot = Voter.CreateBallot(CandidateId);
            Ballots.push_back(Ballot);
        }
    }

    if (GTestCase == LabTestCase::VoteInsteadOfAnotherVoter)
    {
        GTestCase = LabTestCase::Normal;
        const unsigned MaliciousVoterIndex = 0;
        const unsigned VictimVoterIndex = 1;
        Ballots[VictimVoterIndex] = Ballots[MaliciousVoterIndex];
        std::cout << "Voter " << MaliciousVoterIndex + 1 << ": Voting instead of voter " << VictimVoterIndex + 1 << "\n";
    }

    LabVoter::ShuffleBallots(Ballots);

    const bool bSuccess = Voters[0].RunElection(Ballots);

    if (bSuccess)
    {
        std::vector<LabCandidate> Candidates;
        Candidates.reserve(CandidateNum);
        for (size_t Index = 0; Index < CandidateNum; ++Index)
        {
            const unsigned CandidateId = Index + 1;

            LabCandidate Candidate;
            Candidate.CandidateId = CandidateId;
            for (const LabBallot& Ballot : Ballots)
            {
                const unsigned BallotCandidateId = Ballot.GetCandidateId();

                if (BallotCandidateId == 0 || BallotCandidateId > CandidateNum)
                {
                    std::cout << "Error: Election failed, vote for invalid candidate detected\n";
                    return;
                }

                if (BallotCandidateId == CandidateId)
                {
                    ++Candidate.Votes;
                }
            }

            Candidates.push_back(Candidate);
        }

        std::cout << "Election results:\n";

        std::sort(Candidates.begin(), Candidates.end(),
                  [](const LabCandidate& CandidateLeft, const LabCandidate& CandidateRight) -> bool
                  {
                      return CandidateLeft.Votes > CandidateRight.Votes;
                  });

        for (const LabCandidate& Candidate : Candidates)
        {
            std::cout << "\tCandidate " << Candidate.CandidateId << ": " << Candidate.Votes << "\n";
        }
    }
}

int main()
{
    std::srand(std::time(nullptr));

    for (const std::pair<LabTestCase, std::string> TestCaseName : LabTestCaseNames)
    {
        std::cout << TestCaseName.second << std::endl;
        GTestCase = TestCaseName.first;
        Test();
        std::cout << std::endl;
    }

    return 0;
}
