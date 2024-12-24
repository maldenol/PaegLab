/*
 * Лабораторна робота №3
 * Протоколи і алгоритми електронного голосування
 * Мальований Денис Олегович, ІО-13
 */

#include "LabBallot.h"
#include "LabCorruptElectionCommission.h"
#include "LabCorruptRegistrationBureau.h"
#include "LabElectionCommission.h"
#include "LabLocalVoter.h"
#include "LabPublicVoter.h"
#include "LabRegistrationBureau.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

enum class LabTestCase
{
    // Normal
    Normal,
    // Handled
    Voter_SkipVote,
    Voter_VoteAgain,
    Voter_SkipRegister,
    Voter_RegisterAgain,
    Voter_RegisterUnknown,
    Voter_InvalidCandidate,
    Voter_InvalidSignature,
    Voter_VoteInsteadOfAnotherVoter,
    Voter_InterceptBallot,
    Voter_RevealVoter,
    EC_VoteInsteadOfAnotherVoter,
    EC_ChangeVote,
    EC_RevealVoter,
    // Cannot prove
    RB_VoteInsteadOfAnotherVoter,
    RB_RevealVoter,
    RB_EC_ChangeVote,
};

void Test(const LabTestCase TestCase)
{
    bool bTestCaseExecuted = false;

    constexpr size_t CandidateNum = 2;
    constexpr size_t VoterNum = 4;

    LabRegistrationBureau RB{VoterNum};

    std::vector<LabLocalVoter> LocalVoters;
    LocalVoters.reserve(VoterNum);
    for (unsigned Index = 0; Index < VoterNum; ++Index)
    {
        if (TestCase == LabTestCase::Voter_SkipRegister && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            continue;
        }

        unsigned VoterId = Index + 1;
        if (TestCase == LabTestCase::Voter_RegisterUnknown && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            VoterId = 0;
        }

        LabLocalVoter LocalVoter;
        LocalVoter.VoterId = VoterId;
        RB.Register(LocalVoter);
        LocalVoters.push_back(LocalVoter);

        if (TestCase == LabTestCase::Voter_RegisterAgain && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            RB.Register(LocalVoter);
        }
    }

    LabElectionCommission EC = RB.CreateElectionCommission(CandidateNum);
    const LabCrypto::LabPublicKeyEG ECPublicKey = EC.GetPublicKey();

    if (TestCase == LabTestCase::EC_VoteInsteadOfAnotherVoter)
    {
        LabCorruptElectionCommission::VoteInsteadOfVoter(EC);
    }
    if (TestCase == LabTestCase::RB_VoteInsteadOfAnotherVoter)
    {
        LabCorruptRegistrationBureau::VoteInsteadOfVoter(RB, EC);
    }

    for (LabLocalVoter& LocalVoter : LocalVoters)
    {
        if (TestCase == LabTestCase::Voter_SkipVote && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            continue;
        }

        unsigned RegistrationId = LocalVoter.RegistrationId;
        if (TestCase == LabTestCase::Voter_VoteInsteadOfAnotherVoter && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            RegistrationId = LocalVoters[1].RegistrationId;
        }

        unsigned CandidateId = std::rand() % CandidateNum + 1;
        if (TestCase == LabTestCase::Voter_InvalidCandidate && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            CandidateId = 0;
        }

        LabBallot Ballot{RegistrationId};
        Ballot.SetCandidateId(CandidateId);

        LabCrypto::LabPrivateKeyDSA PrivateKey = LocalVoter.PrivateKey;
        if (TestCase == LabTestCase::Voter_InvalidSignature && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            PrivateKey = {};
        }

        const LabBallot SignedBallot = Ballot.Sign(PrivateKey);

        const LabBallotCipher SignedBallotCipher = SignedBallot.Encrypt(ECPublicKey);

        if (TestCase == LabTestCase::Voter_InterceptBallot && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            std::cout << "Third party: Intercept ballot: ciphered, cannot read\n";
        }

        std::cout << "Voter " << LocalVoter.VoterId << ": Voting for candidate " << CandidateId << "\n";
        EC.Vote(SignedBallotCipher);
        LocalVoter.Ballot = SignedBallot;

        if (TestCase == LabTestCase::Voter_VoteAgain && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            std::cout << "Voter " << LocalVoter.VoterId << ": Voting again for candidate " << CandidateId << "\n";
            EC.Vote(SignedBallotCipher);
        }
    }

    if (TestCase == LabTestCase::EC_ChangeVote)
    {
        LabCorruptElectionCommission::ChangeVote(EC);
    }
    if (TestCase == LabTestCase::RB_EC_ChangeVote)
    {
        LabCorruptElectionCommission::ChangeVoteWithRB(EC, RB);
    }

    std::cout << "RB: Requesting vote counting from EC\n";
    std::vector<LabCandidate> Candidates;
    std::vector<LabBallot> Ballots;
    std::vector<LabPublicVoter> PublicVoters;
    RB.GetResults(EC, Candidates, Ballots, PublicVoters);

    if (!Candidates.empty())
    {
        std::cout << "RB: Election results:\n";

        std::sort(Candidates.begin(), Candidates.end(),
                  [](const LabCandidate& CandidateLeft, const LabCandidate& CandidateRight) -> bool
                  {
                      return CandidateLeft.Votes > CandidateRight.Votes;
                  });

        for (const LabCandidate& Candidate : Candidates)
        {
            std::cout << "\tCandidate " << Candidate.CandidateId << ": " << Candidate.Votes << "\n";
        }

        bool bCanProve = false;

        std::cout << "RB: Submitted ballots:\n";
        for (const LabBallot& Ballot : Ballots)
        {
            const unsigned RegistrationId = Ballot.GetRegistrationId();
            const auto Voter = std::find_if(PublicVoters.begin(), PublicVoters.end(),
                                            [RegistrationId](const LabPublicVoter& Voter) -> bool
                                            {
                                                return Voter.RegistrationId == RegistrationId;
                                            });
            const bool bVerified = Ballot.Verify(Voter->PublicKey);

            if (!bVerified)
            {
                bCanProve = true;
            }

            std::cout << "\tBallot " << Ballot.GetRegistrationId() << " for candidate " << Ballot.GetCandidateId() << " ";
            std::cout << "(signature " << (bVerified ? "verified" : "NOT VERIFIED") << ")\n";
        }

        std::cout << "RB: Registration Ids with public DSA keys:\n";
        for (const LabPublicVoter& PublicVoter : PublicVoters)
        {
            std::cout << "\t" << PublicVoter.RegistrationId << "\n";
        }

        std::cout << "Voters:\n";
        bool bHaveIssues = false;
        for (const LabLocalVoter& LocalVoter : LocalVoters)
        {
            const LabBallot& LocalBallot = LocalVoter.Ballot;

            const unsigned RegistrationId = LocalBallot.GetRegistrationId();

            const auto SubmittedBallot = std::find_if(
                    Ballots.begin(), Ballots.end(),
                    [RegistrationId](const LabBallot& Ballot) -> bool
                    {
                        return Ballot.GetRegistrationId() == RegistrationId;
                    });

            if (SubmittedBallot == Ballots.end())
            {
                bHaveIssues = true;
                std::cout << "\tVoter " << LocalVoter.VoterId << "'s (" << LocalBallot.GetRegistrationId() << ") ";
                std::cout << "vote is not counted\n";
                continue;
            }

            if (SubmittedBallot->GetCandidateId() != LocalBallot.GetCandidateId())
            {
                bHaveIssues = true;
                std::cout << "\tVoter " << LocalVoter.VoterId << " (" << LocalBallot.GetRegistrationId() << ") ";
                std::cout << "voted for candidate " << LocalBallot.GetCandidateId() << " ";
                std::cout << "instead of " << SubmittedBallot->GetCandidateId() << "\n";
            }
        }
        if (bHaveIssues)
        {
            if (bCanProve)
            {
                std::cout << "\tCan prove the election results are invalid\n";
            }
            else
            {
                std::cout << "\tCANNOT prove the election results are invalid\n";
            }
        }
        else
        {
            std::cout << "\tNo issues\n";
        }

        if (TestCase == LabTestCase::Voter_RevealVoter)
        {
            std::cout << "Third party: Trying to reveal voter:\n";
            std::cout << "\tRegistrationId: " << Ballots[0].GetRegistrationId() << "\n";
            std::cout << "\tCandidateId: " << Ballots[0].GetCandidateId() << "\n";
            std::cout << "\tVoterId: Unknown\n";
        }
        if (TestCase == LabTestCase::EC_RevealVoter)
        {
            LabCorruptElectionCommission::RevealVoter(EC);
        }
        if (TestCase == LabTestCase::RB_RevealVoter)
        {
            LabCorruptRegistrationBureau::RevealVoter(RB, Ballots[0]);
        }
    }
}

int main()
{
    std::srand(std::time(nullptr));

    std::cout << "\nNormal\n";
    Test(LabTestCase::Normal);

    std::cout << "\nVoter_SkipVote\n";
    Test(LabTestCase::Voter_SkipVote);
    std::cout << "\nVoter_VoteAgain\n";
    Test(LabTestCase::Voter_VoteAgain);
    std::cout << "\nVoter_SkipRegister\n";
    Test(LabTestCase::Voter_SkipRegister);
    std::cout << "\nVoter_RegisterAgain\n";
    Test(LabTestCase::Voter_RegisterAgain);
    std::cout << "\nVoter_RegisterUnknown\n";
    Test(LabTestCase::Voter_RegisterUnknown);
    std::cout << "\nVoter_InvalidCandidate\n";
    Test(LabTestCase::Voter_InvalidCandidate);
    std::cout << "\nVoter_InvalidSignature\n";
    Test(LabTestCase::Voter_InvalidSignature);
    std::cout << "\nVoter_VoteInsteadOfAnotherVoter\n";
    Test(LabTestCase::Voter_VoteInsteadOfAnotherVoter);
    std::cout << "\nVoter_InterceptBallot\n";
    Test(LabTestCase::Voter_InterceptBallot);
    std::cout << "\nVoter_RevealVoter\n";
    Test(LabTestCase::Voter_RevealVoter);

    std::cout << "\nEC_VoteInsteadOfAnotherVoter\n";
    Test(LabTestCase::EC_VoteInsteadOfAnotherVoter);
    std::cout << "\nEC_ChangeVote\n";
    Test(LabTestCase::EC_ChangeVote);
    std::cout << "\nEC_RevealVoter\n";
    Test(LabTestCase::EC_RevealVoter);

    std::cout << "\nRB_VoteInsteadOfAnotherVoter\n";
    Test(LabTestCase::RB_VoteInsteadOfAnotherVoter);
    std::cout << "\nRB_RevealVoter\n";
    Test(LabTestCase::RB_RevealVoter);
    std::cout << "\nRB_EC_ChangeVote\n";
    Test(LabTestCase::RB_EC_ChangeVote);

    return 0;
}
