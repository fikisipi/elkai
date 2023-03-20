#include "LKH.h"
#include "Segment.h"

GainType Penalty_ACVRP()
{
    static Node *StartRoute = 0;
    Node *N, *NextN,*CurrentRoute;
    GainType DemandSum, DistanceSum, P = 0;
    int Forward = SUCC(Depot)->Id != Depot->Id + DimensionSaved;

    if (!StartRoute)
        StartRoute = Depot;
    if (StartRoute->Id > DimensionSaved)
        StartRoute -= DimensionSaved;
    N = StartRoute;
    do {
        int Size = 0;
        CurrentRoute = N;
        DemandSum = 0;
        do {
            if (N->Id <= Dim && N != Depot) {
                if ((DemandSum += N->Demand) > Capacity)
                    P += DemandSum - Capacity;
                if (P > CurrentPenalty ||
                    (P == CurrentPenalty && CurrentGain <= 0)) {
                    StartRoute = CurrentRoute;
                    return CurrentPenalty + (CurrentGain > 0);
                }
                Size++;
            }
            N = Forward ? SUCC(N) : PREDD(N);
        } while (N->DepotId == 0);
        if (MTSPMinSize >= 1 && Size < MTSPMinSize)
            P += MTSPMinSize - Size;
        if (Size > MTSPMaxSize)
            P += Size - MTSPMaxSize;
        if (DistanceLimit != DBL_MAX) {
            DistanceSum = 0;
            N = CurrentRoute;
            do {
                NextN =  Forward ? SUCC(N) : PREDD(N);
                DistanceSum += (C(N, NextN) - N->Pi - NextN->Pi) /
                    Precision;
                DistanceSum += N->ServiceTime;
                N = Forward ? SUCC(NextN) : PREDD(NextN);
            } while (N ->DepotId == 0);
            if (DistanceSum > DistanceLimit &&
                ((P += (DistanceSum - DistanceLimit)) > CurrentPenalty ||
                 (P == CurrentPenalty && CurrentGain <= 0))) {
                StartRoute = CurrentRoute;
                return CurrentPenalty + (CurrentGain > 0);
            }
        }
    } while (N != StartRoute);
    return P;
}
