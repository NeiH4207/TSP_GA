#include<iostream>
#include<cmath>
#include<algorithm>
#include<vector>
#include<time.h>
#include<string>
#include<fstream>
#include<iomanip>
#include<cassert>

using namespace std;

const int SIZE_OF_POPUTATION = 300;

const double pr_mutate = 0.05;

const int NUM_OF_GEN = 500;

const double coeff_remain = 0.7;

int num_of_points;

struct point {
  double x, y;

  point() {};

  point(double _x, double _y) {
    x = _x;
    y = _y;
  }

};

vector<point> points;

vector<int> tour;

double get_distance(int i, int j){
  return sqrt((points[i].x - points[j].x) * (points[i].x - points[j].x) 
    + (points[i].y - points[j].y) * (points[i].y - points[j].y));
}

void store_solution(string s = "output.txt"){
  fstream file;
  file.open("output.txt");
  file << num_of_points << '\n';
  for(int x: tour){
    file << setprecision(5) << points[x].x << ' ' << points[x].y << '\n';
  }
  file.close();
}

struct individual {

  int size;
  
  vector<int> chromosomes;

  double fitness;

  individual() {}

  individual(int num_of_points){
    size = num_of_points;
    chromosomes.resize(size);
    for(int i = 0; i < size; i++){
      chromosomes[i] = i;
    }
    for(int i = 0; i < size; i++){
      int pos1 = rand() % size;
      int pos2 = rand() % size;
      swap(chromosomes[pos1], chromosomes[pos2]);
    }
    fitness = get_fitness();
  }

  bool check_chromosomes(){
    vector<int> have(size);
    int cnt = 0;
    for(int x: chromosomes) {
      if(!have[x]) {
        have[x] = 1;
        cnt++;
      }
    }
    return cnt == size;
  }

  double get_fitness() {

    double length_tour = 0;
    for(int i = 0; i < size - 1; i++){
      length_tour += get_distance(chromosomes[i], chromosomes[i + 1]);
    }
    length_tour += get_distance(0, size - 1);
    return length_tour;
  }

  void mutation() {

    double rd_pr = (rand() % 1000) / 1000.0;

    if(rd_pr < pr_mutate){
      int left = rand() % size;
      int right = rand() % size;
      if(left > right) swap(left, right);
      int type_mutate = rand() % 2;

      if(type_mutate == 1){
        reverse(chromosomes.begin() + left, chromosomes.begin() + right);
      } else{
        swap(chromosomes[left], chromosomes[right]);
      }

    }
  }

  bool operator < (individual &other) {
    return fitness < other.fitness;
  }

};

struct population {

  individual new_population[SIZE_OF_POPUTATION];

  individual old_population[SIZE_OF_POPUTATION];

  double best_fitness = 2e15;

  population() {
    for (int i = 0; i < SIZE_OF_POPUTATION; i++) {
      old_population[i] = individual(num_of_points);
      if(best_fitness > old_population[i].fitness){
        best_fitness = old_population[i].fitness;
        tour = old_population[i].chromosomes;
      }
    }

    sort(old_population, old_population + SIZE_OF_POPUTATION);
  }

  pair<individual, individual> crossover(individual parent1, individual parent2) {
    individual child1, child2;
    child1 = parent1;
    child2 = parent2;

    int cut_point = rand() % num_of_points;
    vector<bool> have1(num_of_points, 0);
    vector<bool> have2(num_of_points, 0);

    for(int i = 0; i < cut_point; i++){
      child1.chromosomes[i] = parent2.chromosomes[num_of_points - i - 1];
      have1[child1.chromosomes[i]] = 1;
      child2.chromosomes[num_of_points - i - 1] = parent1.chromosomes[i];
      have2[child2.chromosomes[num_of_points - i - 1]] = 1;
    }
    int pos1 = cut_point;
    int pos2 = 0;

    for(int i = 0; i < num_of_points; i++){
      if(!have1[parent1.chromosomes[i]]){
        child1.chromosomes[pos1++] = parent1.chromosomes[i];
      }
      if(!have2[parent2.chromosomes[i]]){
        child2.chromosomes[pos2++] = parent2.chromosomes[i];
      }
    }

    child1.mutation();
    child2.mutation();

    child1.fitness = child1.get_fitness();
    child2.fitness = child2.get_fitness();

    return {child1, child2};
  }


  void Selection() {
    sort(new_population, new_population + SIZE_OF_POPUTATION);

    for (int i = SIZE_OF_POPUTATION * coeff_remain; i < SIZE_OF_POPUTATION; i++) {
      old_population[i] = new_population[i - int(SIZE_OF_POPUTATION * coeff_remain)];
      if(best_fitness > old_population[i].fitness){
        best_fitness = old_population[i].fitness;
        tour = old_population[i].chromosomes;
      }
    }
    sort(old_population, old_population + SIZE_OF_POPUTATION);
  }

  void Repopulation() {
    for (int i = 0; i < SIZE_OF_POPUTATION; i += 2) {
      // chon ngau nhien 2 cha me
      int position1 = rand() % SIZE_OF_POPUTATION;
      int position2 = rand() % SIZE_OF_POPUTATION;

      while (position2 == position1) {
        position1 = rand() % SIZE_OF_POPUTATION;
        position2 = rand() % SIZE_OF_POPUTATION;
      }
      auto child = crossover(old_population[position1], old_population[position2]);

      new_population[i] = child.first;
      new_population[i + 1] = child.second;
    }

    Selection();
  }

  void Evolution() {
    for (int gen = 0; gen < NUM_OF_GEN; gen++) {
      cout << "Best of solution of gen " << gen << " :";
      cout << fixed << setprecision(7) << best_fitness << '\n';
      Repopulation();
    }
  }
};

void start() {
  fstream file;
  file.open("input3.txt");
  file >> num_of_points;
  double x, y;
  points.resize(num_of_points);

  for (int i = 0; i < num_of_points; i++) {
    file >> x >> y;
    points[i] = {x, y};
  }
}

int main() {
  start();
  srand(1);
  population GA_TSP;
  GA_TSP.Evolution();
  store_solution();
  cout << GA_TSP.best_fitness;
}
