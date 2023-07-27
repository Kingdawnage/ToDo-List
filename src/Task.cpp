#include "Task.h"
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>

void SaveTasksToFile(const std::vector<Task>& tasks, const std::string& fileName) //Function for saving tasks to a file
{
	std::ofstream ostream(fileName);
	ostream << tasks.size();//This line writes the number  of tasks

	for (const Task& task : tasks) {// This line initiates a for loop that iterates over each element in the 'tasks' container.
		std::string description = task.description;
		std::replace(description.begin(), description.end(), ' ', '_');// This line uses the algorithm header file to replace every space with underscore

		ostream << '\n' << description << ' ' << task.done;
	}
}

std::vector<Task> LoadTasksFromFile(const std::string& fileName)// This line creates a function that loads the data in a file. It returns an array value
{
	if (!std::filesystem::exists(fileName)) {//This line checks whether the file exists already. If not it returns an empty array
		return std::vector<Task>();
	}

	std::vector<Task> tasks;
	std::ifstream istream(fileName);

	int n;// The purpose of this variable is to store the size of the number of the tasks
	istream >> n;

	for (int i = 0; i < n; i++) {//This line iterates based on the number of tasks and writes the description and whether its done
		std::string description;
		bool done;

		istream >> description >> done;
		std::replace(description.begin(), description.end(), '_', ' ');//This line replaces the _ with spaces once more
		tasks.push_back(Task{ description, done });
	}
	return tasks;
}
