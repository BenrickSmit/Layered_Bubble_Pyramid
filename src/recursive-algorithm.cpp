/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * recursive-algorithm.cpp
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

#include "recursive-algorithm.h"

// PUBLIC:
RecursiveAlgorithm::RecursiveAlgorithm(int total_number_bubbles, int total_top_row_bubbles, bool use_unique_numbers):
 m_total_num_chart_elements(total_number_bubbles),m_start_number(1) , m_end_number(total_number_bubbles),
  m_top_row_num_bubbles(total_top_row_bubbles), m_use_unique_numbers(use_unique_numbers)
 {
        // Seed the random number generator
        srand(time(0));
}// end of constructor

void RecursiveAlgorithm::generate_solutions(){
        // Variables
        std::vector<int> starting_solution;
        std::vector<int> potential_solution;
        long long solution_generation_counter = 0;
        const long long TOTAL_NUM_TEST_RUNS = 1000000;

        // Continue generation until there is a solution
        while (!has_solution()){
                //Generate random numbers that are equal in number to m_top_row_num_bubbles
                starting_solution.clear();
                for (auto counter = 0; counter < get_topmost_num_elements(); counter++){
                      auto random_number = (rand() % get_max_allowed_value()) + get_min_allowed_value();
                      starting_solution.push_back(random_number);
                }// end of for loop

                // Recursively generate the rest of the pattern with generate_row
                generate_row(starting_solution, potential_solution);

                // Return the resulting solution as a memeber variable
                m_layered_chart = potential_solution;

                // Test the solution with the LayeredChartTest class
                LayeredChartTest chart_test(get_chart_solution(), get_total_num_chart_elements(),
                                            get_topmost_num_elements(), m_use_unique_numbers);
                if (chart_test.test_layered_chart()){
                        set_has_solution(true);
                }// end of if

                // Break out of the loop if solution_generation_counter is greater than the constant value
                if (solution_generation_counter >= TOTAL_NUM_TEST_RUNS){
                        break;
                }// end of if
                // Increment the total number of while loop runs;
                solution_generation_counter++;
                //if (solution_generation_counter % 1000 == 0) std::cout << ">> " << solution_generation_counter << std::endl;
        }// end of while loop

        if (!has_solution() || (solution_generation_counter == TOTAL_NUM_TEST_RUNS)){
                std::cout << "Solution Not Found!" << std::endl;
                std::cout << "Total Number Generations: " << solution_generation_counter << std::endl;
        } else {
                std::cout << "Solution Found!" << std::endl;
                std::cout << "Solution Found Within " << solution_generation_counter << " Generation(s)" << std::endl;
        }// end of if
}// end of generate_solutions()

void RecursiveAlgorithm::display_solution() const{
        // Variables
        auto element_counter = 0;
        int new_line_location = get_topmost_num_elements();
        int counter = 0;


        //Output the solution in the form of a pyramid
        while ((element_counter < get_chart_solution().size())){   // Display only elements in the list
            std::cout << get_chart_solution().at(element_counter) << " ";

            // Increment as necessary
            counter++;
            element_counter++;

            // Build a pyramid by outputting \n where necessary
            if (counter == new_line_location){
                    std::cout << std::endl;
                    counter = 0;
                    new_line_location--;
            }// end of if
        }// end of while

        std::cout << std::endl;
}// end of display_solution()

bool RecursiveAlgorithm::has_solution() const {
        return m_has_solution;
}// end of has_solution


// PROTECTED:
void RecursiveAlgorithm::generate_row(std::vector<int> top_row, std::vector<int> &full_solution){
          // Variables
          std::vector<int> to_return;
          std::vector<int> calculated_vector;

          // Base condition
          if (top_row.size() == 1){
                  // Simply return an empty vector
                  to_return = top_row;
                  full_solution.push_back(to_return.front());
          } else{
                  // Calculate the vector as necessary by using the provided vector
                  for (auto first = 0, second = first + 1; second < top_row.size(); first++, second++){
                          auto result = top_row.at(first) - top_row.at(second);
                          auto absolute_subtraction = std::abs(result);

                          calculated_vector.push_back(absolute_subtraction);
                  }// end of for loop

                  // Create the second smaller part of the chart
                  full_solution.insert(full_solution.end(), top_row.begin(), top_row.end());
                  generate_row(calculated_vector, full_solution);
          }// end of if
}// end of generate_solution;

void RecursiveAlgorithm::set_has_solution(bool condition){
        m_has_solution = condition;
}// end of set_has_solution()

int RecursiveAlgorithm::get_total_num_chart_elements() const{
        return m_total_num_chart_elements;
}// end of get_total_num_chart_elements

int RecursiveAlgorithm::get_max_allowed_value() const{
        return m_end_number;
}// end of get_max_allowed_value()

int RecursiveAlgorithm::get_min_allowed_value() const{
        return m_start_number;
}// end of get_min_allowed_value()

int RecursiveAlgorithm::get_topmost_num_elements() const{
        return m_top_row_num_bubbles;
}// end of get_topmost_num_elements

std::vector<int> RecursiveAlgorithm::get_chart_solution() const{
        return m_layered_chart;
}// end of get_solution
