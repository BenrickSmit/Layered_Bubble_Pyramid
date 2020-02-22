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

void GeneticAlgorithm::generate_solutions(){
        // Variable
        const int TOTAL_NUMBER_SOLUTIONS = 1000;            // Total number of solutions to generate
        const long long TOTAL_NUMBER_TEST_RUNS = 100000;             // Total number of test-runs before force-closure of program
        long long total_new_solutions_generated = 0;
        long long solution_generator_counter = 0;
        std::vector<std::vector<int>> partial_fit;          // Will contain any data that has partial matches
        std::vector<int> temp_solution;                     // Will contain one potential solution for use with tester

        //Clear the vector of possible solutions to make running it effective
        m_num_charts.clear();

        // Generate potential solutions until a solution has been found
        while((!has_solution())){
                // Should m_num_charts not be empty due to partial_fit setting some values
                // as potential solutions, determine how much needs to be regenerated, and regenerate
                // the data
                if (!m_num_charts.empty()){
                        // Test the potential solutions currently available, and remove the unnecessary parts
                        for (auto counter = 0; counter < m_num_charts.size(); counter ++){
                                temp_solution = m_num_charts.at(counter);
                                std::vector<int> partial_solution;

                                // Determine how many values you should regenerate
                                LayeredChartTest chart_test(temp_solution, get_total_num_chart_elements(), get_topmost_num_elements(), m_use_unique_numbers);
                                int number_elements_to_remove = num_elements_to_remove(get_topmost_num_elements(), chart_test.get_num_layers_match());
                                temp_solution = remove_elements(temp_solution, number_elements_to_remove);
                                partial_solution = generate_chart_data(number_elements_to_remove, get_max_allowed_value(), get_min_allowed_value());

                                // Append the new data to the temporary solution
                                //for(auto number: partial_solution){
                                        //temp_solution.push_back(number);
                                //}// end of for loop
                                temp_solution.insert(temp_solution.end(), partial_solution.begin(), partial_solution.end());

                                //Reinsert the element at the current locus for later testing
                                m_num_charts[counter] = temp_solution;
                                total_new_solutions_generated++;
                        }// end of for loop
                }// end of if

                // Generate solutions until they match TOTAL_NUMBER_SOLUTIONS, should partial data
                // have left any solutions  in m_num_chart generate only the number you need to have
                // TOTAL_NUMBER_SOLUTIONS
                for(auto counter = m_num_charts.size(); counter < TOTAL_NUMBER_SOLUTIONS; counter++){
                        // Generate a possible solution and add it to m_num_charts
                        m_num_charts.push_back(generate_chart_data(get_total_num_chart_elements(), get_max_allowed_value(), get_min_allowed_value()));
                        total_new_solutions_generated++;
                }// end of for loop

                // Test Solutions adding any with a partial fit to partial_fit.
                partial_fit.clear();
                for (auto counter = 0; counter < TOTAL_NUMBER_SOLUTIONS; counter++){
                        // Retrieve a potential solution and test it's fitness
                        temp_solution = m_num_charts.at(counter);
                        LayeredChartTest chart_test(temp_solution, get_total_num_chart_elements(), get_topmost_num_elements(), m_use_unique_numbers);

                        //Determine whether there is a complete match, or only partial match
                        if (chart_test.test_layered_chart()){
                                set_has_solution(true);
                                partial_fit.push_back(temp_solution);
                        } else{   // Not a complete match
                                //Determine how whether one or more layers match
                                if (chart_test.get_num_layers_match() > 0){
                                        partial_fit.push_back(temp_solution);
                                }//end of if
                        }// end of if
                }// end of for loop

                // Remove m_num_charts' elements and assign partial_fit to m_num_charts
                m_num_charts.clear();
                m_num_charts = partial_fit;
                partial_fit.clear();



                // Determine if the number of generations has run its countse
                if (solution_generator_counter >= TOTAL_NUMBER_TEST_RUNS){
                        break;
                }// end of if
                // Increment the total number of while-loop runs
                solution_generator_counter++;
        }// end of while loop

        // Determine which condition was met
        if(TOTAL_NUMBER_TEST_RUNS == solution_generator_counter) {
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
        } else if (has_solution()){
                std::cout << "Solution Found!" << std::endl;
                std::cout << "Found Within " << total_new_solutions_generated << " Generations" << std::endl;
        }// end of if else
}// end of generate_solutions()

void GeneticAlgorithm::trim_solutions(){
        // Find the first solution out of the multitude that fits, then clear the rest
        static std::vector<int> temp_chart;
        for (auto counter = 0; counter < m_num_charts.size(); counter++){
                temp_chart = m_num_charts.at(counter);
                LayeredChartTest test_chart(temp_chart, get_total_num_chart_elements(), get_topmost_num_elements());

                if(test_chart.test_layered_chart()){
                        break;
                }//end of if
        }// end of for

        m_num_charts.clear();
        m_layered_chart = temp_chart;
}// end of trim_solutions();

void GeneticAlgorithm::display_solution() const{
        // Variables
        int element_counter = 0;
        int new_line_location = get_topmost_num_elements();
        int counter = 0;


        //Output the solution in the form of a pyramid
        while ((element_counter < m_layered_chart.size())){   // Display only elements in the list
            std::cout << m_layered_chart.at(element_counter) << " ";

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
                auto random_number = (rand() % max_value) + min_value;
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

        //Copy only the elements that matched
        for (auto counter = 0; counter < (chart.size() - num_elements_remove); counter++){
                to_return.push_back(chart.at(counter));
        }// end of for loop

        chart.clear();
        return to_return;
}//end of remove_elements
