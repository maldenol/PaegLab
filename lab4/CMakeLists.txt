cmake_minimum_required(VERSION 3.26)
project(lab4)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab4 main.cpp
        LabBallot.cpp
        LabBallot.h
        LabBigInt.cpp
        LabBigInt.h
        LabCandidate.h
        LabCrypto.cpp
        LabCrypto.h
        LabTestCase.h
        LabVoter.h
        LabVoter.cpp
)
