/*
 * Лабораторна робота №2
 * Протоколи і алгоритми електронного голосування
 * Мальований Денис Олегович, ІО-13
 */

#include "LabBallot.h"
#include "LabMaskedBallotSetArray.h"
#include "LabCandidate.h"
#include "LabCentralElectionCommission.h"
#include "LabCorruptCentralElectionCommission.h"
#include "LabVoter.h"
#include "LabLocalVoter.h"

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
    InvalidVoter,
    InvalidCandidate,
    VoteUnsigned,
    SignMaskedBallotSetAgain,
    VoteSignedAgain,
    VoterVoteInsteadOfAnotherVoter,
    RevealVoter,
    CECRevealVoterBallotIdDuringUnmasking,
    // Partially handled
    CECChangeVote,
    // Not handled
    CECVoteInsteadOfVoter,
};

void Test(const LabTestCase TestCase)
{
    bool bTestCaseExecuted = false;

    constexpr size_t CandidateNum = 2;
    constexpr size_t VoterNum = 4;

    LabCentralElectionCommission CEC{CandidateNum, VoterNum};
    const LabRSA::LabPublicKey CECPublicKey = CEC.GetPublicKey();
    const std::vector<LabVoter> Voters = CEC.GetVoters();

    std::vector<LabLocalVoter> LocalVoters;
    LocalVoters.reserve(Voters.size());
    for (const LabVoter& Voter : Voters)
    {
        LabLocalVoter LocalVoter;
        LocalVoter.VoterId = Voter.VoterId;
        LocalVoters.push_back(LocalVoter);
    }

    if (TestCase == LabTestCase::CECVoteInsteadOfVoter)
    {
        LabCorruptCentralElectionCommission::VoteInsteadOfVoter(CEC, 1, 2);
    }

    for (size_t VoterIndex = 0; VoterIndex < Voters.size(); ++VoterIndex)
    {
        if (TestCase == LabTestCase::SkipVote && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            continue;
        }

        const LabVoter& Voter = Voters[VoterIndex];
        LabLocalVoter& LocalVoter = LocalVoters[VoterIndex];

        unsigned VoterId = Voter.VoterId;
        if (TestCase == LabTestCase::InvalidVoter && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            VoterId = 0;
        }
        if (TestCase == LabTestCase::VoterVoteInsteadOfAnotherVoter && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            VoterId = 2;
        }

        std::cout << "Voter " << VoterId << ": Generating masked ballot sets and sending to CEC for masked signing\n";

        std::vector<LabBallot> SignedMaskedBallotSet;
        LabMaskedBallotSetArray MaskedBallotSetArray;

        bool bReceivedSignedMaskedBallotSet = false;
        while (!bReceivedSignedMaskedBallotSet)
        {
            for (size_t Index = 0; Index < LabMaskedBallotSetArray::Size; ++Index) {
                const unsigned BallotId = std::rand() % LabBallot::MaxBallotId;
                const unsigned R = LabRSA::GenerateR(CECPublicKey);

                std::vector<LabBallot> MaskedBallotSet;
                MaskedBallotSet.reserve(CandidateNum);

                for (size_t CandidateId = 1; CandidateId <= CandidateNum; ++CandidateId) {
                    LabBallot Ballot{BallotId};
                    Ballot.SetCandidateId(CandidateId);

                    if (TestCase == LabTestCase::InvalidCandidate && CandidateId == 1)
                    {
                        Ballot.SetCandidateId(0);
                    }

                    const LabBallot MaskedBallot = Ballot.Mask(CECPublicKey, R);
                    MaskedBallotSet.push_back(MaskedBallot);
                }

                MaskedBallotSetArray.SetBallotSet(Index, MaskedBallotSet, R);
            }

            if (TestCase == LabTestCase::CECRevealVoterBallotIdDuringUnmasking && !bTestCaseExecuted)
            {
                bTestCaseExecuted = true;
                LabCorruptCentralElectionCommission::RevealVoterBallotIdDuringUnmasking(CEC, 1, MaskedBallotSetArray);
            }

            bReceivedSignedMaskedBallotSet = CEC.SignMaskedBallotSet(VoterId, MaskedBallotSetArray, SignedMaskedBallotSet) != LabSignMaskedBallotSetResult::DuplicateBallotId;
            if (!bReceivedSignedMaskedBallotSet)
            {
                std::cout << "Voter " << VoterId << ": Regenerating masked ballot sets due to duplicate BallotId\n";
            }
        }

        if (SignedMaskedBallotSet.empty())
        {
            std::cout << "Voter " << VoterId << ": Did not receive signed masked ballot set from CEC\n";
            continue;
        }
        std::cout << "Voter " << VoterId << ": Received signed masked ballot set from CEC\n";

        unsigned CandidateId = std::rand() % CandidateNum + 1;
        if (TestCase == LabTestCase::InvalidCandidate && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            CandidateId = 0;
        }

        if (TestCase == LabTestCase::VoteUnsigned && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            const unsigned BallotId = std::rand() % LabBallot::MaxBallotId;
            LabBallot UnsignedBallot{BallotId};
            UnsignedBallot.SetCandidateId(CandidateId);
            const LabBallot EncryptedUnsignedBallot = UnsignedBallot.Encrypt(CECPublicKey);
            std::cout << "Voter " << VoterId << ": Unsigned voting for candidate " << CandidateId << " ";
            std::cout << "(" << BallotId << ")\n";
            CEC.Vote(EncryptedUnsignedBallot);
            continue;
        }

        bool bVoted = false;
        for (const LabBallot& SignedMaskedBallot : SignedMaskedBallotSet)
        {
            const unsigned R = MaskedBallotSetArray.GetRForUnmaskingSignedBallotSet();
            const LabBallot SignedBallot = SignedMaskedBallot.Unmask(CECPublicKey, R);

            if (SignedBallot.GetCandidateId() != CandidateId)
            {
                continue;
            }

            if (!SignedBallot.Verify(CECPublicKey))
            {
                std::cout << "Voter " << VoterId << ": Invalid CEC signature\n";
                break;
            }

            const LabBallot EncryptedSignedBallot = SignedBallot.Encrypt(CECPublicKey);

            std::cout << "Voter " << VoterId << ": Voting for candidate " << CandidateId << " ";
            std::cout << "(" << SignedBallot.GetBallotId() << ")\n";
            CEC.Vote(EncryptedSignedBallot);

            if (TestCase == LabTestCase::VoteSignedAgain && !bTestCaseExecuted)
            {
                bTestCaseExecuted = true;
                std::cout << "Voter " << VoterId << ": Voting for candidate " << CandidateId << " ";
                std::cout << "(" << SignedBallot.GetBallotId() << ")\n";
                CEC.Vote(EncryptedSignedBallot);
            }

            LocalVoter.SubmittedBallot = SignedBallot;

            bVoted = true;

            break;
        }
        if (!bVoted)
        {
            std::cout << "Voter " << VoterId << ": CEC did not return ballot for candidate " << CandidateId << "\n";
        }

        if (TestCase == LabTestCase::SignMaskedBallotSetAgain && !bTestCaseExecuted)
        {
            bTestCaseExecuted = true;
            --VoterIndex;
            std::cout << "Voter " << VoterId << ": Trying to sign unmasked ballot sets again\n";
        }
    }

    if (TestCase == LabTestCase::CECChangeVote)
    {
        const unsigned BallotId = CEC.GetSignedBallots()[0].GetBallotId();
        LabCorruptCentralElectionCommission::ChangeVote(CEC, BallotId);
    }

    if (CEC.CountVotes())
    {
        std::cout << "CEC: Election results:\n";

        std::vector<LabCandidate> Candidates = CEC.GetCandidates();
        std::sort(Candidates.begin(), Candidates.end(),
                  [](const LabCandidate& CandidateLeft, const LabCandidate& CandidateRight) -> bool
                  {
                      return CandidateLeft.Votes > CandidateRight.Votes;
                  });

        for (const LabCandidate& Candidate : Candidates)
        {
            std::cout << "\tCandidate " << Candidate.CandidateId << ": " << Candidate.Votes << "\n";
        }

        std::cout << "CEC: Submitted ballots:\n";
        for (const LabBallot& SignedBallot : CEC.GetSignedBallots())
        {
            std::cout << "\tBallot " << SignedBallot.GetBallotId() << " for candidate " << SignedBallot.GetCandidateId() << " ";
            std::cout << "(signature " << (SignedBallot.Verify(CECPublicKey) ? "verified" : "NOT VERIFIED") << ")\n";
        }

        std::cout << "Issues found by voters (can do nothing though):\n";
        bool bHaveIssues = false;
        for (const LabLocalVoter& LocalVoter : LocalVoters)
        {
            const LabBallot& LocalSubmittedBallot = LocalVoter.SubmittedBallot;

            const LabBallot* SubmittedBallot = nullptr;
            for (const LabBallot& SignedBallot : CEC.GetSignedBallots())
            {
                if (SignedBallot.GetBallotId() == LocalSubmittedBallot.GetBallotId())
                {
                    SubmittedBallot = &SignedBallot;
                    break;
                }
            }

            if (!SubmittedBallot)
            {
                bHaveIssues = true;
                std::cout << "\tVoter " << LocalVoter.VoterId << "'s (" << LocalSubmittedBallot.GetBallotId() << ") ";
                std::cout << "vote is not counted\n";
                continue;
            }

            if (SubmittedBallot->GetCandidateId() != LocalSubmittedBallot.GetCandidateId())
            {
                bHaveIssues = true;
                std::cout << "\tVoter " << LocalVoter.VoterId << " (" << LocalSubmittedBallot.GetBallotId() << ") ";
                std::cout << "voted for candidate " << LocalSubmittedBallot.GetCandidateId() << " ";
                std::cout << "instead of " << SubmittedBallot->GetCandidateId() << "\n";
            }
        }
        if (!bHaveIssues)
        {
            std::cout << "\tNo issues\n";
        }

        if (TestCase == LabTestCase::RevealVoter)
        {
            const LabBallot& Ballot = CEC.GetSignedBallots()[0];
            std::cout << "Someone: Trying to reveal VoterId of ballot " << Ballot.GetBallotId() << ": there is no way\n";
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
    std::cout << "\nInvalidVoter\n";
    Test(LabTestCase::InvalidVoter);
    std::cout << "\nInvalidCandidate\n";
    Test(LabTestCase::InvalidCandidate);
    std::cout << "\nVoteUnsigned\n";
    Test(LabTestCase::VoteUnsigned);
    std::cout << "\nSignMaskedBallotSetAgain\n";
    Test(LabTestCase::SignMaskedBallotSetAgain);
    std::cout << "\nVoteSignedAgain\n";
    Test(LabTestCase::VoteSignedAgain);
    std::cout << "\nVoterVoteInsteadOfAnotherVoter\n";
    Test(LabTestCase::VoterVoteInsteadOfAnotherVoter);
    std::cout << "\nRevealVoter\n";
    Test(LabTestCase::RevealVoter);
    std::cout << "\nCECRevealVoterBallotIdDuringUnmasking\n";
    Test(LabTestCase::CECRevealVoterBallotIdDuringUnmasking);
    std::cout << "\nCECChangeVote\n";
    Test(LabTestCase::CECChangeVote);
    std::cout << "\nCECVoteInsteadOfVoter\n";
    Test(LabTestCase::CECVoteInsteadOfVoter);

    return 0;
}
