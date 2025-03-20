#include <bits/stdc++.h>
using namespace std;

class AntColonyOptimizationTSP
{
    int numCities, numAnts, maxIterations;
    double alpha, beta, evaporationRate, Q;
    vector<vector<double>> distances;  // Distance matrix
    vector<vector<double>> pheromones; // Pheromone matrix
    mt19937 rng;                       // Mersenne Twister random number generator

public:
    AntColonyOptimizationTSP(int numCities, int numAnts, int maxIterations,
                             double alpha, double beta, double evaporationRate,
                             double Q, const vector<vector<double>> &distances)
        : numCities(numCities), numAnts(numAnts), maxIterations(maxIterations),
          alpha(alpha), beta(beta), evaporationRate(evaporationRate), Q(Q),
          distances(distances)
    {
        pheromones = vector<vector<double>>(numCities, vector<double>(numCities, 1.0));
        random_device rd;
        rng = mt19937(rd());
    }

    vector<int> solve()
    {
        vector<int> bestTour;
        double bestTourLength = numeric_limits<double>::max();

        for (int iter = 0; iter < maxIterations; iter++)
        {
            vector<vector<int>> allTours(numAnts, vector<int>(numCities));
            vector<double> allTourLengths(numAnts, 0.0);
            for (int ant = 0; ant < numAnts; ant++)
            {
                allTours[ant] = constructTour();
                allTourLengths[ant] = getTourLength(allTours[ant]);
                if (allTourLengths[ant] < bestTourLength)
                {
                    bestTourLength = allTourLengths[ant];
                    bestTour = allTours[ant];
                }
            }
            updatePheromones(allTours, allTourLengths);
        }
        cout << "Best Tour Length: " << bestTourLength << "\n";
        return bestTour;
    }

private:
    vector<int> constructTour()
    {
        vector<int> tour(numCities, 0);
        vector<bool> visited(numCities, false);

        // Choose a random starting city
        uniform_int_distribution<int> dist(0, numCities - 1);
        int currentCity = dist(rng);
        tour[0] = currentCity;
        visited[currentCity] = true;

        for (int i = 1; i < numCities; i++)
        {
            int nextCity = selectNextCity(currentCity, visited);
            tour[i] = nextCity;
            visited[nextCity] = true;
            currentCity = nextCity;
        }
        return tour;
    }

    int selectNextCity(int currentCity, const vector<bool> &visited)
    {
        vector<double> probabilities(numCities, 0.0);
        double sum = 0.0;
        for (int i = 0; i < numCities; i++)
        {
            if (!visited[i])
            {
                // Pheromone influence: pheromones[currentCity][i]^alpha
                // Visibility (heuristic) influence: (1/distances[currentCity][i])^beta
                probabilities[i] = pow(pheromones[currentCity][i], alpha) *
                                   pow(1.0 / distances[currentCity][i], beta);
                sum += probabilities[i];
            }
        }

        // Roulette wheel selection
        uniform_real_distribution<double> distReal(0.0, sum);
        double randValue = distReal(rng);
        double cumulative = 0.0;
        for (int i = 0; i < numCities; i++)
        {
            if (!visited[i])
            {
                cumulative += probabilities[i];
                if (cumulative >= randValue)
                    return i;
            }
        }
        throw runtime_error("No next city found!");
    }

    double getTourLength(const vector<int> &tour)
    {
        double length = 0.0;
        for (int i = 0; i < numCities - 1; i++)
        {
            length += distances[tour[i]][tour[i + 1]];
        }
        // Return to the start city
        length += distances[tour[numCities - 1]][tour[0]];
        return length;
    }

    void updatePheromones(const vector<vector<int>> &allTours, const vector<double> &allTourLengths)
    {
        // Evaporation step: reduce all pheromone trails
        for (int i = 0; i < numCities; i++)
        {
            for (int j = 0; j < numCities; j++)
            {
                pheromones[i][j] *= (1.0 - evaporationRate);
            }
        }
        // Deposit pheromones based on each ant's tour
        for (int ant = 0; ant < numAnts; ant++)
        {
            double contribution = Q / allTourLengths[ant];
            const vector<int> &tour = allTours[ant];
            for (int i = 0; i < numCities - 1; i++)
            {
                int city1 = tour[i];
                int city2 = tour[i + 1];
                pheromones[city1][city2] += contribution;
                pheromones[city2][city1] += contribution;
            }
            // Deposit for the edge from the last city back to the first city
            int last = tour[numCities - 1];
            int first = tour[0];
            pheromones[last][first] += contribution;
            pheromones[first][last] += contribution;
        }
    }
};

int main()
{
    // Example distance matrix
    vector<vector<double>> distances = {
        {0, 10, 12, 11},
        {10, 0, 8, 9},
        {12, 8, 0, 7},
        {11, 9, 7, 0}};
    int numCities = distances.size();
    int numAnts = 5;
    int maxIterations = 100;
    double alpha = 1.0;
    double beta = 2.0;
    double evaporationRate = 0.5;
    double Q = 100;

    AntColonyOptimizationTSP aco(numCities, numAnts, maxIterations, alpha, beta, evaporationRate, Q, distances);
    vector<int> bestTour = aco.solve();

    cout << "Best Tour: ";
    for (int city : bestTour)
    {
        cout << city << " ";
    }
    cout << "\n";

    return 0;
}
