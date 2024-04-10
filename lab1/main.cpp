#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>

std::vector<int> parseTokens(const std::string &line)
{
	std::istringstream iss(line);
	std::string token;
	std::vector<int> tokens;
	while (std::getline(iss, token, ' ') && tokens.size() < 1023)
	{
		try
		{
			int value = std::stoi(token);
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

std::vector<bool> generateChipSeq(std::vector<bool> &genA, std::vector<bool> &genB, const std::vector<unsigned int> &cons)
{
	std::vector<bool> chipSeq;
	for (auto i = 0; i < 1023; i++)
	{
		const auto nextB = genB.at(1) ^ genB.at(2) ^ genB.at(5) ^ genB.at(7) ^ genB.at(8) ^ genB.at(9);
		const auto rotated = genB.at(cons.at(0) - 1) ^ genB.at(cons.at(1) - 1);
		const auto res = genA.at(9) ^ rotated;
		const auto nextA = genA.at(2) ^ genA.at(9);

		// rotate right
		std::rotate(genA.rbegin(), genA.rbegin() + 1, genA.rend());
		std::rotate(genB.rbegin(), genB.rbegin() + 1, genB.rend());
		// overwrite
		genA.at(0) = nextA;
		genB.at(0) = nextB;

		chipSeq.push_back(res);
		// printf("%d", res);
	}
	// printf("\n");
	return chipSeq;
}

int calcCrossCorr(const std::vector<int> &sumSignals, const std::vector<bool> &chipSeq, const unsigned int offset)
{
	int sum = 0;
	for (auto i = 0; i < 1023; i++)
	{
		const auto chip = chipSeq.at(i);
		const auto signal = sumSignals.at(i + offset);
		sum += (chip ? 1 : -1) * signal;
	}
	return sum;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Please provide a file name. Assuming ../lab1/gps_sequence_22.txt" << std::endl;
		argv[1] = const_cast<char *>("../lab1/gps_sequence_22.txt");
		// return 1;
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

	std::vector<int> sumSignals = parseTokens(line);
	for (int i = 0; i < 1023; i++)
	{
		if (sumSignals.at(i) != sumSignals.at(i + 1023))
		{
			std::cerr << "Error: Signal is not repeated" << std::endl;
			return 1;
		}
	}

	// Gold Sequence
	std::vector<std::vector<bool>> goldCodes;
	std::vector<std::vector<unsigned int>> cons = {{2, 6}, {3, 7}, {4, 8}, {5, 9}, {1, 9}, {2, 10}, {1, 8}, {2, 9}, {3, 10}, {2, 3}, {3, 4}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}, {1, 4}, {2, 5}, {3, 6}, {4, 7}, {5, 8}, {6, 9}, {1, 3}, {4, 6}};
	for (auto j = 0; j < 24; j++)
	{
		std::vector<bool> genA(10, true);
		std::vector<bool> genB(10, true);

		std::vector<bool> chipSeq = generateChipSeq(genA, genB, cons[j]);
		goldCodes.push_back(chipSeq);
	}

	for (auto satellite = 0; satellite < 24; satellite++)
	{
		std::vector<bool> chipSeq = goldCodes[satellite];
		for (auto offset = 0; offset < 1023; offset++)
		{
			const auto crossCorr = calcCrossCorr(sumSignals, chipSeq, offset);
			if (crossCorr > 828 || crossCorr < -828)
			{
				// printf("%d\n", crossCorr);
				printf("Satellite %d has sent bit %d (delta = %d)\n", satellite + 1, crossCorr > 828 ? 1 : 0, offset + 1);
			}
		}
	}
}