#include <Python.h>
#include "LKH.h"
#include "Genetic.h"
#include "math.h"
#include "Heap.h"

// TODO:
// - Delete obsolete problem types
// - Make sure camel case is used everywhere
// - Use git submodule instead of a LKH copy

static void LoadWeightMatrix(int *_wArr);
void ParseTour(int *outN, int *outBuff, int *Tour)
{
    int i, j, n, Forwards;
    int bufIdx = 0;
    char *FullFileName;
    time_t Now;

    n = ProblemType != ATSP ? Dimension : Dimension / 2;

    for (i = 1; i < n && Tour[i] != 1; i++)
        ;
    Forwards = ProblemType == ATSP ||
               Tour[i < n ? i + 1 : 1] < Tour[i > 1 ? i - 1 : Dimension];
    for (j = 1; j <= n; j++)
    {
        outBuff[bufIdx] = Tour[i] - 1;
        bufIdx++;
        if (Forwards)
        {
            if (++i > n)
                i = 1;
        }
        else if (--i < 1)
            i = n;
    }
    *outN = bufIdx;
}
static void CreateNodes()
{
    Node *Prev = 0, *N = 0;
    int i;

    if (Dimension <= 0)
        eprintf("DIMENSION is not positive (or not specified)");
    if (ProblemType == ATSP)
        Dimension *= 2;
    else if (ProblemType == HPP)
    {
        Dimension++;
        if (Dimension > MaxMatrixDimension)
            eprintf("Dimension too large in HPP problem");
    }
    assert(NodeSet = (Node *)calloc(Dimension + 1, sizeof(Node)));
    for (i = 1; i <= Dimension; i++, Prev = N)
    {
        N = &NodeSet[i];
        if (i == 1)
            FirstNode = N;
        else
            Link(Prev, N);
        N->Id = i;
        if (MergeTourFiles >= 1)
            assert(N->MergeSuc =
                       (Node **)calloc(MergeTourFiles, sizeof(Node *)));
    }
    Link(N, FirstNode);
}

static int FixEdge(Node *Na, Node *Nb)
{
    if (!Na->FixedTo1 || Na->FixedTo1 == Nb)
        Na->FixedTo1 = Nb;
    else if (!Na->FixedTo2 || Na->FixedTo2 == Nb)
        Na->FixedTo2 = Nb;
    else
        return 0;
    if (!Nb->FixedTo1 || Nb->FixedTo1 == Na)
        Nb->FixedTo1 = Na;
    else if (!Nb->FixedTo2 || Nb->FixedTo1 == Na)
        Nb->FixedTo2 = Na;
    else
        return 0;
    return 1;
}

void ReadParameters()
{
    char *Line, *Keyword, *Token, *Name;
    unsigned int i;

    ProblemFileName = PiFileName = InputTourFileName =
        OutputTourFileName = TourFileName = 0;
    CandidateFiles = MergeTourFiles = 0;
    AscentCandidates = 50;
    BackboneTrials = 0;
    Backtracking = 0;
    CandidateSetSymmetric = 0;
    CandidateSetType = ALPHA;
    Crossover = ERXT;
    DelaunayPartitioning = 0;
    DelaunayPure = 0;
    Excess = -1;
    ExtraCandidates = 0;
    ExtraCandidateSetSymmetric = 0;
    ExtraCandidateSetType = QUADRANT;
    Gain23Used = 1;
    GainCriterionUsed = 1;
    GridSize = 1000000.0;
    InitialPeriod = -1;
    InitialStepSize = 0;
    InitialTourAlgorithm = WALK;
    InitialTourFraction = 1.0;
    KarpPartitioning = 0;
    KCenterPartitioning = 0;
    KMeansPartitioning = 0;
    Kicks = 1;
    KickType = 0;
    MaxBreadth = INT_MAX;
    MaxCandidates = 5;
    MaxPopulationSize = 0;
    MaxSwaps = -1;
    MaxTrials = -1;
    MoorePartitioning = 0;
    MoveType = 5;
    NonsequentialMoveType = -1;
    Optimum = MINUS_INFINITY;
    PatchingA = 1;
    PatchingC = 0;
    PatchingAExtended = 0;
    PatchingARestricted = 0;
    PatchingCExtended = 0;
    PatchingCRestricted = 0;
    Precision = 100;
    POPMUSIC_InitialTour = 0;
    POPMUSIC_MaxNeighbors = 5;
    POPMUSIC_SampleSize = 10;
    POPMUSIC_Solutions = 50;
    POPMUSIC_Trials = 1;
    Recombination = IPT;
    RestrictedSearch = 1;
    RohePartitioning = 0;
    Runs = 0;
    Seed = 1;
    SierpinskiPartitioning = 0;
    StopAtOptimum = 1;
    Subgradient = 1;
    SubproblemBorders = 0;
    SubproblemsCompressed = 0;
    SubproblemSize = 0;
    SubsequentMoveType = 0;
    SubsequentPatching = 1;
    TimeLimit = DBL_MAX;
    TraceLevel = 1;
    LastLine = 0;

    // Go here
    Runs = 5;
    Precision = 50;
    TourFileName = "aaa2.txt";
}

void _elk_ReadProblem(int *myMatrix, int matrixLen)
{
    int i, K;
    char *Line, *Keyword;

    // FreeStructures();
    FirstNode = 0;
    WeightType = WeightFormat = ProblemType = -1;
    CoordType = NO_COORDS;
    Type = EdgeWeightType = EdgeWeightFormat = 0;
    EdgeDataFormat = NodeCoordType = DisplayDataType = 0;
    Distance = 0;
    GridSize = 1000000.0;
    C = 0;
    c = 0;

    // Go here
    ProblemType = ATSP;
    Dimension = (int)sqrt(matrixLen);
    DimensionSaved = Dimension;
    WeightType = EXPLICIT;
    Distance = Distance_EXPLICIT;
    WeightFormat = FULL_MATRIX;
    LoadWeightMatrix(myMatrix);

    Swaps = 0;

    /* Adjust parameters */
    if (Seed == 0)
        Seed = (unsigned)time(0);
    if (Precision == 0)
        Precision = 100;
    if (InitialStepSize == 0)
        InitialStepSize = 1;
    if (MaxSwaps < 0)
        MaxSwaps = Dimension;
    if (KickType > Dimension / 2)
        KickType = Dimension / 2;
    if (Runs == 0)
        Runs = 10;
    if (MaxCandidates > Dimension - 1)
        MaxCandidates = Dimension - 1;
    if (ExtraCandidates > Dimension - 1)
        ExtraCandidates = Dimension - 1;
    if (SubproblemSize >= Dimension)
        SubproblemSize = Dimension;
    else if (SubproblemSize == 0)
    {
        if (AscentCandidates > Dimension - 1)
            AscentCandidates = Dimension - 1;
        if (InitialPeriod < 0)
        {
            InitialPeriod = Dimension / 2;
            if (InitialPeriod < 100)
                InitialPeriod = 100;
        }
        if (Excess < 0)
            Excess = 1.0 / Dimension;
        if (MaxTrials == -1)
            MaxTrials = Dimension;
        MakeHeap(Dimension);
    }
    if (POPMUSIC_MaxNeighbors > Dimension - 1)
        POPMUSIC_MaxNeighbors = Dimension - 1;
    if (POPMUSIC_SampleSize > Dimension)
        POPMUSIC_SampleSize = Dimension;
    if (CostMatrix == 0 && Dimension <= MaxMatrixDimension &&
        Distance != 0 && Distance != Distance_1 && Distance != Distance_LARGE &&
        Distance != Distance_ATSP && Distance != Distance_SPECIAL)
    {
        Node *Ni, *Nj;
        assert(CostMatrix =
                   (int *)calloc((size_t)Dimension * (Dimension - 1) / 2,
                                 sizeof(int)));
        Ni = FirstNode->Suc;
        do
        {
            Ni->C =
                &CostMatrix[(size_t)(Ni->Id - 1) * (Ni->Id - 2) / 2] - 1;
            if (ProblemType != HPP || Ni->Id < Dimension)
                for (Nj = FirstNode; Nj != Ni; Nj = Nj->Suc)
                    Ni->C[Nj->Id] = Fixed(Ni, Nj) ? 0 : Distance(Ni, Nj);
            else
                for (Nj = FirstNode; Nj != Ni; Nj = Nj->Suc)
                    Ni->C[Nj->Id] = 0;
        } while ((Ni = Ni->Suc) != FirstNode);
        WeightType = EXPLICIT;
        c = 0;
    }
    if (Precision > 1 && (WeightType == EXPLICIT || ProblemType == ATSP))
    {
        int j, n = ProblemType == ATSP ? Dimension / 2 : Dimension;
        for (i = 2; i <= n; i++)
        {
            Node *N = &NodeSet[i];
            for (j = 1; j < i; j++)
                if (N->C[j] * Precision / Precision != N->C[j])
                    eprintf("PRECISION (= %d) is too large", Precision);
        }
    }
    C = WeightType == EXPLICIT ? C_EXPLICIT : C_FUNCTION;
    D = WeightType == EXPLICIT ? D_EXPLICIT : D_FUNCTION;
    if (SubsequentMoveType == 0)
        SubsequentMoveType = MoveType;
    K = MoveType >= SubsequentMoveType || !SubsequentPatching ? MoveType : SubsequentMoveType;
    if (PatchingC > K)
        PatchingC = K;
    if (PatchingA > 1 && PatchingA >= PatchingC)
        PatchingA = PatchingC > 2 ? PatchingC - 1 : 1;
    if (NonsequentialMoveType == -1 ||
        NonsequentialMoveType > K + PatchingC + PatchingA - 1)
        NonsequentialMoveType = K + PatchingC + PatchingA - 1;
    if (PatchingC >= 1)
    {
        BestMove = BestSubsequentMove = BestKOptMove;
        if (!SubsequentPatching && SubsequentMoveType <= 5)
        {
            MoveFunction BestOptMove[] =
                {0, 0, Best2OptMove, Best3OptMove,
                 Best4OptMove, Best5OptMove};
            BestSubsequentMove = BestOptMove[SubsequentMoveType];
        }
    }
    else
    {
        MoveFunction BestOptMove[] = {0, 0, Best2OptMove, Best3OptMove,
                                      Best4OptMove, Best5OptMove};
        BestMove = MoveType <= 5 ? BestOptMove[MoveType] : BestKOptMove;
        BestSubsequentMove = SubsequentMoveType <= 5 ? BestOptMove[SubsequentMoveType] : BestKOptMove;
    }
    if (ProblemType == HCP || ProblemType == HPP)
        MaxCandidates = 0;
    if (TraceLevel >= 1)
    {
    }
    if (InitialTourFileName)
        printf("initial tour name\n");
    if (InputTourFileName)
        printf("input tour name\n");
    if (SubproblemTourFileName && SubproblemSize > 0)
        printf("subproblem tour name\n");
    if (MergeTourFiles >= 1)
    {
        printf("merge files");
    }
    LastLine = 0;
}

static void LoadWeightMatrix(int *_wArr)
{
    int _wArrIndex = 0;
    Node *Ni, *Nj;
    int i, j, n, W;

    CreateNodes();
    if (ProblemType != ATSP)
    {
        assert(CostMatrix =
                   (int *)calloc((size_t)Dimension * (Dimension - 1) / 2,
                                 sizeof(int)));
        Ni = FirstNode->Suc;
        do
        {
            Ni->C =
                &CostMatrix[(size_t)(Ni->Id - 1) * (Ni->Id - 2) / 2] - 1;
        } while ((Ni = Ni->Suc) != FirstNode);
    }
    else
    {
        n = Dimension / 2;
        assert(CostMatrix = (int *)calloc((size_t)n * n, sizeof(int)));
        for (Ni = FirstNode; Ni->Id <= n; Ni = Ni->Suc)
            Ni->C = &CostMatrix[(size_t)(Ni->Id - 1) * n] - 1;
    }
    if (ProblemType == HPP)
        Dimension--;
    switch (1)
    {
    case 1:
        if (ProblemType == ATSP)
        {
            n = Dimension / 2;
            for (i = 1; i <= n; i++)
            {
                Ni = &NodeSet[i];
                for (j = 1; j <= n; j++)
                {
                    W = _wArr[_wArrIndex];
                    _wArrIndex++;
                    if (W > INT_MAX / 2 / Precision)
                        W = INT_MAX / 2 / Precision;
                    Ni->C[j] = W;
                    if (i != j && W > M)
                        M = W;
                }
                Nj = &NodeSet[i + n];
                FixEdge(Ni, Nj);
            }
            Distance = Distance_ATSP;
            WeightType = -1;
        }
        else
            for (i = 1, Ni = FirstNode; i <= Dimension; i++, Ni = Ni->Suc)
            {
                for (j = 1; j <= Dimension; j++)
                {
                    W = _wArr[_wArrIndex];
                    _wArrIndex++;
                    if (W > INT_MAX / 2 / Precision)
                        W = INT_MAX / 2 / Precision;
                    if (j < i)
                        Ni->C[j] = W;
                }
            }
        break;
    }
    if (ProblemType == HPP)
        Dimension++;
}

// These functions reset the global state that LKH uses.
// It makes it possible to re-run the solver without
// restarting the program.
// I'm sorry.

void _Reset1();
void _Reset2();
void _Reset3();
void _Reset4();
void _Reset5();
void _Reset6();
void _Reset7();
void _Reset8();

int m_calculate(int *matrixBuff, int matrixLen, int *tourBuff, int *tourN)
{
    _Reset1();
    _Reset2();
    _Reset3();
    _Reset4();
    _Reset5();
    _Reset6();
    _Reset7();
    _Reset8();

    GainType Cost, OldOptimum;
    double Time, LastTime = GetTime();
    ReadParameters();
    MaxMatrixDimension = 20000;
    MergeWithTour = Recombination == IPT ? MergeWithTourIPT : MergeWithTourGPX2;
    _elk_ReadProblem(matrixBuff, matrixLen);

    AllocateStructures();
    TraceLevel = -1;
    CandidateFiles = 0;
    PiFileName = 0;
    EdgeFiles = 0;
    CreateCandidateSet();
    InitializeStatistics();

    if (Norm == 0)
    {
        Optimum = BestCost = (GainType)LowerBound;
        UpdateStatistics(Optimum, GetTime() - LastTime);
        RecordBetterTour();
        RecordBestTour();
        ParseTour(tourN, tourBuff, BestTour);
        return 0;
    }
    else
    {
        BestCost = PLUS_INFINITY;
    }

    for (Run = 1; Run <= Runs; Run++)
    {
        LastTime = GetTime();
        Cost = FindTour(); /* using the Lin-Kernighan heuristic */
        if (MaxPopulationSize > 1)
        {
            /* Genetic algorithm */
            int i;
            for (i = 0; i < PopulationSize; i++)
            {
                GainType OldCost = Cost;
                Cost = MergeTourWithIndividual(i);
                if (TraceLevel >= 1 && Cost < OldCost)
                {
                    printff("  Merged with %d: Cost = " GainFormat, i + 1,
                            Cost);
                    if (Optimum != MINUS_INFINITY && Optimum != 0)
                        printff(", Gap = %0.4f%%",
                                100.0 * (Cost - Optimum) / Optimum);
                    printff("\n");
                }
            }
            if (!HasFitness(Cost))
            {
                if (PopulationSize < MaxPopulationSize)
                {
                    AddToPopulation(Cost);
                    if (TraceLevel >= 1)
                        PrintPopulation();
                }
                else if (Cost < Fitness[PopulationSize - 1])
                {
                    i = ReplacementIndividual(Cost);
                    ReplaceIndividualWithTour(i, Cost);
                    if (TraceLevel >= 1)
                        PrintPopulation();
                }
            }
        }
        else if (Run > 1)
            Cost = MergeTourWithBestTour();
        if (Cost < BestCost)
        {
            BestCost = Cost;
            RecordBetterTour();
            RecordBestTour();
        }
        OldOptimum = Optimum;
        if (Cost < Optimum)
        {
            if (FirstNode->InputSuc)
            {
                Node *N = FirstNode;
                while ((N = N->InputSuc = N->Suc) != FirstNode)
                    ;
            }
            Optimum = Cost;
            printff("*** New optimum = " GainFormat " ***\n\n", Optimum);
        }
        Time = fabs(GetTime() - LastTime);
        UpdateStatistics(Cost, Time);
        if (TraceLevel >= 1 && Cost != PLUS_INFINITY)
        {
            printff("Run %d: Cost = " GainFormat, Run, Cost);
            if (Optimum != MINUS_INFINITY && Optimum != 0)
                printff(", Gap = %0.4f%%",
                        100.0 * (Cost - Optimum) / Optimum);
            printff(", Time = %0.2f sec. %s\n\n", Time,
                    Cost < Optimum ? "<" : Cost == Optimum ? "=" : "");
        }
        if (StopAtOptimum && Cost == OldOptimum && MaxPopulationSize >= 1)
        {
            Runs = Run;
            break;
        }
        if (PopulationSize >= 2 &&
            (PopulationSize == MaxPopulationSize ||
             Run >= 2 * MaxPopulationSize) &&
            Run < Runs)
        {
            Node *N;
            int Parent1, Parent2;
            Parent1 = LinearSelection(PopulationSize, 1.25);
            do
                Parent2 = LinearSelection(PopulationSize, 1.25);
            while (Parent2 == Parent1);
            ApplyCrossover(Parent1, Parent2);
            N = FirstNode;
            do
            {
                if (ProblemType != HCP && ProblemType != HPP)
                {
                    int d = C(N, N->Suc);
                    AddCandidate(N, N->Suc, d, INT_MAX);
                    AddCandidate(N->Suc, N, d, INT_MAX);
                }
                N = N->InitialSuc = N->Suc;
            } while (N != FirstNode);
        }
        SRandom(++Seed);
    }
    ParseTour(tourN, tourBuff, BestTour);
    return Norm;
}

static PyObject *elk_solve(PyObject *self, PyObject *arg)
{
    int pyLen = PyObject_Length(arg);
    int pyLenSqrt = (int)sqrt(pyLen);
    if (pyLen < 4 || pyLenSqrt * pyLenSqrt != pyLen)
    {
        PyErr_SetString(PyExc_ValueError, "Argument should be a list with N^2 >= 4 elements.\n"
                                          "Example: [1, 1, 1, 1].");
        return 0;
    }
    int *matrixBuff = (int *)malloc(sizeof(int) * pyLen);
    int *tourBuff = (int *)malloc(sizeof(int) * pyLenSqrt);
    int tourN = 0;

    for (long i = 0; i < pyLen; i++)
    {
        PyObject *pyNumber = PyObject_GetItem(arg, PyLong_FromLong(i));
        long long justNumber = PyLong_AsLongLong(pyNumber);
        int justNumber_i = (int)justNumber;
        // TODO: don't lose precision
        matrixBuff[i] = justNumber_i;
    }
    int norm_result = m_calculate(matrixBuff, pyLen, tourBuff, &tourN);
    free(matrixBuff);
    PyObject *list = PyList_New(tourN);
    int i;
    for (i = 0; i < tourN; i++)
    {
        PyObject *tourElement = PyLong_FromLong((long)tourBuff[i]);
        PyList_SetItem(list, i, tourElement);
    }

    free(tourBuff);
    return list;
}

static char elk_docs[] =
    "solve(x): Solve a TSP problem.\n";

static PyMethodDef funcs[] = {
    {"solve", (PyCFunction)elk_solve,
     METH_O, elk_docs},
    {NULL}};

static struct PyModuleDef hello_module_def = {
    PyModuleDef_HEAD_INIT,
    "_elkai",
    "",
    -1,
    funcs};

PyMODINIT_FUNC PyInit__elkai(void)
{
    return PyModule_Create(&hello_module_def);
}