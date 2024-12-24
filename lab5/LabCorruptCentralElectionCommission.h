#ifndef LAB5_LABCORRUPTCENTRALELECTIONCOMMISSION_H
#define LAB5_LABCORRUPTCENTRALELECTIONCOMMISSION_H

#include <vector>

class LabCentralElectionCommission;
class LabElectionCommission;

class LabCorruptCentralElectionCommission final
{
public:
    static void VoteInsteadOfAnotherVoter(LabCentralElectionCommission& CEC, std::vector<LabElectionCommission>& ECs);
    static void DropVote(LabCentralElectionCommission& CEC);
    static void DuplicateVote(LabCentralElectionCommission& CEC);
    static void SubstituteVote(LabCentralElectionCommission& CEC);
};

#endif //LAB5_LABCORRUPTCENTRALELECTIONCOMMISSION_H
