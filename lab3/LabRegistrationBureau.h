#ifndef LAB3_LABREGISTRATIONBUREAU_H
#define LAB3_LABREGISTRATIONBUREAU_H

#include "LabBallot.h"
#include "LabCandidate.h"
#include "LabPublicVoter.h"
#include "LabRBVoter.h"

#include <vector>

class LabElectionCommission;
class LabLocalVoter;

class LabRegistrationBureau final
{
public:
    explicit LabRegistrationBureau(const size_t VoterNum);

    void Register(LabLocalVoter& LocalVoter);

    LabElectionCommission CreateElectionCommission(const size_t CandidateNum) const;

    void GetResults(LabElectionCommission& EC,
                    std::vector<LabCandidate>& Candidates,
                    std::vector<LabBallot>& Ballots,
                    std::vector<LabPublicVoter>& PublicVoters) const;

private:
    std::vector<LabPublicVoter> GetPublicVoters() const;

    std::vector<LabRBVoter> Voters;

    friend class LabCorruptRegistrationBureau;
};

#endif //LAB3_LABREGISTRATIONBUREAU_H
