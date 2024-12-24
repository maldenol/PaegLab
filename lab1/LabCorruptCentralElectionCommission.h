#ifndef LAB1_LABCORRUPTCENTRALELECTIONCOMMISSION_H
#define LAB1_LABCORRUPTCENTRALELECTIONCOMMISSION_H

class LabCentralElectionCommission;

class LabCorruptCentralElectionCommission final
{
public:
    static void RevealVoter(LabCentralElectionCommission& CentralElectionCommission, const unsigned VoterId);
    static void VoteInsteadOfVoter(LabCentralElectionCommission& CentralElectionCommission, const unsigned VoterId, const unsigned CandidateId);
    static void ChangeVote(LabCentralElectionCommission& CentralElectionCommission, const unsigned VoterId, const unsigned CandidateId);
};

#endif //LAB1_LABCORRUPTCENTRALELECTIONCOMMISSION_H
