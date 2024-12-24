#ifndef LAB4_LABTESTCASE_H
#define LAB4_LABTESTCASE_H

#include <map>
#include <string>

enum class LabTestCase : unsigned
{
    Normal,
    AbstainVoting,
    VoteAgain,
    UnregisteredVoter,
    VoteInsteadOfAnotherVoter,
    InvalidCandidate,
    StageOneDropVote,
    StageTwoDropVote,
    StageOneDuplicateVote,
    StageTwoDuplicateVote,
    StageOneSubstituteVotePrevious,
    StageOneSubstituteVoteNext,
    StageTwoSubstituteVotePrevious,
    StageTwoSubstituteVoteNext,
};

static const std::map<LabTestCase, std::string> LabTestCaseNames =
{
        {LabTestCase::Normal, "Normal"},
        {LabTestCase::AbstainVoting, "AbstainVoting"},
        {LabTestCase::VoteAgain, "VoteAgain"},
        {LabTestCase::UnregisteredVoter, "UnregisteredVoter"},
        {LabTestCase::VoteInsteadOfAnotherVoter, "VoteInsteadOfAnotherVoter"},
        {LabTestCase::InvalidCandidate, "InvalidCandidate"},
        {LabTestCase::StageOneDropVote, "StageOneDropVote"},
        {LabTestCase::StageTwoDropVote, "StageTwoDropVote"},
        {LabTestCase::StageOneDuplicateVote, "StageOneDuplicateVote"},
        {LabTestCase::StageTwoDuplicateVote, "StageTwoDuplicateVote"},
        {LabTestCase::StageOneSubstituteVotePrevious, "StageOneSubstituteVotePrevious"},
        {LabTestCase::StageOneSubstituteVoteNext, "StageOneSubstituteVoteNext"},
        {LabTestCase::StageTwoSubstituteVotePrevious, "StageTwoSubstituteVotePrevious"},
        {LabTestCase::StageTwoSubstituteVoteNext, "StageTwoSubstituteVoteNext"},
};

extern LabTestCase GTestCase;

#endif //LAB4_LABTESTCASE_H
