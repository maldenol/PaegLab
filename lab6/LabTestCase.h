#ifndef LAB6_LABTESTCASE_H
#define LAB6_LABTESTCASE_H

#include <map>
#include <string>

enum class LabTestCase : unsigned
{
    Normal,
    AbstainRegister,
    RegisterAgain,
    AbstainReceiveToken,
    ReceiveTokenAgain,
    AbstainVoting,
    VoteAgain,
    InvalidCandidate,
    VoteInsteadOfAnotherVoter,
    ECVoteInsteadOfAnotherVoter,
    RBVoteInsteadOfAnotherVoter,
    ECSubstituteVote,
    ECRevealVoter,
    RBRevealVoter,
};

static const std::map<LabTestCase, std::string> LabTestCaseNames =
{
        {LabTestCase::Normal, "Normal"},
        {LabTestCase::AbstainRegister, "AbstainRegister"},
        {LabTestCase::RegisterAgain, "RegisterAgain"},
        {LabTestCase::AbstainReceiveToken, "AbstainReceiveToken"},
        {LabTestCase::ReceiveTokenAgain, "ReceiveTokenAgain"},
        {LabTestCase::AbstainVoting, "AbstainVoting"},
        {LabTestCase::VoteAgain, "VoteAgain"},
        {LabTestCase::InvalidCandidate, "InvalidCandidate"},
        {LabTestCase::VoteInsteadOfAnotherVoter, "VoteInsteadOfAnotherVoter"},
        {LabTestCase::ECVoteInsteadOfAnotherVoter, "ECVoteInsteadOfAnotherVoter"},
        {LabTestCase::RBVoteInsteadOfAnotherVoter, "RBVoteInsteadOfAnotherVoter"},
        {LabTestCase::ECSubstituteVote, "ECSubstituteVote"},
        {LabTestCase::ECRevealVoter, "ECRevealVoter"},
        {LabTestCase::RBRevealVoter, "RBRevealVoter"},
};

extern LabTestCase GTestCase;

#endif //LAB6_LABTESTCASE_H
