#include <Python.h>
#include "LKH.h"

static void CreateNodes()
{
	Node *Prev = 0, *N = 0;
	int i;

	if (Dimension <= 0)
		eprintf("DIMENSION is not positive (or not specified)");
	if (ProblemType == ATSP)
		Dimension *= 2;
	else if (ProblemType == HPP) {
		Dimension++;
		if (Dimension > MaxMatrixDimension)
			eprintf("Dimension too large in HPP problem");
	}
	assert(NodeSet = (Node *)calloc(Dimension + 1, sizeof(Node)));
	for (i = 1; i <= Dimension; i++, Prev = N) {
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

static int FixEdge(Node * Na, Node * Nb)
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


static void LoadWeights(int *ww)
{
	Node *Ni, *Nj;
	int i, j, n, W;
	if (!FirstNode)
		CreateNodes();
	if (ProblemType != ATSP) {
		assert(CostMatrix =
			(int *)calloc((size_t)Dimension * (Dimension - 1) / 2,
				sizeof(int)));
		Ni = FirstNode->Suc;
		do {
			Ni->C =
				&CostMatrix[(size_t)(Ni->Id - 1) * (Ni->Id - 2) / 2] - 1;
		} while ((Ni = Ni->Suc) != FirstNode);
	}
	else {
		n = Dimension / 2;
		assert(CostMatrix = (int *)calloc((size_t)n * n, sizeof(int)));
		for (Ni = FirstNode; Ni->Id <= n; Ni = Ni->Suc)
			Ni->C = &CostMatrix[(size_t)(Ni->Id - 1) * n] - 1;
	}
	if (ProblemType == HPP)
		Dimension--;
	if (ProblemType == ATSP) {
		n = Dimension / 2;
		for (i = 1; i <= n; i++) {
			Ni = &NodeSet[i];
			for (j = 1; j <= n; j++) {
				/*
				if (!fscanint(ProblemFile, &W))
					eprintf("Missing weight in EDGE_WEIGHT_SECTION");
					*/
				W = *ww;
				ww++;
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
		for (i = 1, Ni = FirstNode; i <= Dimension; i++, Ni = Ni->Suc) {
			for (j = 1; j <= Dimension; j++) {
				/*
				if (!fscanint(ProblemFile, &W))
					eprintf("Missing weight in EDGE_WEIGHT_SECTION");
					*/
				W = *ww;
				ww++;
				if (W > INT_MAX / 2 / Precision)
					W = INT_MAX / 2 / Precision;
				if (j < i)
					Ni->C[j] = W;
			}
		}
	if (ProblemType == HPP)
		Dimension++;
}

int m_calculate()
{
	MaxMatrixDimension = 20000;
	MergeWithTour = Recombination == IPT ? MergeWithTourIPT :
		MergeWithTourGPX2;

	ProblemType = ATSP;
	Dimension = 3;
	DimensionSaved = Dimension;

	WeightType = EXPLICIT;
	Distance = Distance_EXPLICIT;
	int W[][3] = { {0, 2, 5}, {2, 0, 5}, {5, 2, 0} };
   printf("gucci\n");
	LoadWeights(W[0]);
   printf("gucci 2\n");
	AllocateStructures();
   printf("gucci 2\n");
	CreateCandidateSet();
   printf("gucci 2\n");
	InitializeStatistics();
   printf("gucci 2\n");
   return Norm;

}

static PyObject* elk_solve(PyObject* self)
{
   int norm_result = m_calculate();
   char out_buff[1000];
   sprintf(out_buff, "The norm is: %d", norm_result);
   return Py_BuildValue("s", out_buff);
}

static char elk_docs[] =
   "solve( ): Solve a TSP problem.\n";

static PyMethodDef funcs[] = {
   {"solve", (PyCFunction)elk_solve, 
   METH_NOARGS, elk_docs},
   {NULL}
};

static struct PyModuleDef hello_module_def = {
  PyModuleDef_HEAD_INIT,
  "_elkai",
  "",
  -1,
  funcs
};

PyMODINIT_FUNC PyInit__elkai(void)
{
  return PyModule_Create(&hello_module_def);
}