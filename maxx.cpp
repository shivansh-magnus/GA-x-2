#include <bits/stdc++.h>
#include <chrono>
#include <thread>
using namespace std;

// Defining Global variables
int PopulationSize = 4;
int maxGeneration = 20;

// Solution genotype
string DecimalToBinary(int num)
{
    string str;
    while (num)
    {
        if (num & 1)
            str += '1';
        else // 0
            str += '0';
        num >>= 1;
    }

    string str2;
    for (int i = str.size() - 1; i >= 0; i--)
    {
        str2 += str[i];
    }

    return str2;
}

// Solution phenotype
int binaryToDecimal(string n)
{
    string num = n;
    int dec_value = 0;

    int base = 1;

    int len = num.length();
    for (int i = len - 1; i >= 0; i--)
    {
        if (num[i] == '1')
            dec_value += base;
        base = base * 2;
    }

    return dec_value;
}

// Defining fitness function
int fitness(string gene)
{
    return pow(binaryToDecimal(gene), 2);
}

// Defining structure of solution and its fitness function
struct Solution
{
    string gene;
    int fit;
    float probabilitySelection;
    float expectedCount;
};

// Generating a random binary digit
int findRandom()
{
    int num = ((int)rand() % 2);
    srand(time(NULL));
    return num;
}
// Generating a random binary string using above function
string generateBinaryString()
{
    int N = 5;

    string S = "";

    for (int i = 0; i < N; i++)
    {
        int x = findRandom();
        S += to_string(x);
        this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    return S;
}

// Best Fitness Calculation among the given population
int bestFitnessGeneration(Solution population[], int PopulationSize)
{
    int bestfitG = 0;
    for (int i = 0; i < PopulationSize; i++)
    {
        bestfitG = max(bestfitG, population[i].fit);
    }
    return bestfitG;
}

// Total Fitness Calculation among the given population
int totalFitness(Solution population[], int PopulationSize)
{
    int totalfitG = 0;
    for (int i = 0; i < PopulationSize; i++)
    {
        totalfitG += population[i].fit;
    }
    return totalfitG;
}

// Average Fitness Calculation among the given population
float avgFitness(Solution population[], int PopulationSize)
{
    float avgfitG = totalFitness(population, PopulationSize) / PopulationSize;
    return avgfitG;
}

// Selection
int Selection(Solution population[], int PopulationSize)
{
    double rndNumber = rand() % PopulationSize;
    double offset = rndNumber;
    int pick = 0;

    for (int i = 0; i < PopulationSize; i++)
    {
        if (offset >= population[i].probabilitySelection)
        {
            pick = i;
        }
    }

    return pick;
}

// CrossOver
void crossover(Solution population[], int p1, int p2)
{
    int crosspoint = (rand() % 4) + 1;
    this_thread::sleep_for(std::chrono::milliseconds(300));

    Solution temp;
    temp.gene = population[p1].gene;
    for (int i = crosspoint; i < 5; i++)
    {
        population[p1].gene[i] = population[p2].gene[i];
    }

    for (int i = crosspoint; i < 5; i++)
    {
        population[p2].gene[i] = temp.gene[i];
    }
}

// Mutation
void mutation(Solution p1)
{
    int mutatepoint = (rand() % 4) + 1;
    int x = ((int)rand() % 10) + 1;
    if (x > 8)
    {
        if (p1.gene[mutatepoint] == '1')
        {
            p1.gene[mutatepoint] = '0';
        }
        else
        {
            p1.gene[mutatepoint] = '1';
        }
    }
}

// Worst individual
int worstIndividual(Solution population[], int PopulationSize)
{
    int worstfitG = 1000;
    int idx = -1;
    for (int i = 0; i < PopulationSize; i++)
    {
        if (population[i].fit < worstfitG)
        {
            idx = i;
            worstfitG = population[i].fit;
        }
    }
    return idx;
}

// Best individual
int bestIndividual(Solution population[], int PopulationSize)
{
    int bestfitG = -1;
    int idx = -1;
    for (int i = 0; i < PopulationSize; i++)
    {
        if (population[i].fit > bestfitG)
        {
            idx = i;
            bestfitG = population[i].fit;
        }
    }
    return idx;
}

int main()
{

    // Generating initial population
    Solution population[PopulationSize];
    for (int i = 0; i < PopulationSize; i++)
    {
        population[i].gene = generateBinaryString();
        population[i].fit = fitness(population[i].gene);
    }
    for (int i = 0; i < PopulationSize; i++)
    {
        population[i].probabilitySelection = population[i].fit / totalFitness(population, PopulationSize);
        population[i].expectedCount = population[i].fit / avgFitness(population, PopulationSize);
    }

    Solution genePool[PopulationSize];
    for (int i = 0; i < PopulationSize; i++)
    {
        genePool[i] = population[i];
    }

    while (maxGeneration--)
    {
        int bestFitnessAll = bestFitnessGeneration(genePool, PopulationSize);
        Solution bestIndi = genePool[bestIndividual(genePool, PopulationSize)];

        int selected[4] = {0, 1, 1, 3};

        for (int i = 0; i < PopulationSize; i += 2)
        {
            int p1 = selected[i];
            int p2 = selected[i + 1];
            int crosspoint = (rand() % 4) + 1;

            Solution temp;
            temp.gene = genePool[p1].gene;

            genePool[p1].gene.replace(crosspoint, 1, population[p2].gene, crosspoint, (genePool[p1].gene.size() - crosspoint));
            genePool[p2].gene.replace(crosspoint, 1, temp.gene, crosspoint, (temp.gene.size() - crosspoint));

            genePool[p1].gene = genePool[p1].gene.substr(0, 5);
            genePool[p2].gene = genePool[p2].gene.substr(0, 5);
        }

        for (int i = 0; i < PopulationSize; i += 2)
        {
            Solution p1 = genePool[i];
            int mutatepoint = (rand() % 4) + 1;
            int x = ((int)rand() % 10) + 1;
            if (x > 8)
            {
                if (p1.gene[mutatepoint] == '1')
                {
                    p1.gene[mutatepoint] = '0';
                }
                else
                {
                    p1.gene[mutatepoint] = '1';
                }
            }
        }

        if (bestFitnessAll > genePool[worstIndividual(genePool, PopulationSize)].fit)
        {
            genePool[worstIndividual(genePool, PopulationSize)].gene = bestIndi.gene;
        }

        for (int i = 0; i < PopulationSize; i++)
        {
            genePool[i].fit = fitness(genePool[i].gene);
        }

        for (int i = 0; i < PopulationSize; i++)
        {
            genePool[i].probabilitySelection = genePool[i].fit / totalFitness(genePool, PopulationSize);
            genePool[i].expectedCount = genePool[i].fit / avgFitness(genePool, PopulationSize);
        }

        cout << "Generation " << (maxGeneration - 20) << " ---- " << endl;
        for (int i = 0; i < 4; i++)
        {
            cout << genePool[i].gene << "  -  ";
            cout << fitness(genePool[i].gene) << endl;
        }
        cout << "Best Individual in this generation is - " << bestIndi.gene << endl;
        cout << "Avg fitness of the generation is- " << avgFitness(genePool, PopulationSize);

        cout << endl;
        cout << endl;
    }

    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;

    Solution finalSol = genePool[bestIndividual(genePool, PopulationSize)];
    cout << "Gene of best sol is: " << finalSol.gene << endl;
    cout << "Phenotype of this sol is: " << binaryToDecimal(finalSol.gene) << endl;
    cout << "Fitness of this sol is: " << finalSol.fit << endl;

    return 0;
}