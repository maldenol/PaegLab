cmake_minimum_required(VERSION 3.26)
project(lab6)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab6 main.cpp
        LabBallot.cpp
        LabBallot.h
        LabBigInt.cpp
        LabBigInt.h
        LabCandidate.h
        LabCorruptElectionCommission.cpp
        LabCorruptElectionCommission.h
        LabCorruptRegistrationBureau.cpp
        LabCorruptRegistrationBureau.h
        LabCrypto.cpp
        LabCrypto.h
        LabECToken.h
        LabElectionCommission.cpp
        LabElectionCommission.h
        LabRegistrationBureau.cpp
        LabRegistrationBureau.h
        LabTestCase.h
        LabVoter.h
        LabVoter.cpp
        LabVoterToken.h
)
