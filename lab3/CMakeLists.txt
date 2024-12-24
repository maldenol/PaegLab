cmake_minimum_required(VERSION 3.26)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab3 main.cpp
        LabBallot.cpp
        LabBallot.h
        LabCandidate.h
        LabCorruptElectionCommission.cpp
        LabCorruptElectionCommission.h
        LabCorruptRegistrationBureau.cpp
        LabCorruptRegistrationBureau.h
        LabCrypto.cpp
        LabCrypto.h
        LabElectionCommission.cpp
        LabElectionCommission.h
        LabLocalVoter.h
        LabPublicVoter.h
        LabRBVoter.h
        LabRegistrationBureau.cpp
        LabRegistrationBureau.h
)
