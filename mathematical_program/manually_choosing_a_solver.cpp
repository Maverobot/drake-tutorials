#include <drake/solvers/ipopt_solver.h>
#include <drake/solvers/mathematical_program.h>

#include <iostream>

template <typename... Args>
void print(Args&&... value) {
  (std::cout << ... << value) << "\n---\n";
}

int main(int argc, char* argv[]) {
  /*
   * Solves a simple optimization problem
   *
   * min x(0)
   *
   * subject to x(0) + x(1) = 1
   *            0 <= x(1) <= 1
   */
  // Set up the optimization problem.
  auto prog = drake::solvers::MathematicalProgram();
  auto x = prog.NewContinuousVariables(2);
  print("x = ", x);
  auto constraint1 = prog.AddConstraint(x[0] + x[1] == 1);
  print("constraint1: ", constraint1);
  auto constraint2 = prog.AddConstraint(0 <= x[1]);
  print("constraint2: ", constraint2);
  auto constraint3 = prog.AddConstraint(x[1] <= 1);
  print("constraint3: ", constraint3);
  auto cost1 = prog.AddCost(x[0]);
  print("cost1: ", cost1);

  drake::solvers::IpoptSolver solver;
  // The initial guess is [1, 1]. The third argument is the options for Ipopt solver, and we set no
  // solver options.
  Eigen::VectorXd initial_guess(2);
  initial_guess << 1, 1;
  auto result = solver.Solve(prog, initial_guess, std::nullopt);
  print(result.get_solution_result());
  print("x* = ", result.GetSolution(x));
  print("Solver is ", result.get_solver_id().name());
  print("Ipopt solver status: ", result.get_solver_details<drake::solvers::IpoptSolver>().status,
        ", meaning ",
        result.get_solver_details<drake::solvers::IpoptSolver>().ConvertStatusToString());
  return 0;
  ;
}
