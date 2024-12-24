#ifndef LAB6_LABCORRUPTELECTIONCOMMISSION_H
#define LAB6_LABCORRUPTELECTIONCOMMISSION_H

class LabElectionCommission;

class LabCorruptElectionCommission final
{
public:
    static void VoteInsteadOfAnotherVoter(LabElectionCommission& EC);
    static void SubstituteVote(LabElectionCommission& EC);
    static void RevealVoter(LabElectionCommission& EC);
};

#endif //LAB6_LABCORRUPTELECTIONCOMMISSION_H
