/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.cpp
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

#include "genetic-algorithm.h"
#include "recursive-algorithm.h"
#include "layered-chart-test.h"


// PROTOTYPE FUNCTIONS
// This function will determine whether the input is between min_num and max_num
// and will return a boolean value depicting this condition
bool number_validation(int user_input, int min_num, int max_num);

// This function will determine whether to exit the program or not, based on
// The input arguments. it receives two numbers, one the user inputs, the other
// Is programmatically set to show the exit.
void exit_program(int user_number, int exit_choice);

// This function will display the bubble options to the user who can then choose
// Which they would prefer
int bubble_choice();

// This funciton will display the algorithm options to the user who can then choose
// which they would prefer
int algorithm_choice();

// This function will recursively return the factorial value of an input number,
// instead of multiplying the numbers you add them. i.e. with 2, you get 2 + 1 = 3.
// with 3 you get 3 + 2 + 1 = 6.
int factorial_derivative(int value);

// This function states why solution to the layered pyramid - if you're using unique numbers -
// cannot be found for anything where the top number of bubbles is > 5
void reason_for_failure();

//Main loop of the program
int main(){
        // EXPECTED RESULTS:
        //There are a limited number of unique solutions
        // 2 backrow = 4 unique solutions (option 1)          -> Done(Recursive, Genetic)
        // 3 backrow = 8 unique solutions (option 2)          -> Done(Recursive, Genetic)
        // 4 backrow = 8 unique solutions (option 3)          -> Done(Recursive, )
        // 5 backrow = 2 unique solutions (option 4)          -> Done(Recursive, )
        // 6 backrow = 0 unique solutions (option 5) - Error  -> Done(Recursive, )

	//Program Varaibles
	bool unique_numbers_only = true;
	std::vector<int> num_bubbles;
	std::vector<int> num_top_row_bubbles;
	int number_of_bubbles_option = 0;
	int algorithm_of_choice_option = 0;

	//Set up the number of bubbles required for the vector
	num_bubbles.clear();
	num_bubbles.push_back(factorial_derivative(2));
	num_bubbles.push_back(factorial_derivative(3));
	num_bubbles.push_back(factorial_derivative(4));
	num_bubbles.push_back(factorial_derivative(5));
	num_bubbles.push_back(factorial_derivative(6));
	num_bubbles.push_back(factorial_derivative(7));
	num_bubbles.push_back(factorial_derivative(8));

	//Set the number of bubbles found in the top row of the chart
	num_top_row_bubbles.clear();
	num_top_row_bubbles.push_back(2);
	num_top_row_bubbles.push_back(3);
	num_top_row_bubbles.push_back(4);
	num_top_row_bubbles.push_back(5);
	num_top_row_bubbles.push_back(6);
	num_top_row_bubbles.push_back(7);
	num_top_row_bubbles.push_back(8);

	// PROGRAM START
	// Determine the nubmer of elements the user wants to test
	// Number of bubbles choice (Can be exit)
		number_of_bubbles_option = bubble_choice();
		exit_program(number_of_bubbles_option, 6);    //Option six closes the program
	// Algorithm Used Choice  (Can be exit)
		algorithm_of_choice_option = algorithm_choice();
		exit_program(algorithm_of_choice_option, 3);  //Option 3 closes the program

	// Determine Which algorithm to use (Can be exit)
		// num_bubbles corresponds to Bubble Option list - 1
		number_of_bubbles_option--;
		// If choice is exit, this code won't execute
		if(algorithm_of_choice_option == 1){ //Genetic Algorithm
			GeneticAlgorithm ga(num_bubbles.at(number_of_bubbles_option),
					    num_top_row_bubbles.at(number_of_bubbles_option),
					    unique_numbers_only);   // no unique numbers

			// Generate the solutions
			ga.generate_solutions();

			// Should there be a solution; display it
			if (ga.has_solution()){
				//ga.trim_solutions();
				ga.display_solution();
			} else{
				if (num_top_row_bubbles.at(number_of_bubbles_option) > 5){
					reason_for_failure();
				}//end of if
			} //end of if else
		} else { //Recursive Algorithm
			RecursiveAlgorithm ra(num_bubbles.at(number_of_bubbles_option),
					      num_top_row_bubbles.at(number_of_bubbles_option),
					      unique_numbers_only);	// no unique numbers

			// Generate the solutions
			ra.generate_solutions();

			// Should there be a solution; display it
			if (ra.has_solution()){
//				//ra.trim_solutions();
				ra.display_solution();
			} else{
				if (num_top_row_bubbles.at(number_of_bubbles_option) > 5){
					reason_for_failure();
				}//end of if
			} //end of if else
		}// end of if else
	// PROGRAM END
	
	// Official End
	return 0;
}//end of main


// FUNCTION DEFINITIONS
int algorithm_choice(){
	//variable to return
	int num_to_return = 0;
	int chosen_option = 0;
	int min_options = 1;	    //Value of the lowest option
	int max_options = 3;	    //Value of the highest option

	//Display the algorithm Options
	std::cout << "" << std::endl;
	std::cout << "Please choose the algorithm:" << std::endl;
	std::cout << "(Note: numbers only, words not supported)" << std::endl;
	std::cout << "===========================================" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Option 1) Genetic Algorithm" << std::endl;
	std::cout << "Option 2) Recursive Algorithm" << std::endl;
	std::cout << "Option 3) EXIT" << std::endl;
	std::cout << "" << std::endl;

        std::cout << "Your Choice: Option ";
        std::cin >> chosen_option;

        //Validate the input
        if (number_validation(chosen_option, min_options, max_options)){
                num_to_return = chosen_option;
        }else{  //Not within bounds, so choose the exit option
                std::cout << "\nInvalid Option. Defaulting to EXIT option" << std::endl;
                num_to_return = max_options;
        }//end of if else

	return num_to_return;
}// end of algorithm_choice

int bubble_choice(){
        //variable to return
        int num_to_return = 0;
        int chosen_option = 0;
        int max_options = 6;          // Value of the lowest option
        int min_options = 1;          // Value of the highest option

        //Display the algorithm Options
        std::cout << "" << std::endl;
        std::cout << "Please choose the number of bubbles:" << std::endl;
        std::cout << "(Note: numbers only, words not supported)" << std::endl;
        std::cout << "===========================================" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Option 1) 3 Bubbles using numbers (1-3)" << std::endl;
        std::cout << "Option 2) 6 Bubbles using numbers (1-6)" << std::endl;
        std::cout << "Option 3) 10 Bubbles using numbers (1-10)" << std::endl;
        std::cout << "Option 4) 15 Bubbles using numbers (1-15)" << std::endl;
        std::cout << "Option 5) 21 Bubbles using numbers (1-21)" << std::endl;
        std::cout << "Option 6) EXIT" << std::endl;
        std::cout << "NOTE: The more bubbles there are, the longer it will take..." << std::endl;
        std::cout << "" << std::endl;

        std::cout << "Your Choice: Option ";
        std::cin >> chosen_option;

        //Validate the input
        if (number_validation(chosen_option, min_options, max_options)){
                num_to_return = chosen_option;
        }else{  //Not within bounds, so choose the exit option
                std::cout << "\nInvalid Option. Defaulting to EXIT option" << std::endl;
                num_to_return = max_options;
        }//end of if else

        return num_to_return;
}// end of bubble_choice

bool number_validation(int user_input, int min_num, int max_num){
	//Variables
	bool con_to_return = false;

	//Determine whether the input number is between the max and min value
	if((user_input >= min_num) && (user_input <= max_num)){
		con_to_return = true;
	}//end of if

	return con_to_return;
}// end of number_validation

void exit_program(int user_number, int exit_choice){
	//Determine whether the exit code is the same as the user's input
	if(user_number == exit_choice){
		exit(0);
	}//end of if
}// end of exit_program

int factorial_derivative(int value){
	if(value > 1){
		//Normal condition until you've reached 1
		return value + factorial_derivative(value-1);
	} else {
		//The Base Case
		return (1);
	}// end of if
}// end of factorial

void reason_for_failure(){
	std::cout << "Reason for failure:\n======================" << std::endl;
	std::cout << "Exact Difference Triangles are triangles that can be made" << std::endl;
	std::cout << "should a number be a triangular number, i.e. ((k/(k+1))2) = n" << std::endl;
	std::cout << "where k is the total number of values in the backmost row of the " << std::endl;
	std::cout << "triangle and n is the total number of values available." << std::endl;
	std::cout << "" << std::endl;
	std::cout << "Exact Difference Triangles are only possible where k <= 5" << std::endl;
	std::cout << "as the rule for a solution generally holds when n % 4 == 0 or n % 4 == 3." << std::endl;
	std::cout << "Should the rule hold you can expect at least four unique" << std::endl;
	std::cout << "solutions." << std::endl;
	std::cout << "Where k > 5, there are no unique solutions, but there are" << std::endl;
	std::cout << "non-unique solutions." << std::endl;
	std::cout << "" << std::endl;
}// end of reason_for_failure
