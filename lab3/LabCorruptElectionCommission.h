#ifndef LAB3_LABCORRUPTELECTIONCOMMISSION_H
#define LAB3_LABCORRUPTELECTIONCOMMISSION_H

class LabElectionCommission;
class LabRegistrationBureau;

class LabCorruptElectionCommission final
{
public:
    static void VoteInsteadOfVoter(LabElectionCommission& EC);
    static void ChangeVote(LabElectionCommission& EC);
    static void RevealVoter(LabElectionCommission& EC);

    static void ChangeVoteWithRB(LabElectionCommission& EC, LabRegistrationBureau& RB);
};

#endif //LAB3_LABCORRUPTELECTIONCOMMISSION_H
