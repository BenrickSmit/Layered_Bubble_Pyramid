# LAYERED PYRAMID:
This program provides solutions to the triangular array where each number in a subsequent row is the absolute difference of the two numbers immediately behind it, except for the backmost(topmost) row.

The program can find solutions for most of the problems, should you give it enough time, and should you allow non-unique numbers. As normal, the smaller the array, the quicker it is to solve. Luck also plays a big part in solving the puzzle with non-unique numbers.

Should, however, the solution require unique numbers, it is possible to find all the solutions until where the backmost row is of number 5, however, should the backmost row be any bigger, it is nigh impossible to find a solution.

To allow for unique numbers only, simply change the <code>bool unique_numbers_only = false</code> to <code>bool unique_numbers_only = true</code>

# SCREENSHOTS
Please find some screenshots of the results:

1) [2 backmost (unique numbers)](screenshots/2_backmost_row.png)
2) [3 backmost (unique numbers)](screenshots/3_backmost_row.png)
3) [4 backmost (non-unique numbers)](screenshots/4_backmost_row.png)
4) [5 backmost (unique numbers)](screenshots/5_backmost_row.png)

# LICENSE
This program is published under the [GPLv3](COPYING) License.
