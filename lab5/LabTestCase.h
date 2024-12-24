#ifndef LAB5_LABTESTCASE_H
#define LAB5_LABTESTCASE_H

#include <map>
#include <string>

enum class LabTestCase : unsigned
{
    Normal,
    InvalidCandidate,
    AbstainVoting,
    VoteAgain,
    UnregisteredVoter,
    VoteInsteadOfAnotherVoter,
    ECVoteInsteadOfAnotherVoter,
    AllECVoteInsteadOfAnotherVoter,
    CECVoteInsteadOfAnotherVoter,
    ECDropVote,
    AllECDropVote,
    CECDropVote,
    ECDuplicateVote,
    AllECDuplicateVote,
    CECDuplicateVote,
    ECSubstituteVote,
    AllECSubstituteVote,
    CECSubstituteVote,
};

static const std::map<LabTestCase, std::string> LabTestCaseNames =
{
        {LabTestCase::Normal, "Normal"},
        {LabTestCase::InvalidCandidate, "InvalidCandidate"},
        {LabTestCase::AbstainVoting, "AbstainVoting"},
        {LabTestCase::VoteAgain, "VoteAgain"},
        {LabTestCase::UnregisteredVoter, "UnregisteredVoter"},
        {LabTestCase::VoteInsteadOfAnotherVoter, "VoteInsteadOfAnotherVoter"},
        {LabTestCase::ECVoteInsteadOfAnotherVoter, "ECVoteInsteadOfAnotherVoter"},
        {LabTestCase::AllECVoteInsteadOfAnotherVoter, "AllECVoteInsteadOfAnotherVoter"},
        {LabTestCase::CECVoteInsteadOfAnotherVoter, "CECVoteInsteadOfAnotherVoter"},
        {LabTestCase::ECDropVote, "ECDropVote"},
        {LabTestCase::AllECDropVote, "AllECDropVote"},
        {LabTestCase::CECDropVote, "CECDropVote"},
        {LabTestCase::ECDuplicateVote, "ECDuplicateVote"},
        {LabTestCase::AllECDuplicateVote, "AllECDuplicateVote"},
        {LabTestCase::CECDuplicateVote, "CECDuplicateVote"},
        {LabTestCase::ECSubstituteVote, "ECSubstituteVote"},
        {LabTestCase::AllECSubstituteVote, "AllECSubstituteVote"},
        {LabTestCase::CECSubstituteVote, "CECSubstituteVote"},
};

extern LabTestCase GTestCase;

#endif //LAB5_LABTESTCASE_H
