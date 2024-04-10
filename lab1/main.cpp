#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

std::vector<unsigned int> parseTokens(const std::string &line)
{
	std::istringstream iss(line);
	std::string token;
	std::vector<unsigned int> tokens;
	while (std::getline(iss, token, ' ') && tokens.size() < 1023)
	{
		try
		{
			unsigned int value = std::stoul(token);
			tokens.push_back(value);
		}
		catch (const std::invalid_argument &ia)
		{
			std::cerr << "Invalid argument: " << ia.what() << '\n';
		}
		catch (const std::out_of_range &oor)
		{
			std::cerr << "Out of Range error: " << oor.what() << '\n';
		}
	}

	// Copy the tokens and append them to the end of the vector
	tokens.insert(tokens.end(), tokens.begin(), tokens.end());

	if (tokens.size() != 2046)
	{
		std::cout << "Error tokens has size " << tokens.size() << std::endl;
		throw std::runtime_error("Invalid number of tokens");
	}
	return tokens;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Please provide a file name" << std::endl;
		return 1;
	}

	std::ifstream file(argv[1]);
	if (!file)
	{
		std::cerr << "Could not open file" << std::endl;
		return 1;
	}

	std::string line;
	if (!std::getline(file, line))
	{
		return 0;
	}
	std::string line;
	if (!std::getline(file, line))
	{
		return 0;
	}

	std::vector<unsigned int> sumSignals = parseTokens(line);

	// ... (remaining code)
}