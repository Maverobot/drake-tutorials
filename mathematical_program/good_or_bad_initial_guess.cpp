#include <drake/solvers/ipopt_solver.h>
#include <drake/solvers/mathematical_program.h>

#include <iostream>

template <typename... Args>
void print(Args&&... value) {
  (std::cout << ... << value) << "\n---\n";
}

int main(int argc, char* argv[]) {
  // Set up the optimization problem.
  auto prog = drake::solvers::MathematicalProgram();
  auto x = prog.NewContinuousVariables(2);
  prog.AddConstraint(pow(x[0], 2) + pow(x[1], 2) == 100.);
  prog.AddCost(pow(x[0], 2) - pow(x[1], 2));
  auto solver = drake::solvers::IpoptSolver();
  // The user doesn't provide an initial guess.
  auto result = solver.Solve(prog, std::nullopt, std::nullopt);
  print("Without a good initial guess, success? ", result.is_success());
  print("Solution:", result.GetSolution(x));
  // Pass an initial guess
  Eigen::VectorXd initial_guess(2);
  initial_guess << -5, 0;
  result = solver.Solve(prog, initial_guess, std::nullopt);
  print("With a good initial guess, success? ", result.is_success());
  print("Solution:", result.GetSolution(x));
  return 0;
  ;
}
