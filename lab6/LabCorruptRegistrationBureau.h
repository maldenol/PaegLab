#ifndef LAB6_LABCORRUPTREGISTRATIONBUREAU_H
#define LAB6_LABCORRUPTREGISTRATIONBUREAU_H

class LabElectionCommission;
class LabRegistrationBureau;

class LabCorruptRegistrationBureau final
{
public:
    static void VoteInsteadOfAnotherVoter(LabRegistrationBureau& RB, LabElectionCommission& EC);
    static void RevealVoter(LabRegistrationBureau& RB);
};

#endif //LAB6_LABCORRUPTREGISTRATIONBUREAU_H
