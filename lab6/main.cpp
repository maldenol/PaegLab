/*
 * Лабораторна робота №6
 * Протоколи і алгоритми електронного голосування
 * Мальований Денис Олегович, ІО-13
 */

#include "LabBallot.h"
#include "LabBigInt.h"
#include "LabCandidate.h"
#include "LabCorruptElectionCommission.h"
#include "LabCorruptRegistrationBureau.h"
#include "LabCrypto.h"
#include "LabElectionCommission.h"
#include "LabRegistrationBureau.h"
#include "LabTestCase.h"
#include "LabVoter.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

LabTestCase GTestCase;

void Test()
{
    constexpr size_t CandidateNum = 2;
    constexpr size_t VoterNum = 4;

    LabRegistrationBureau RB{VoterNum};

    std::vector<LabVoter> Voters;
    Voters.reserve(VoterNum);
    for (unsigned Index = 0; Index < VoterNum; ++Index)
    {
        const unsigned VoterId = Index + 1;

        Voters.emplace_back(VoterId);

        if (GTestCase == LabTestCase::AbstainRegister)
        {
            GTestCase = LabTestCase::Normal;
            continue;
        }

        RB.Register(VoterId);

        if (GTestCase == LabTestCase::RegisterAgain)
        {
            GTestCase = LabTestCase::Normal;
            RB.Register(VoterId);
        }
    }

    LabElectionCommission EC{RB.GetRegistrationIds(), CandidateNum};

    RB.SetVoterTokens(EC.GetVoterTokens());

    const LabCrypto::LabPublicKeyEG& PublicKeyEG = EC.GetPublicKeyEG();

    for (LabVoter& Voter : Voters)
    {
        if (GTestCase == LabTestCase::AbstainReceiveToken)
        {
            GTestCase = LabTestCase::Normal;
            continue;
        }

        const unsigned VoterId = Voter.GetVoterId();
        const LabVoterToken VoterToken = RB.GetToken(VoterId);
        Voter.SetVoterToken(VoterToken);

        if (GTestCase == LabTestCase::ReceiveTokenAgain)
        {
            GTestCase = LabTestCase::Normal;
            RB.GetToken(VoterId);
        }
    }

    std::cout << "Voters:\n";
    for (const LabVoter& Voter : Voters)
    {
        std::cout << "\t" << Voter.GetVoterId() << " " << Voter.GetVoterToken().RegistrationId << "\n";
    }

    const std::vector<LabCandidate>& Candidates = EC.GetCandidates();
    std::cout << "Candidates:\n";
    for (const LabCandidate& Candidate : Candidates)
    {
        std::cout << "\t" << Candidate.CandidateId << "\n";
    }

    if (GTestCase == LabTestCase::ECVoteInsteadOfAnotherVoter)
    {
        GTestCase = LabTestCase::Normal;
        LabCorruptElectionCommission::VoteInsteadOfAnotherVoter(EC);
    }
    else if (GTestCase == LabTestCase::RBVoteInsteadOfAnotherVoter)
    {
        GTestCase = LabTestCase::Normal;
        LabCorruptRegistrationBureau::VoteInsteadOfAnotherVoter(RB, EC);
    }

    for (const LabVoter& Voter : Voters)
    {
        if (GTestCase == LabTestCase::AbstainVoting)
        {
            GTestCase = LabTestCase::Normal;
            continue;
        }

        const LabVoterToken& VoterToken = Voter.GetVoterToken();

        unsigned RegistrationId = VoterToken.RegistrationId;

        if (GTestCase == LabTestCase::VoteInsteadOfAnotherVoter)
        {
            GTestCase = LabTestCase::Normal;
            RegistrationId = Voters[1].GetVoterToken().RegistrationId;
            std::cout << "Voter " << Voter.GetVoterId() << ": Voting instead of voter " << Voters[1].GetVoterId() << "\n";
        }

        LabBallot Ballot;
        Ballot.SetRegistrationId(RegistrationId);

        const size_t CandidateIndex = LabBigInt::RandomRange(0, CandidateNum - 1);
        unsigned CandidateId = Candidates[CandidateIndex].CandidateId;

        if (GTestCase == LabTestCase::InvalidCandidate)
        {
            GTestCase = LabTestCase::Normal;
            CandidateId = 99;
        }

        Ballot.SetCandidateId(CandidateId);

        const std::vector<LabBigInt> BallotCipher = Ballot.Encrypt(VoterToken.PublicKeyBBS, PublicKeyEG);

        std::cout << "Voter " << Voter.GetVoterId() << ": Voting for candidate " << CandidateId << "\n";
        EC.Vote(BallotCipher);

        if (GTestCase == LabTestCase::VoteAgain)
        {
            GTestCase = LabTestCase::Normal;
            std::cout << "Voter " << Voter.GetVoterId() << ": Voting again for candidate " << CandidateId << "\n";
            EC.Vote(BallotCipher);
        }
    }

    if (GTestCase == LabTestCase::ECSubstituteVote)
    {
        GTestCase = LabTestCase::Normal;
        LabCorruptElectionCommission::SubstituteVote(EC);
    }
    else if (GTestCase == LabTestCase::ECRevealVoter)
    {
        GTestCase = LabTestCase::Normal;
        LabCorruptElectionCommission::RevealVoter(EC);
    }
    else if (GTestCase == LabTestCase::RBRevealVoter)
    {
        GTestCase = LabTestCase::Normal;
        LabCorruptRegistrationBureau::RevealVoter(RB);
    }

    if (EC.CountVotes())
    {
        std::vector<LabCandidate> ResultCandidates = EC.GetCandidates();
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
