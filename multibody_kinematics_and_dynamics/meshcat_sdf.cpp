#include <drake/geometry/meshcat.h>
#include <drake/geometry/meshcat_visualizer.h>
#include <drake/multibody/plant/multibody_plant.h>
#include <drake/systems/framework/diagram_builder.h>
#include "drake/multibody/meshcat/joint_sliders.h"
#include "drake/multibody/parsing/parser.h"

#include <chrono>
#include <iostream>
#include <thread>

void model_inspector(std::shared_ptr<drake::geometry::Meshcat>& meshcat, std::string filename);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " path-to-sdf-file\n";
  }

  auto meshcat = std::make_shared<drake::geometry::Meshcat>(8080);

  model_inspector(meshcat, argv[1]);
  return 0;
}

void model_inspector(std::shared_ptr<drake::geometry::Meshcat>& meshcat, std::string filename) {
  meshcat->Delete();
  meshcat->DeleteAddedControls();
  drake::systems::DiagramBuilder<double> diagram_builder;

  // Note: the time_step here is chosen arbitrarily.
  auto [plant, scene_graph] =
      drake::multibody::AddMultibodyPlantSceneGraph(&diagram_builder, 0.001);

  // Load the file into the plant/scene_graph.
  drake::multibody::Parser parser(&plant);
  parser.AddModelFromFile(filename);
  plant.Finalize();

  // Add two visualizers, one to publish the "visual" geometry, and one to publish the "collision"
  // geometry.
  auto& visual = drake::geometry::MeshcatVisualizer<double>::AddToBuilder(
      &diagram_builder, scene_graph, meshcat,
      drake::geometry::MeshcatVisualizerParams{.role = drake::geometry::Role::kPerception,
                                               .prefix = "visual"});
  auto& collision = drake::geometry::MeshcatVisualizer<double>::AddToBuilder(
      &diagram_builder, scene_graph, meshcat,
      drake::geometry::MeshcatVisualizerParams{.role = drake::geometry::Role::kProximity,
                                               .prefix = "collision"});
  // Disable the collision geometry at the start; it can be enabled by the
  // checkbox in the meshcat controls.;
  meshcat->SetProperty("collision", "visible", false);

  auto* sliders =
      diagram_builder.AddSystem<drake::multibody::meshcat::JointSliders<double>>(meshcat, &plant);
  auto diagram = diagram_builder.Build();
  sliders->Run(*diagram);
}
