#include <drake/solvers/mathematical_program.h>
#include <drake/solvers/solve.h>

#include <iostream>

template <typename... Args>
void print(Args&&... value) {
  (std::cout << ... << value) << "\n---\n";
}

int main(int argc, char* argv[]) {
  /*
   * Solves a simple optimization problem
   *    min x(0)^2 + x(1)^2
   * subject to x(0) + x(1) = 1
   *        x(0) <= x(1)
   */
  // Set up the optimization problem.
  auto prog = drake::solvers::MathematicalProgram();
  auto x = prog.NewContinuousVariables(2);
  print("x = ", x);
  auto constraint1 = prog.AddConstraint(x[0] + x[1] == 1);
  print("constraint1: ", constraint1);
  auto constraint2 = prog.AddConstraint(x[0] <= x[1]);
  print("constraint2: ", constraint2);
  auto cost1 = prog.AddCost(pow(x[0], 2) + pow(x[1], 2));
  print("cost1: ", cost1);

  // Now solve the optimization problem.
  auto result = drake::solvers::Solve(prog);

  // print out the result.
  print("Success: ", result.is_success());
  // Print the solution to the decision variables.
  print("x* = ", result.GetSolution(x));
  // Print the optimal cost.
  print("optimal cost = ", result.get_optimal_cost());
  // Print the name of the solver that was called.
  print("solver is: ", result.get_solver_id().name());
  return 0;
}
