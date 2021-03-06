// GA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Population.h"
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <sstream>

const int NUM_OF_GENOMES = 100;		//Number of solutions
const int NUM_OF_CHROMOSOMES = 5;	//Number of components in solution
const int NUM_OF_GENES = 0;			//Number of lowest level components in chromosomes
float TOTAL_SCORE = 0;

std::string sentence = "hello and welcome to class";
std::vector<std::string> words;


std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(-1, 1);

char GeneFunc()
{
	int num = rand() % 26 + 97;
	return (char)num;
}

void FitnessFunc(Genome<char>& a_genome)
{
	float score = 0;

	auto calculateScore = [](char target, char actual)
	{
		int forwardDist = std::abs(target - actual);
		int secondDist = (actual - 'a') + ('z' - target);
		return std::min(forwardDist, secondDist);
	};

	for (int i = 0; i < words.size(); ++i)
	{
		auto& word = words[i];
		const auto generatedWord = a_genome.GetChromosomeAtPoint(i).getGenes();

		for (int j = 0; j < word.size() && j < generatedWord.size(); ++j)
		{
			auto letterScore = calculateScore(word[j], generatedWord[j]);
			score += letterScore;
		}
		int sizeDifference = std::abs((int)word.size() - (int)generatedWord.size());
		score += sizeDifference * 26;
	}

	//for (size_t i = 0; i < NUM_OF_CHROMOSOMES; i++)
	//{
	//	auto word = words[i];
	//	auto myWord = a_genome.GetChromosomeAtPoint(i).getGenes();
	//	if (myWord.size() > word.size())
	//	{
	//		for (int j = 0; j < word.size(); ++j)
	//		{
	//			if (myWord[j] == '/0')
	//			{
	//				score += 26;
	//			}
	//			else
	//			{
	//				score += calculateScore(word[j], myWord[j]);
	//			}
	//		}
	//		int penalty = (myWord.size() - word.size()) * 26;
	//		score += penalty;
	//	}
	//	else if (myWord.size() < word.size())
	//	{
	//		for (int j = 0; j < myWord.size(); ++j)
	//		{
	//			if (myWord[j] == '\0')
	//			{
	//				score += 26;
	//			}
	//			else
	//			{
	//				score += calculateScore(word[j], myWord[j]);
	//			}
	//		}
	//		int penalty = (word.size() - myWord.size()) * 26;
	//		score += penalty;
	//	}
	//	else
	//	{
	//		for (int j = 0; j < myWord.size(); ++j)
	//		{
	//			if (myWord[j] == '\0')
	//			{
	//				score += 26;
	//			}
	//			else
	//			{
	//				score += calculateScore(word[j], myWord[j]);
	//			}
	//		}
	//	}
	//}
	a_genome.setFitness(-score);
}

void MutateFunc(Chromosome<char>& a_chromosome)
{	
	int charData;

	int randNum = rand() % 100;
	if (randNum < 2)
	{
		a_chromosome.RemoveGene();
	}
	if (randNum > 98)
	{
		a_chromosome.AddGene();
	}
	int size = a_chromosome.getGenes().size();
	for (size_t i = 0; i < size; ++i)
	{
		int chance = rand() % 101;
		if (chance < 10)
		{
			charData = (int)a_chromosome.getGenes()[i];
			charData += dis(gen);
			if (charData < 97) charData = 122;
			if (charData > 122) charData = 97;
			//charData = std::clamp(charData, 97, 122);

			a_chromosome.setGene(i, (char)charData);
			break;
		}
	}
}

void DisplayFunction(std::vector<Genome<char>> a_genomes)
{
	int size = a_genomes[0].GetNumberOfChromosomes();
	for (int j = 0; j < size; ++j)
	{
		auto& chromosome = a_genomes[0].GetChromosomeAtPoint(j);
		for (int k = 0; k < chromosome.getGenes().size(); k++)
		{
			std::cout << a_genomes[0].GetChromosomeAtPoint(j).getGenes()[k];
		}
		std::cout << " ";
	}
	std::cout << "		" << a_genomes[0].GetFitness() << std::endl;	
}

int main()
{
	srand((unsigned int)time(NULL));

	std::stringstream iss(sentence);
	std::string item;
	while (std::getline(iss, item, ' '))
	{
		words.push_back(item);
	}
	for (int i = 0; i < words.size(); ++i)
	{
		TOTAL_SCORE += 26 * words[i].size();
	}
	///Method 1:
	Population<char> m_pop = Population<char>(NUM_OF_GENOMES, NUM_OF_CHROMOSOMES, NUM_OF_GENES, GeneFunc);
	m_pop.SetFitnessFunction(FitnessFunc);
	m_pop.SetMutateFunction(MutateFunc);
	m_pop.SetDisplayFunction(DisplayFunction);
	///Method 2:
	/*Population<char> m_pop = Population<char>();
	
	m_pop.SetNumberOfGenomes(NUM_OF_GENOMES);
	m_pop.SetNumberOfChromosomes(NUM_OF_CHROMOSOMES);
	m_pop.SetNumberOfGenes(NUM_OF_GENES);

	m_pop.SetGeneFunction(GeneFunc);

	m_pop.SetFitnessFunction(FitnessFunc);
	m_pop.SetMutateFunction(MutateFunc);
	m_pop.SetDisplayFunction(DisplayFunction);

	m_pop.GenerateGenomes();*/

	int numOfGeneration = 0;

	

	while (true)
	{
		m_pop.CalculateFitness();

		if (m_pop.GetFittestGenome().GetFitness() == 0)
		{
			std::cout << "--------------------------------------" << std::endl;
			m_pop.DisplayFitest();
			std::cout << "Number of Generations: " << numOfGeneration << std::endl;
			break;
		}

		m_pop.DisplayFitest();		

		m_pop.SelectionAndBreeding(SelectionMethod::TOURNAMENT);

		m_pop.Mutate();

		numOfGeneration++;
	}

	
	system("PAUSE");
    return 0;
}

