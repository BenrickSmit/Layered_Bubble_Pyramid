/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * recursive-algorithm.h
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

#ifndef _RECURSIVE_ALGORITHM_H_
#define _RECURSIVE_ALGORITHM_H_

#include <vector>
#include <iostream>
#include <cmath>
#include <ctime>
#include <random>

#include "layered-chart-test.h"

class RecursiveAlgorithm{
public:
	RecursiveAlgorithm(int total_number_bubbles, int total_top_row_bubbles, bool use_unique_numbers = false);

	// Generate the data as necessary
	void generate_solutions();

	// This function will output the solution that fits to the screen
	void display_solution() const;

	// This function will return true only if the m_has_solution is true, i.e.
	// if there is a solution that has been successfully tested
	bool has_solution() const;

protected:
	// Generate the solution recursively after the first n numbers have been generated.
	// That is, the function is given a vector signifying the top row, and it has to
	// calculate the bottom row to
	void generate_row(std::vector<int> top_row, std::vector<int> &full_solution);

	// This function allows you to set whether there is a solution or not
	void set_has_solution(bool condition = false);

	// Get the number of elements;
	int get_total_num_chart_elements() const;

	// This function return the maximum allowable value for solution generation
	int get_max_allowed_value() const;

	// This function returns the minimum allowable value for the soution generation
	int get_min_allowed_value() const;

	// Get the number of bubbles in topmost layers
	int get_topmost_num_elements() const;

	// This function returns the layered chart for use without modification
	std::vector<int> get_chart_solution() const;

private:
	std::vector<int> m_layered_chart;		  // Contains the list of values that make up the chart
	int m_total_num_chart_elements;			  // Contains the number of chart elements
	int m_start_number;				  // Contains the number to start with (i.e. minimum allowed integer)
	int m_end_number;				  // Contains the number to end with (i.e. maximum allowed integer)
	int m_top_row_num_bubbles;			  // Contains the number of bubbles in the top row
	bool m_has_solution;				  // This value determines whether the class has found a tested solution
	int m_use_unique_numbers;			  // Contains the condition to only use unique numbers
};

#endif // _RECURSIVE_ALGORITHM_H_

