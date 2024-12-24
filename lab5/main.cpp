/*
 * Лабораторна робота №5
 * Протоколи і алгоритми електронного голосування
 * Мальований Денис Олегович, ІО-13
 */

#include "LabBallot.h"
#include "LabBigInt.h"
#include "LabCandidate.h"
#include "LabCentralElectionCommission.h"
#include "LabCorruptCentralElectionCommission.h"
#include "LabCorruptElectionCommission.h"
#include "LabCrypto.h"
#include "LabElectionCommission.h"
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
    constexpr size_t ECNum = 2;

    LabCentralElectionCommission CEC{VoterNum, CandidateNum};
    const LabCrypto::LabPublicKeyRSA& EncryptKey = CEC.GetEncryptKey();

    std::vector<LabVoter> Voters = CEC.GetVoters();
    std::cout << "Voters:\n";
    for (const LabVoter& Voter : Voters)
    {
        std::cout << "\t" << Voter.GetVoterId() << "\n";
    }

    const std::vector<LabCandidate>& Candidates = CEC.GetCandidates();
    std::cout << "Candidates:\n";
    for (const LabCandidate& Candidate : Candidates)
    {
        std::cout << "\t" << Candidate.CandidateId << "\n";
    }

    std::vector<LabElectionCommission> ECs;
    ECs.reserve(ECNum);
    for (size_t Index = 0; Index < ECNum; ++Index)
    {
        const unsigned ElectionCommissionId = Index + 1;
        ECs.emplace_back(ElectionCommissionId, Voters, Candidates);
    }

    if (GTestCase == LabTestCase::UnregisteredVoter)
    {
        GTestCase = LabTestCase::Normal;
        const std::vector<LabBallot> Ballots = LabVoter{0}.CreateBallots(99, ECNum, EncryptKey);
        for (size_t Index = 0; Index < Ballots.size(); ++Index)
        {
            ECs[Index].Vote(Ballots[Index]);
        }
    }
    else if (GTestCase == LabTestCase::VoteInsteadOfAnotherVoter)
    {
        GTestCase = LabTestCase::Normal;
        std::vector<LabBallot> Ballots = Voters[0].CreateBallotsForVoter(Candidates[0].CandidateId, ECNum, EncryptKey, Voters[1].GetVoterId());
        for (size_t Index = 0; Index < Ballots.size(); ++Index)
        {
            ECs[Index].Vote(Ballots[Index]);
        }
    }
    else if (GTestCase == LabTestCase::ECVoteInsteadOfAnotherVoter)
    {
        GTestCase = LabTestCase::Normal;
        LabCorruptElectionCommission::VoteInsteadOfAnotherVoter(ECs[0], EncryptKey, ECNum);
    }
    else if (GTestCase == LabTestCase::AllECVoteInsteadOfAnotherVoter)
    {
        GTestCase = LabTestCase::Normal;
        LabCorruptElectionCommission::VoteInsteadOfAnotherVoterAll(ECs, EncryptKey);
    }
    else if (GTestCase == LabTestCase::CECVoteInsteadOfAnotherVoter)
    {
        GTestCase = LabTestCase::Normal;
        LabCorruptCentralElectionCommission::VoteInsteadOfAnotherVoter(CEC, ECs);
    }

    for (LabVoter& Voter : Voters)
    {
        if (GTestCase == LabTestCase::AbstainVoting)
        {
            GTestCase = LabTestCase::Normal;
            continue;
        }

        const size_t CandidateIndex = LabBigInt::RandomRange(0, CandidateNum - 1);
        unsigned CandidateId = Candidates[CandidateIndex].CandidateId;

        if (GTestCase == LabTestCase::InvalidCandidate)
        {
            GTestCase = LabTestCase::Normal;
            CandidateId = 99;
        }

        std::cout << "Voter " << Voter.GetVoterId() << ": Voting for candidate " << CandidateId << "\n";

        const std::vector<LabBallot> Ballots = Voter.CreateBallots(CandidateId, ECNum, EncryptKey);

        for (size_t Index = 0; Index < Ballots.size(); ++Index)
        {
            ECs[Index].Vote(Ballots[Index]);
        }

        if (GTestCase == LabTestCase::VoteAgain)
        {
            GTestCase = LabTestCase::Normal;
            std::cout << "Voting again\n";
            for (size_t Index = 0; Index < Ballots.size(); ++Index)
            {
                ECs[Index].Vote(Ballots[Index]);
            }
        }
    }

    bool bCorruptEC = true;
    if (GTestCase == LabTestCase::ECDropVote)
    {
        GTestCase = LabTestCase::Normal;
        LabCorruptElectionCommission::DropVote(ECs[0]);
    }
    else if (GTestCase == LabTestCase::AllECDropVote)
    {
        GTestCase = LabTestCase::Normal;
        LabCorruptElectionCommission::DropVoteAll(ECs);
    }
    else if (GTestCase == LabTestCase::ECDuplicateVote)
    {
        GTestCase = LabTestCase::Normal;
        LabCorruptElectionCommission::DuplicateVote(ECs[0]);
    }
    else if (GTestCase == LabTestCase::AllECDuplicateVote)
    {
        GTestCase = LabTestCase::Normal;
        LabCorruptElectionCommission::DuplicateVoteAll(ECs);
    }
    else if (GTestCase == LabTestCase::ECSubstituteVote)
    {
        GTestCase = LabTestCase::Normal;
        LabCorruptElectionCommission::SubstituteVote(ECs[0], EncryptKey, ECNum);
    }
    else if (GTestCase == LabTestCase::AllECSubstituteVote)
    {
        GTestCase = LabTestCase::Normal;
        LabCorruptElectionCommission::SubstituteVoteAll(ECs, EncryptKey);
    }
    else
    {
        bCorruptEC = false;
    }

    for (const LabElectionCommission& EC : ECs)
    {
        const std::vector<LabBallot> ECBallots = EC.GetBallots(bCorruptEC);

        if (ECBallots.empty())
        {
            std::cout << "Election failed\n";
            return;
        }

        std::cout << "EC " << EC.GetElectionCommissionId() << ": Submitted ballots:\n";
        for (const LabBallot& Ballot : ECBallots)
        {
            std::cout << "\tBallot from voter " << Ballot.GetVoterId() << " for candidate " << Ballot.GetCandidateId().ToInt() << "\n";
        }

        for (const LabVoter& Voter : Voters)
        {
            Voter.VerifyOwnBallotPresence(ECBallots, true);
        }
    }

    std::vector<LabBallot> Ballots;
    bool bSuccess = CEC.ReceiveBallots(ECs);
    if (bSuccess)
    {
        if (GTestCase == LabTestCase::CECDropVote)
        {
            GTestCase = LabTestCase::Normal;
            LabCorruptCentralElectionCommission::DropVote(CEC);
        }
        else if (GTestCase == LabTestCase::CECDuplicateVote)
        {
            GTestCase = LabTestCase::Normal;
            LabCorruptCentralElectionCommission::DuplicateVote(CEC);
        }
        else if (GTestCase == LabTestCase::CECSubstituteVote)
        {
            GTestCase = LabTestCase::Normal;
            LabCorruptCentralElectionCommission::SubstituteVote(CEC);
        }

        bSuccess = CEC.CountVotes(Ballots);
    }

    if (bSuccess)
    {
        std::vector<LabCandidate> ResultCandidates = CEC.GetCandidates();
        std::sort(ResultCandidates.begin(), ResultCandidates.end(),
                  [](const LabCandidate& CandidateLeft, const LabCandidate& CandidateRight) -> bool
                  {
                      return CandidateLeft.Votes > CandidateRight.Votes;
                  });

        std::cout << "CEC: Election results:\n";
        for (const LabCandidate& Candidate : ResultCandidates)
        {
            std::cout << "\tCandidate " << Candidate.CandidateId << ": " << Candidate.Votes << "\n";
        }

        std::cout << "CEC: Submitted ballots:\n";
        for (const LabBallot& Ballot : Ballots)
        {
            std::cout << "\tBallot from voter " << Ballot.GetVoterId() << " for candidate " << Ballot.GetCandidateId().ToInt() << "\n";
        }

        for (const LabVoter& Voter : Voters)
        {
            Voter.VerifyOwnBallotPresence(Ballots, false);
        }
    }
    else
    {
        std::cout << "Election failed\n";
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
