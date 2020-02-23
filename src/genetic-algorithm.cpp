/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * genetic-algorithm.cpp
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

#include "genetic-algorithm.h"

//PUBLIC:
GeneticAlgorithm::GeneticAlgorithm(int total_number_bubbles, int total_top_row_bubbles, bool use_unique_numbers)
  :m_total_num_chart_elements(total_number_bubbles), m_start_number(1), m_end_number(total_number_bubbles),
   m_top_row_num_bubbles(total_top_row_bubbles), m_use_unique_numbers(use_unique_numbers){
        // Calculate the number of layers
        auto layer_counter = 0;
        auto top_layer_number = total_top_row_bubbles;
        while (top_layer_number > 1){
                layer_counter++;
                top_layer_number--;
        }// end of while

        // Seed the random number generator
        srand(time(0));
        set_has_solution(false);
}// end of constructor

GeneticAlgorithm::~GeneticAlgorithm(){
        // Clean all the vectors
        for(std::vector<int> solution : m_num_charts){
            solution.clear();
            solution.resize(0);
        }// end of for
        m_num_charts.clear();
        m_num_charts.resize(0);
}// end of deconstructor

void GeneticAlgorithm::generate_solutions(){
        // Variable
        auto total = get_total_num_chart_elements();
        auto max_nums = ((total == 3) ? 10:((total == 6) ? (100):(1000)));
        //max_nums = 100000;

        const long long TOTAL_NUMBER_TEST_RUNS = max_nums;           // How many total loops to expect
        const long long TOTAL_NUM_GENERATIONS = 1000;               // How many random vectors need to be created
        long long solution_generator_counter = 0;                   // How many solution loops were generated near the end (while loops)
        long long total_new_solutions_generated = 0;                // How many individual possibilities were generated
        std::vector<std::vector<int>> partial_fit_solutions;        // Store vectors that fit the criteria partially
        std::vector<std::vector<int>> full_solutions;               // Store vectors that match perfectedly
        std::vector<std::vector<int>> possible_solutions;           // solutions that are randomly generated
        std::vector<int> temp_solution;                             // Contains a solution for temporary use
        std::vector<int> partial_generation;                       // Contains only a part of the solution necessary

        // Generate the required information until TOTAL_NUM_TEST_RUNS has been fulfilled
        m_num_charts.clear();
        partial_generation.clear();
        full_solutions.clear();
        possible_solutions.clear();
        temp_solution.clear();
        while (solution_generator_counter < TOTAL_NUMBER_TEST_RUNS){
                // Cycle through the partial fits and determine how many elements need to be regenerated
                // IF it contains any data
                if (!partial_fit_solutions.empty()){
                        // Find out how many new elements need to be generated that don't fit the tests
                        for(auto counter = 0; counter < partial_fit_solutions.size(); counter++){
                                // Clear Temp Solution, and assign it the relevant solution from partial_fit_solutions for testing
                                temp_solution.clear();
                                temp_solution = partial_fit_solutions.at(counter);
                                LayeredChartTest partial_success_testing(temp_solution, get_total_num_chart_elements(), get_topmost_num_elements(), m_use_unique_numbers);

                                // Should either the full vector fit, or only a part of it
                                if (!partial_success_testing.test_layered_chart()){
                                        // Calculate the values required for the genetic algorithm
                                        auto num_layers_matched = partial_success_testing.get_num_layers_match();
                                        auto num_elements_to_generate = num_elements_to_remove(get_topmost_num_elements(), num_layers_matched+1);
                                        auto num_elements_to_keep = temp_solution.size() - num_elements_to_generate;

                                        // Remove the unnecessary parts of the temp_solution.
                                        temp_solution = remove_elements(temp_solution, num_elements_to_keep);
                                        partial_generation = generate_chart_data(num_elements_to_generate, get_max_allowed_value(), get_min_allowed_value());

                                        temp_solution.insert(temp_solution.end(), partial_generation.begin(), partial_generation.end());
                                        partial_fit_solutions[counter] = temp_solution;
                                        total_new_solutions_generated++;
                                } else{   // Partial Matches only
                                        full_solutions.push_back(temp_solution);
                                }// end of if
                        }// end of for
                }// end of if

                // Generate new vectors such that possible_solutions == TOTAL_NUM_GENERATIONS after the addition
                // of any partial_fit_solutions, should it exist
                // Should there be any data to add from the partial solutions
                possible_solutions.clear();
                if(!partial_fit_solutions.empty()){
                        possible_solutions.insert(possible_solutions.end(), partial_fit_solutions.begin(),
                                                  partial_fit_solutions.end());
                        partial_fit_solutions.clear();
                }// end of if
                // Generate the required data but only as much as is required to reach TOTAL_NUM_GENERATIONS
                auto new_size = (possible_solutions.empty() ? 0:possible_solutions.size()-1);
                temp_solution.clear();
                for (auto counter = new_size; counter < TOTAL_NUM_GENERATIONS; counter++){
                        temp_solution = generate_chart_data(get_total_num_chart_elements(), get_max_allowed_value(), get_min_allowed_value());
                        possible_solutions.push_back(temp_solution);
                        total_new_solutions_generated++;
                }// end of for loop


                // Test the solutions and narrow them down into partial_fit_solutions, and full_solutions
                for(std::vector<int> possible_solution : possible_solutions){
                        LayeredChartTest possible_solution_tester(possible_solution, get_total_num_chart_elements(),
                                                                  get_topmost_num_elements(), m_use_unique_numbers);

                        // Test full solutions and partial solutions. Ignore outright failures
                        if (possible_solution_tester.test_layered_chart()){
                                full_solutions.push_back(possible_solution);
                                set_has_solution(true);
                        } else{
                                // Determine whether it's a total failure or whether it's only partial
                                if (possible_solution_tester.get_num_layers_match() > 0){
                                        partial_fit_solutions.push_back(possible_solution);
                                }// end of if
                        }// end of if
                }// end of for loop

                // Increment the loop counter
                solution_generator_counter++;

                m_num_charts = full_solutions;
                trim_solutions();
                full_solutions = m_num_charts;

                //Display the number of full solutions found
                if(solution_generator_counter % 2 == 0) std::cout << "loop: " << solution_generator_counter << " UniqueSol: " << full_solutions.size() << " PartialSol: " << partial_fit_solutions.size() << " GenSol: " << possible_solutions.size() << "\r";
        }// end of while loop
        std::cout << std::endl;

        //m_num_charts.insert(m_num_charts.end(), full_solutions.begin(), full_solutions.end());
        m_num_charts = full_solutions;
        trim_solutions();

        // Display more information about the solution:
        std::cout << "" << std::endl;
        std::cout << "============================================" << std::endl;
        std::cout << "Method: Genetic Algorithm" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "A Genetic Algorithm uses the principles of " << std::endl;
        std::cout << "evolution to iterativesly find a solution that" << std::endl;
        std::cout << "fits a number of criteria to create a solution" << std::endl;
        std::cout << "that is fit enough to survive the tests." << std::endl;
        std::cout << "============================================" << std::endl;
        std::cout << "" << std::endl;

        // Determine which condition was met
        if((TOTAL_NUMBER_TEST_RUNS == solution_generator_counter) && (!has_solution())) {
                std::cout << "Solution Not Found!" << std::endl;
                std::cout << "Total Generations: " << TOTAL_NUMBER_TEST_RUNS << std::endl;
                std::cout << "Total Number Solutions Generated: " << total_new_solutions_generated << std::endl;

                //Calculate the total number of possible permutations
                long long total_possible_solutions = 1;
                int factorial = get_topmost_num_elements();
                while(factorial > 1){
                        total_possible_solutions *= factorial;
                        factorial--;
                }//end of while

                std::cout << "Total Number Possible Permutations: " << total_possible_solutions << std::endl;
        } else {
                std::cout << "Solution(s) Found!" << std::endl;
                std::cout << "Found Within " << total_new_solutions_generated << " Generations" << std::endl;
        }// end of if else
}// end of generate_solutions()

void GeneticAlgorithm::trim_solutions(bool test){
        // Find the solutions out of the multitude that fits, then clear the rest
        std::vector<int> temp_chart;
        std::vector<std::vector<int>> solutions;       //Contains all the solutions
        solutions.clear();
        for (auto counter = 0; counter < m_num_charts.size(); counter++){
                temp_chart = m_num_charts.at(counter);
                LayeredChartTest test_chart(temp_chart, get_total_num_chart_elements(), get_topmost_num_elements(), m_use_unique_numbers);

                if (test){
                        if(test_chart.test_layered_chart()){
                                solutions.push_back(temp_chart);
                        }//end of if
                }else{
                        solutions.push_back(temp_chart);
                }// end of if
        }// end of for

        m_num_charts.clear();
        m_num_charts = solutions;
        solutions.clear();

        //Make sure to only store different solutions - no identical solutions
        std::sort(m_num_charts.begin(), m_num_charts.end());
        m_num_charts.erase(std::unique(m_num_charts.begin(), m_num_charts.end()), m_num_charts.end());
}// end of trim_solutions();

void GeneticAlgorithm::display_solution() const{
        //Account for all complete solutions
        std::cout << "" << std::endl;
        for(std::vector<int> solution: m_num_charts){
                //Solution Variables
                int element_counter = 0;
                int new_line_location = get_topmost_num_elements();
                int counter = 0;

                std::cout << "Solution:" << std::endl;
                //Output the solution in the form of a pyramid
                while ((element_counter < solution.size())){   // Display only elements in the list
                    std::cout << solution.at(element_counter) << " ";

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
        }// end of for loop

        std::cout << std::endl;
}// end of display_solution

bool GeneticAlgorithm::has_solution() const{
        return m_has_at_least_one_solution;
}// end of has_solution()


//PROTECTED:
std::vector<int> GeneticAlgorithm::generate_chart_data(int num_elements, int max_value, int min_value){
        // Variables
        static std::vector<int> to_return;

        //Generate the random data
        to_return.clear();
        for (auto counter = 0; counter < num_elements; counter++){
                //Generate random numbers, but only once, so you limit the possibility of a solution to n!, and not n^n.
                auto random_number = 0;
                do {
                        random_number = (rand() % max_value) + min_value;
                }while(((std::find(to_return.begin(), to_return.end(), random_number)) != to_return.end()) && m_use_unique_numbers);

                to_return.push_back(random_number);
        }// end of for loop

        return to_return;
}// end of generate_chart_data()

int GeneticAlgorithm::get_num_layers() const{
        return m_number_of_layers;
}// end of get_num_layers()

int GeneticAlgorithm::get_total_num_chart_elements() const{
        return m_total_num_chart_elements;
}// end of get_total_num_chart_elements

int GeneticAlgorithm::get_max_allowed_value() const{
        return m_end_number;
}// end of get_max_allowed_value()

int GeneticAlgorithm::get_min_allowed_value() const{
        return m_start_number;
}// end of get_min_allowed_value()

int GeneticAlgorithm::get_topmost_num_elements() const{
        return m_top_row_num_bubbles;
}// end of get_topmost_num_elements()

void GeneticAlgorithm::set_has_solution(bool condition = false) {
        m_has_at_least_one_solution = condition;
}// end of set_has_solution

int GeneticAlgorithm::num_elements_to_remove(int num_elements_in_top_row, int num_rows_matched){
        // Variables
        int elements_to_remove = 1;
        std::vector<int> elements_at_layer;

        // Populate a vector reversely with the number of elements at a particular layer, i.e.
        // if the topmost layer has 4 bubbles, then it goes 4, 3, 2, 1.
        elements_at_layer.clear();
        while (num_elements_in_top_row > 1){
                elements_at_layer.push_back(num_elements_in_top_row);
                num_elements_in_top_row--;
        }// end of while

        // Determine whether the number of rows that match are greater than the number of possible layers
        if (num_rows_matched > (elements_at_layer.size()+1)){
                elements_to_remove = 0;
        }

        // Calculate how many elements are left in the vector should one, two, etc, layers
        // match
        for (auto counter = num_rows_matched; counter < elements_at_layer.size(); counter++){
                elements_to_remove += elements_at_layer.at(counter);
        }// end of for loop

        // Override the number of elements to remove only if the number of layers is equal
        // to the number of layers that match - we don't want to match this as this function
        // will never be called should all layers match, still this is just for completeness'
        // sake.
        if ((elements_at_layer.size()+1) == num_rows_matched){
                elements_to_remove = 0;
        }// end of if

        return elements_to_remove;
}// end of num_elements_to_remove

std::vector<int> GeneticAlgorithm::remove_elements(std::vector<int> chart, int num_elements_remove){
        // Variable Declarations
        static std::vector<int> to_return;

        // Account for all possibilities and edge cases.
        // Should the num_elements_remove be > 0 && < chart.size, it's okay.
        // Should num_elements_remove > chart.size(), not okay
        // Should num_elements remove < 0, not okay.
        to_return.clear();
        if ((num_elements_remove < 0) || (num_elements_remove >= chart.size())) {
                return to_return;
        }// end of if

        //Copy the elements
        for (auto counter = 0; counter < num_elements_remove; counter++){
                to_return.push_back(chart.at(counter));
        }// end of for loop

        chart.clear();
        return to_return;
}//end of remove_elements
