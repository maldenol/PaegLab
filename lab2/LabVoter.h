#ifndef LAB2_LABVOTER_H
#define LAB2_LABVOTER_H

struct LabVoter final
{
    unsigned VoterId = 0;

private:
    bool bHasSentMaskedBallotSetArray = false;

    friend class LabCentralElectionCommission;
    friend class LabCorruptCentralElectionCommission;
};

#endif //LAB2_LABVOTER_H
