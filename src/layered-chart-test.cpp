/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * layered-chart-test.cpp
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

#include <iostream>

#include "layered-chart-test.h"

//PUBLIC
LayeredChartTest::LayeredChartTest(std::vector<int> chart_to_test,
                                   int total_number_bubbles,
                                   int total_top_row_bubbles, bool unique_numbers)
  : m_layered_chart(chart_to_test), m_total_bubbles(total_number_bubbles),
    m_top_row_bubbles(total_top_row_bubbles){
        //Set basic parameters for the genetic algorithm to use
                // This will contain the number of layers that were successful in the chart vector
                m_number_of_successes = 0;

                // This contains the number of expected layers
                auto layer_counter = total_top_row_bubbles;
                m_number_of_layers = 0;
                while(layer_counter > 1){
                        m_number_of_layers ++;
                        layer_counter--;
                }//end of while

                //Set the variable that will determine whether unique numbers have to be used
                m_unique_numbers = unique_numbers;
}//end of Constructor

bool LayeredChartTest::test_layered_chart(){
        //Variables
        bool con_to_return = false;

        //Determine whether all the elements are within the list
        if (value_within_range(get_total_bubbles(), 1)){
                //std::cout << "Values within range" << std::endl;
                con_to_return = true;
        } else{
                //std::cerr << "Values not within range" << std::endl;
                con_to_return = false;
                return con_to_return;
        }// end of if else

        // Should the test require unique numbers
        if (m_unique_numbers){
                if (singleton_values()) {
                        //std::cout << "Values are unique" << std::endl;
                        con_to_return = true;
                } else{
                        //std::cerr << "Values used are not unique" << std::endl;
                        con_to_return = false;
                        return con_to_return;
                }// end of if
        }// end of if

        //Test whether the chart is correctly made
        con_to_return = recursive_test(get_layered_chart(), 0, get_total_top_bubbles());

        return con_to_return;
}// end of test_layered_chart

double LayeredChartTest::get_genetic_match(){
        return (m_number_of_successes/m_number_of_layers) * 100.0;
}// end of get_genetic_match()


//PROTECTED:
bool LayeredChartTest::recursive_test(std::vector<int> subset_chart_vector, int min_pos, int row_length){
        // Variables
        int local_min_pos;
        int local_max_pos;
        std::vector<int> top_row;
        std::vector<int> row_below;
        std::vector<int> calc_row;

        // Some conditions don't need to be tested right now

        local_min_pos = min_pos;
        local_max_pos = min_pos + row_length-1;               //index starts at 0.
        top_row = create_subset_vector(local_min_pos, local_max_pos);
        local_min_pos = row_length + local_min_pos;
        local_max_pos = local_max_pos + row_length -1;
        row_below = create_subset_vector(local_min_pos, local_max_pos);
        calc_row = calculate_row_below(top_row);

        //Determine the genetic match - how many layers matched
        if(equals(row_below, calc_row)){
                m_number_of_successes++;
        }// end of if

        //Base case to test
        if((row_length-1) <= 1) {  //row length is one, i.e. is at the bottom
                return (equals(row_below, calc_row));
        } else {
                return equals(row_below, calc_row) && recursive_test(subset_chart_vector, local_min_pos, row_length - 1);
        }// end of if
}// end of recursive_test()

std::vector<int> LayeredChartTest::create_subset_vector(int min_pos, int max_pos){
        // Variables
        std::vector<int> to_return;

        // This vector can only find the subset within the current chart, so it has to check
        // whether the max and min values fall within the list
        if (!can_create_subset_vector()){
                std::cerr << "unable to create subset vector" << std::endl;
                return std::vector<int>();
        }// end of if

        //Create the actual subset_chart
        to_return.clear();
        for(auto counter = min_pos; counter <= max_pos; counter++){
                to_return.push_back(get_layered_chart().at(counter));
        }//end of for loop

        return to_return;
}// end of create_subset_vector()

std::vector<int> LayeredChartTest::calculate_row_below(std::vector<int> top_row){
        // Variables
        static std::vector<int> to_return;
        to_return.clear();

        //Calculate the bottom row
        for(auto first = 0, second = first + 1; second < top_row.size(); first++, second++){
                auto result = top_row.at(first) - top_row.at(second);
                auto absolute_subtraction = std::abs(result);

                to_return.push_back(absolute_subtraction);
        }// end of for

        return to_return;
}//end of calculate_row_below()

bool LayeredChartTest::equals(std::vector<int> lhs, std::vector<int> rhs){
        //Variables
        bool to_return = false;

        //Should both be empty
        if ((lhs == std::vector<int>()) && (rhs == std::vector<int>())){
                return true;
        }// end of if

        //Determine whether the vectors have the same length
        if (lhs.size() != rhs.size()){
                return false;
        }// end of if

        //Cycle through the vectors and determine their equality
        for (auto counter = 0; counter < lhs.size(); counter++){
                if (lhs.at(counter) == rhs.at(counter)){
                        to_return = true;
                        continue;
                } else{
                        to_return = false;
                        break;
                }// end of if else
        }// end of for loop

        return to_return;
}// end of equals

bool LayeredChartTest::can_create_subset_vector(int min_pos, int max_pos){
        //Variables
        bool con_to_return = false;

        //See whether the vector's size permits the subset
        if ((min_pos >= 0) && (max_pos >= 0)){
                if ((min_pos < get_layered_chart().size()) && (max_pos < get_layered_chart().size())){
                        con_to_return = true;
                }// end of if
        } //end of if

        return con_to_return;
}// end of can_create_subset_vector()

bool LayeredChartTest::value_within_range(int max_value, int min_value){
        // Variables
        bool con_to_return = false;

        //Cycle through the items in the returned vector
        for(int num: get_layered_chart()){
                //Determine whether the number is within bounds
                if ((num >= min_value) && (num <= max_value)) {
                        con_to_return = true;
                } else {
                        con_to_return = false;
                        break;
                }// end of if
        }// end of for

        return con_to_return;
}// end of value_within_range()

bool LayeredChartTest::singleton_values(){
        // Variables
        bool con_to_return = false;

        // Determine whether every value only appears once in the list.
        for (auto i = 0; i < get_layered_chart().size(); i++){
                for (auto j = 0; j < get_layered_chart().size(); j++){
                        // Make sure to not select the same index
                        if (i != j){
                                // Determine whether any number has been used more than once
                                auto first = get_layered_chart().at(i);
                                auto second = get_layered_chart().at(j);
                                if (first != second){
                                        // Doesn't fail as the numbers are unique
                                        con_to_return = true;
                                }else {
                                        // Does not contain unique numbers - thus it fails
                                        con_to_return = false;
                                        return con_to_return;
                                }// end of if
                        } // end of if
                }// end of for loop
        }// end of for loop

        return con_to_return;
}// end of singleton_values()

int LayeredChartTest::get_total_bubbles() const{
        return m_total_bubbles;
}// end of get_total_bubbles()

int LayeredChartTest::get_total_top_bubbles() const{
        return m_top_row_bubbles;
}// end of get_total_top_bubbles()

std::vector<int> LayeredChartTest::get_layered_chart() const{
        return m_layered_chart;
}// end of get_layered_chart()

int LayeredChartTest::get_num_layers_match() const{
        return m_number_of_successes;
}// endof get_num_layers_match
