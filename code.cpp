#include <iostream>
#include <queue>
#include <vector>
#include <random>

// Task structure representing a unit of work
struct Task {
  int id;
  int workload; // Simulates task processing time (higher value = longer processing)
  bool isShortFlow; // Flag indicating if this is a short flow
  // ... other task data (replace with your actual task data)
};

// Network Interface Card (NIC) structure
struct NIC {
    int id;
 std::queue<Task> packets; // Queue of tasks waiting for transmission
  bool canHandleShortFlows; // Flag indicating if NIC can handle short flows
  bool canHandleLongFlows;  // Flag indicating if NIC can handle long flows

  // Function to check if NIC can handle a specific flow type
  bool canHandleFlow(const Task& task) const {
    return (canHandleShortFlows && task.isShortFlow) || (canHandleLongFlows && !task.isShortFlow);
  } // Queue of tasks waiting for transmission
};
  

// Function to select the next NIC for a task based on LBT algorithm
NIC* selectNextNIC( std::vector<NIC>& nics,  Task& task, int totalLoad, int Q1, int Q2) {
  NIC* bestNIC = nullptr;
  int bestLoad = 0;
   int n=nics.size();
  // Loop through each NIC
  for ( int i=0;i<n;i++) {
   NIC &nic=nics[i];
    int queueLength = nic.packets.size();
    //std::cout<<queueLength<<std::endl;

    // Explore based on flow type and thresholds, considering NIC capabilities
    if (task.isShortFlow && nic.canHandleShortFlows) {
      if (queueLength < Q1) {
        // Short flow: prioritize shortest queue on a compatible NIC
        if (!bestNIC || queueLength < bestLoad) {
          bestNIC = &nic;
          bestLoad = queueLength;
        }
      } else {
        // Short flow with high congestion: avoid this NIC (implicit due to loop continuing)
      }
    } else if (!task.isShortFlow && nic.canHandleLongFlows) {
      if (totalLoad < Q1) {
        // Long flow, low total load: prioritize any compatible NIC
        if (!bestNIC || queueLength > bestLoad) {
          bestNIC = &nic;
          bestLoad = queueLength;
        }
      } else if (totalLoad >= Q1 && totalLoad < Q2) {
        // Long flow, medium total load: prioritize NICs with longer queues for long flows (if compatible)
        if (!bestNIC || queueLength > bestLoad) {
          bestNIC = &nic;
          bestLoad = queueLength;
        }
      } else {
        // Long flow, high total load: prioritize NICs with longest queues for best-effort flows (if compatible)
        if (!bestNIC || queueLength > bestLoad) {
          bestNIC = &nic;
          bestLoad = queueLength;
        }
      }
    } else {
      // Task flow type not compatible with NIC capabilities: avoid this NIC (implicit due to loop continuing)
    }
  }
  return bestNIC;
}
int main() {
  int numTasks, numNICs;

  // Get user input for number of tasks
  std::cout << "Enter the number of tasks to generate: ";
  std::cin >> numTasks;

  // Get user input for number of NICs
  std::cout << "Enter the number of NICs: ";
  std::cin >> numNICs;

  // Generate tasks with random workload and flow type (replace with your actual task generation logic)
  std::vector<Task> tasks;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 100); // Random workload between 1 and 100

  for (int i = 0; i < numTasks; ++i) {
    int isShortFlow = rand() % 2; // Randomly assign flow type (0: long, 1: short)
    Task task{i, dis(gen), isShortFlow};
    tasks.push_back(task);
  }

  // Get user input for number of packets in each NIC (replace with your actual network data)
  std::vector<NIC> nics(numNICs);
  for(int i=0;i<numNICs;i++)
  nics[i].id=i+1;
  for (int i = 0; i < numNICs; i++) {
    int numPackets;
    std::cout << "Enter the number of packets initially in NIC " << i + 1 << ": ";
    std::cin >> numPackets;
    nics[i].canHandleShortFlows=rand()%2;
    nics[i].canHandleLongFlows=!(nics[i].canHandleShortFlows);
    for (int j = 0; j < numPackets; ++j) {
      nics[i].packets.push(Task{}); // Placeholder tasks for initial NIC population
    }
    
  }

  // Get user input for LBT thresholds
  int Q1, Q2;
  std::cout << "Enter the value for threshold Q1: ";
  std::cin >> Q1;
  std::cout << "Enter the value for threshold Q2: ";
  std::cin >> Q2;

  // Calculate total workload across all tasks
  int totalLoad = 0;
  int siz=tasks.size();
  for (int i=0;i<siz;i++) {
    Task task=tasks[i];
    NIC* nextNIC = selectNextNIC(nics, task, totalLoad, Q1, Q2);
    if (nextNIC) {
     // std::cout<<nextNIC->id<<std::endl;
      nextNIC->packets.push(task);
      std::cout << "Found suitable NIC for " << (task.isShortFlow ? "short" : "long") << " flow task " << task.id << " with " <<" on NIC " << nextNIC->id << std::endl;

    } else {
      // Handle case where no suitable NIC is found (e.g., congestion on all NICs)
      std::cout << "No suitable NIC found for task " << task.id << std::endl;
    }
  }

  // ... (your code to process tasks on NICs or simulate transmission)


  return 0;
}