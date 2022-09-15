#include <drake/common/eigen_types.h>
#include <drake/examples/pendulum/pendulum_plant.h>
#include <drake/systems/analysis/simulator.h>
#include <drake/systems/controllers/pid_controller.h>
#include <drake/systems/framework/diagram_builder.h>
#include <drake/systems/primitives/symbolic_vector_system.h>
#include <drake/systems/primitives/vector_log_sink.h>

#include "matplotlibcpp.h"

// sudo apt install libgraphviz-dev
#include <graphviz/gvc.h>

#include <fstream>

namespace plt = matplotlibcpp;

bool createPNGFromDotFile(std::string dot_file_name) {
  GVC_t* gvc;
  Agraph_t* g;
  FILE* fp;
  gvc = gvContext();
  fp = fopen((dot_file_name + ".dot").c_str(), "r");
  g = agread(fp, 0);
  gvLayout(gvc, g, "dot");
  gvRender(gvc, g, "png", fopen((dot_file_name + ".png").c_str(), "w"));
  gvFreeLayout(gvc, g);
  agclose(g);
  return (gvFreeContext(gvc));
}

int main(int argc, char* argv[]) {
  drake::systems::DiagramBuilder<double> diagram_builder;

  auto* pendulum =
      diagram_builder.AddNamedSystem<drake::examples::pendulum::PendulumPlant<double>>("pendulum");

  Eigen::VectorXd kp(1);
  kp << 10;
  Eigen::VectorXd ki(1);
  ki << 1;
  Eigen::VectorXd kd(1);
  kd << 1;
  auto* controller =
      diagram_builder.AddNamedSystem<drake::systems::controllers::PidController<double>>(
          "controller", kp, ki, kd);

  // Now "wire up" the controller to the pendulum.
  diagram_builder.Connect(pendulum->get_state_output_port(),
                          controller->get_input_port_estimated_state());
  diagram_builder.Connect(controller->get_output_port(), pendulum->get_input_port());

  // Make the desired_state input of the controller an input to the diagram.
  diagram_builder.ExportInput(controller->get_input_port_desired_state());

  // Log the state of the pendulum.
  drake::systems::VectorLogSink<double>* logger =
      drake::systems::LogVectorOutput<double>(pendulum->get_state_output_port(), &diagram_builder);
  logger->set_name("logger");

  std::unique_ptr<drake::systems::Diagram<double>> diagram = diagram_builder.Build();
  diagram->set_name("diagram");

  // Write the graph to the file "graph.png"
  std::ofstream out("graph.dot");
  out << diagram->GetGraphvizString(2);
  out.close();
  createPNGFromDotFile("graph");

  // Set up a simulator to run this diagram.
  drake::systems::Simulator<double> simulator(*diagram);
  auto& context = simulator.get_mutable_context();

  // We'll try to regulate the pendulum to a particular angle.
  auto desired_angle = M_PI / 2.;

  // First we extract the subsystem context for the pendulum.
  auto& pendulum_context = diagram->GetMutableSubsystemContext(*pendulum, &context);
  // Then we can set the pendulum state, which is (theta, thetadot).
  pendulum_context.get_mutable_continuous_state_vector().SetFromVector(
      drake::Vector2<double>{desired_angle + 0.1, 0.2});

  // The diagram has a single input port (port index 0), which is the desired_state.
  diagram->get_input_port(0).FixValue(&context, drake::Vector2<double>{desired_angle, 0.});

  // Simulate for 10 seconds.
  simulator.Initialize();
  simulator.AdvanceTo(40);

  // Read log data
  drake::systems::VectorLog<double> log = logger->FindLog(simulator.get_context());

  // Plot the desired and current theta
  auto t = log.sample_times();
  plt::figure();
  plt::plot(t, log.data().row(0), "tab:blue");
  plt::plot(std::array<double, 2>{t[0], t[t.size() - 1]},
            std::array<double, 2>{desired_angle, desired_angle}, "tab:green");

  plt::xlabel("Sample time");
  plt::ylabel("theta (rad)");
  plt::show();

  // TODO: strangely the plot looks different than from the python tutorial.
}
