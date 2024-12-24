#ifndef LAB2_LABLOCALVOTER_H
#define LAB2_LABLOCALVOTER_H

#include "LabBallot.h"

struct LabLocalVoter final
{
    unsigned VoterId = 0;
    LabBallot SubmittedBallot = LabBallot{0};
};

#endif //LAB2_LABLOCALVOTER_H
