/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * genetic-algorithm.h
 * Copyright (C) 2020 Benrick Smit <metatronicprogramming@hotmail.com>
 *
 * N_Layered_Chart is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * N_Layered_Chart is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GENETIC_ALGORITHM_H_
#define _GENETIC_ALGORITHM_H_

#include <vector>
#include <iostream>
#include <ctime>
#include <random>

#include "layered-chart-test.h"

class GeneticAlgorithm{
public:
	GeneticAlgorithm(int total_number_bubbles, int total_top_row_bubbles, bool use_unique_numbers = false);

	// This function generates 1000 solutions, checks each solution,
	// and stops only once at least one generated solution satisfies LayeredChartTest.
	// It uses genetic algorithm ideas to generate new possible solutions from past solutions
	void generate_solutions();

	// This function will search the total number of possible solutions
	// for the first solution that matches 100%
	void trim_solutions();

	// This function will output the solution that fits to the screen
	void display_solution() const;

	// Get a boolean which states whether there was at least one
	// solution in the mix
	bool has_solution() const;

protected:
	// This function will take three arguments which will create a vector for the chart
	// The min_value is the minimum allowed value for the generated numbers
	// The max_value is the maximum value allowed for the generated numbers
	// the num_elements is how many numbers need to be generated
	std::vector<int> generate_chart_data(int num_elements, int max_value, int min_value = 1);

	// Get the number of layers
	int get_num_layers() const;

	// Get the number of elements;
	int get_total_num_chart_elements() const;

	// Get the maximum allowed value;
	int get_max_allowed_value() const;

	// Get the minimum allowed value
	int get_min_allowed_value() const;

	// Get the number of bubbles in topmost layers
	int get_topmost_num_elements() const;

	// Set a boolean which indicates that there is at least one solution.
	void set_has_solution(bool condition);

	// Determine how many elements to remove should one, two, etc
	// layers match for this particular iteration
	int num_elements_to_remove(int num_elements_in_top_row, int num_rows_matched);

	// Remove a number of elements from the back of the vector
	std::vector<int> remove_elements(std::vector<int> chart, int num_elements_remove = 0);
private:
	std::vector<std::vector<int>> m_num_charts;	// Contains a list of charts
	std::vector<int> m_layered_chart;		// Contains the chart data
	int m_total_num_chart_elements;			// Contains the number of chart elements
	int m_start_number;				// Contains the number to start with (i.e. minimum allowed integer)
	int m_end_number;				// Contains the number to end with (i.e. maximum allowed integer)
	int m_top_row_num_bubbles;			// Contains the number of bubbles in the top row
	int m_number_of_layers;				// Contains the number of layers to be expected
	bool m_has_at_least_one_solution;		// Contains true if there was at least one solution in the mix
	bool m_use_unique_numbers;			// Contains the condition that allows only unique numbers should it be required
};

#endif // _GENETIC_ALGORITHM_H_

