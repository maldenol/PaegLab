/*
 * Лабораторна робота №1
 * Протоколи і алгоритми електронного голосування
 * Мальований Денис Олегович, ІО-13
 */

#include "LabBallotPaper.h"
#include "LabCandidate.h"
#include "LabCentralElectionCommission.h"
#include "LabCorruptCentralElectionCommission.h"
#include "LabVoter.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

enum class LabTestCase
{
    // Normal
    Normal,
    // Handled
    SkipVote,
    InvalidVote,
    InvalidVoter,
    VoteAgain,
    VoterVoteInsteadOfAnotherVoter,
    // Not handled
    RevealVoter,
    CECVoteInsteadOfVoter,
    CECChangeVote,
};

void Test(const LabTestCase TestCase)
{
    bool bTestCaseExecuted = false;

    constexpr size_t CandidateNum = 2;
    constexpr size_t VoterNum = 4;

    LabCentralElectionCommission CentralElectionCommission{CandidateNum, VoterNum};

    if (TestCase == LabTestCase::CECVoteInsteadOfVoter)
    {
        LabCorruptCentralElectionCommission::VoteInsteadOfVoter(CentralElectionCommission, 1, 1);
    }

    for (const LabVoter& Voter : CentralElectionCommission.GetVoters())
    {
        if (TestCase == LabTestCase::SkipVote && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            continue;
        }

        unsigned VoterId = Voter.VoterId;
        if (TestCase == LabTestCase::InvalidVoter && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            VoterId = 0;
        }
        if (TestCase == LabTestCase::VoterVoteInsteadOfAnotherVoter && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            ++VoterId;
        }

        unsigned CandidateId = std::rand() % CandidateNum + 1;
        if (TestCase == LabTestCase::InvalidVote && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            CandidateId = 0;
        }

        LabBallotPaper BallotPaper{VoterId};
        BallotPaper.SetCandidateId(CandidateId);
        BallotPaper.Sign(Voter.PrivateKey);
        const LabBallotPaper EncryptedBallotPaper = BallotPaper.Encrypt(Voter.Gamma);
        std::cout << "Voter " << VoterId << ": voting for candidate " << CandidateId << "\n";
        CentralElectionCommission.Vote(VoterId, EncryptedBallotPaper);

        if (TestCase == LabTestCase::VoteAgain && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            std::cout << "Voter " << VoterId << ": voting for candidate " << CandidateId << "\n";
            CentralElectionCommission.Vote(VoterId, EncryptedBallotPaper);
        }
    }

    if (TestCase == LabTestCase::RevealVoter)
    {
        LabCorruptCentralElectionCommission::RevealVoter(CentralElectionCommission, 1);
    }

    if (TestCase == LabTestCase::CECChangeVote)
    {
        LabCorruptCentralElectionCommission::ChangeVote(CentralElectionCommission, 1, 1);
    }

    if (CentralElectionCommission.CountVotes())
    {
        std::cout << "Election results:\n";

        std::vector<LabCandidate> Candidates = CentralElectionCommission.GetCandidates();
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

    std::cout << "\nNormal\n";
    Test(LabTestCase::Normal);
    std::cout << "\nSkipVote\n";
    Test(LabTestCase::SkipVote);
    std::cout << "\nInvalidVote\n";
    Test(LabTestCase::InvalidVote);
    std::cout << "\nInvalidVoter\n";
    Test(LabTestCase::InvalidVoter);
    std::cout << "\nVoteAgain\n";
    Test(LabTestCase::VoteAgain);
    std::cout << "\nVoterVoteInsteadOfAnotherVoter\n";
    Test(LabTestCase::VoterVoteInsteadOfAnotherVoter);
    std::cout << "\nRevealVoter\n";
    Test(LabTestCase::RevealVoter);
    std::cout << "\nCECVoteInsteadOfVoter\n";
    Test(LabTestCase::CECVoteInsteadOfVoter);
    std::cout << "\nCECChangeVote\n";
    Test(LabTestCase::CECChangeVote);

    return 0;
}
