/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * layered-chart-test.h
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

#ifndef _LAYERED_CHART_TEST_H_
#define _LAYERED_CHART_TEST_H_

#include <vector>

class LayeredChartTest{
public:
	LayeredChartTest(std::vector<int> chart_to_test, int total_number_bubbles, int total_top_row_bubbles, bool unique_numbers = false);

	// This function will determine whether all the elements are within the range required,
	// while making sure that layered chart still does what it needs to, i.e. that the each
	// element of the row below the current can be found with the help of an absolute substraction
	// of the elements directly above it.
	bool test_layered_chart();

	// This function determines how many layers matched and returns it as a result.
	double get_genetic_match();

	// Returns the number of layers that match
	int get_num_layers_match() const;

protected:
	// This funtion will return true if the layered_chart follows the expected pattern. Otherwise
	// it will return false.
	// It does this by taking a subset of the current solution's next row, and calculating it
	// based on the current row. Should the two subsets be equal, it continues, otherwise, it
	// does not continue
	bool recursive_test(std::vector<int> subset_chart_vector, int min_pos, int row_length);

	// Create the subset vector as required starting from min_pos ending at
	// max_pos
	std::vector<int> create_subset_vector(int min_pos = 0, int max_pos = 1);

	// Calculates the row below based on the top row, by following the absolute subtraction
	// but only as far as the row length is required.
	// top_row is the row to use to calculate the bottom row.
	// row_length is the length of the top_row
	std::vector<int> calculate_row_below(std::vector<int> top_row);

	// Determines whether the provided vectors have identical values at the same places
	// Can only be used for vectors of the same length
	bool equals(std::vector<int> lhs, std::vector<int> rhs);

	// Determine whether a subset would be possible given the current constraints
	bool can_create_subset_vector(int min_pos = 0, int max_pos = 1);

	// Determine whether only values between the specified minimum and maximum value
	// has been used
	bool value_within_range(int max_value, int min_value = 1);

	// Determine whether the values only appear once in the list
	bool singleton_values();

	// Get the total number of bubbles in the chart
	int get_total_bubbles() const;

	// Get the total number of bubbles in the topmost row
	int get_total_top_bubbles() const;

	// Returns the layered chart stored in the test class
	const std::vector<int>& get_layered_chart() const;

private:
	std::vector<int> m_layered_chart;	    // The resulting chart (as a vector) to test
	int m_total_bubbles;			    // The total number of bubbles in the chart - gives the max
						    // value that you can use
	int m_top_row_bubbles;			    // The total number of bubbles in the topmost row

	double m_number_of_successes;		    // total number of successful loops
	double m_number_of_layers;		    // total number of layers
	bool m_unique_numbers;			    // Should this condition be set, then you can only use unique numbers
};

#endif // _LAYERED_CHART_TEST_H_

