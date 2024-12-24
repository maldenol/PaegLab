#ifndef LAB3_LABCORRUPTREGISTRATIONBUREAU_H
#define LAB3_LABCORRUPTREGISTRATIONBUREAU_H

#include "LabRBVoter.h"

#include <vector>

class LabBallot;
class LabElectionCommission;
class LabRegistrationBureau;

class LabCorruptRegistrationBureau final
{
public:
    static void VoteInsteadOfVoter(const LabRegistrationBureau& RB, LabElectionCommission& EC);
    static void RevealVoter(const LabRegistrationBureau& RB, const LabBallot& Ballot);

    static const std::vector<LabRBVoter>& GetRBVoters(const LabRegistrationBureau& RB);
};

#endif //LAB3_LABCORRUPTREGISTRATIONBUREAU_H
