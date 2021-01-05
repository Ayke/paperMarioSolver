# PaperMario Battle Puzzle Solver
This program solves papermario puzzles by a simple BFS.


The playboard is mapped into a 12x4 matrix, thus each position is mapped into a pair of number.
The first number in the pair represents its position in the clockwise order, 
the second number in the pair represents how close it is to the circle center.


![Position mapping](https://user-images.githubusercontent.com/4937997/103705278-9da11880-4f5f-11eb-849b-e3a221682d31.png)


In the above picture, the input should be:


2 1


3 0


4 1


4 2


4 3


5 0


7 1


8 0

