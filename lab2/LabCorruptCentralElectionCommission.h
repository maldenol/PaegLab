#ifndef LAB2_LABCORRUPTCENTRALELECTIONCOMMISSION_H
#define LAB2_LABCORRUPTCENTRALELECTIONCOMMISSION_H

class LabCentralElectionCommission;
class LabMaskedBallotSetArray;

class LabCorruptCentralElectionCommission final
{
public:
    static void RevealVoterBallotIdDuringUnmasking(LabCentralElectionCommission& CEC, const unsigned VoterId, LabMaskedBallotSetArray MaskedBallotSetArray);
    static void ChangeVote(LabCentralElectionCommission& CEC, const unsigned BallotId);
    static void VoteInsteadOfVoter(LabCentralElectionCommission& CEC, const unsigned VoterId, const unsigned CandidateId);
};

#endif //LAB2_LABCORRUPTCENTRALELECTIONCOMMISSION_H
