#ifndef LAB5_LABCORRUPTELECTIONCOMMISSION_H
#define LAB5_LABCORRUPTELECTIONCOMMISSION_H

#include "LabCrypto.h"

#include <vector>

class LabElectionCommission;

class LabCorruptElectionCommission final
{
public:
    static void VoteInsteadOfAnotherVoter(LabElectionCommission& EC, const LabCrypto::LabPublicKeyRSA& EncryptKey, const size_t ECNum);
    static void VoteInsteadOfAnotherVoterAll(std::vector<LabElectionCommission>& ECs, const LabCrypto::LabPublicKeyRSA& EncryptKey);
    static void DropVote(LabElectionCommission& EC);
    static void DropVoteAll(std::vector<LabElectionCommission>& ECs);
    static void DuplicateVote(LabElectionCommission& EC);
    static void DuplicateVoteAll(std::vector<LabElectionCommission>& ECs);
    static void SubstituteVote(LabElectionCommission& EC, const LabCrypto::LabPublicKeyRSA& EncryptKey, const size_t ECNum);
    static void SubstituteVoteAll(std::vector<LabElectionCommission>& ECs, const LabCrypto::LabPublicKeyRSA& EncryptKey);
};

#endif //LAB5_LABCORRUPTELECTIONCOMMISSION_H
