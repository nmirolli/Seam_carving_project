/* #############################################################################
 #
 #  ClastrStream:      Algorithms
 #  Project:           3
 #  File:              README.txt
 #  Author:            Nicholas Mirolli
 #  Email:             ndm28@zips.uakron.edu
 #
 ###############################################################################

 ###############################################################################
 # RUNNING THE PROGRAM
 ###############################################################################

 1) First download the zip file.  This project was written using Sublime Text 2
 		and compiled using g++ through the command line.  All the code for this
 		project is contained in main.cpp to make it easier to compile and not 
 		require a makefile.

 2) To run the project, first compile it to make sure the latest save of
 		main.cpp is built into the project.  Compile the project using the
 		following command:
 			g++ main.cpp

 3) To run the program, make sure that your test cases are saved into the same 
 		directory as the executable.  In this case I have supplied some basic
 		test cases with the zip file so just put any extra test cases into the
 		extracted folder.  By compiling the program using the command above
 		the executable a.exe is created.  To run the program use the following
 		command:

 			.\a testcase.pgm horizontalSeams verticalSeams
 	
 		where a is the executable name, testcase.pgm is the testcase file, 
 		(REMEMBER TO INCLUDE THE .PGM EXTENSION WHEN YOU INPUT THE FILENAME)
 		horizontalSeams is the integer value for the # of horizontal seams
 		and verticalSeams is the integer value for the # of vertical seams.

 4) When the program finishes running the testcase_processed.pgm file can be
		found in the same directory where you placed the testcase.pgm file.

 ###############################################################################
 # EXPLAINATION OF THE SOURCE CODE
 ###############################################################################

 void calcEnergy(int**, int**, int, int) {}
 		This function deals with the calculation of the energy of each pixel in
 		the .pgm file by comparing it to each of the adjacent pixels.  The 
 		function to calculate the energy is as follows:
 			e(i,j) = |v(i,j)-v(i-1,j)|+ |v(i,j)-v(i+1,j)|+ |v(i,j)-v(i,j-1)|+ |v(i,j)-v(i,j+1)|
 		except for the special cases where one of the elements that will be 
 		compared is out of bounds.
 		This function accepts a pointer to the greyscale array, a pointer 
 		to the energy array, the number of rows, and the number of columns.

 void cumulativeVert(int**, int**, int, int) {}
 		This function deals with the calculation of the cumulative energy of a
 		pixel based on the energies of the pixels above it. The function to 
 		calculate the cumulative vertical energy is as follows:
 			ceArray[i][j] = eArray[i][j] + min(min(ceArray[i-1][j-1], ceArray[i-1][j]), ceArray[i-1][j+1]); 
 		except for the special cases where one of the elements that will be
 		compared is out of bounds.
 		This function accepts a pointer to the energy array, a pointer to the 
 		cumulative energy array, the number of rows, and the number of columns.

 void cumulativeHoriz(int**, int**, int, int) {}
 		This function deals with the calculation of the cumulative energy of a
 		pixel based on the energies of the pixels to the left of it.  The 
 		function to calculate the cumulative horizontal energy is as follows:
 			ceArray[i][j] = eArray[i][j] + min(min(ceArray[i-1][j-1], ceArray[i][j-1]), ceArray[i+1][j-1]);
 		except for the special cases where one of the elements that will be 
 		compared is out of bounds.
 		This function accepts a pointer to the energy array, a pointer to the 
 		cumulative energy arrya, the number of rows, and the number of columns.

 void vertSeam(int**, int**, int, int) {}
 		This function deals with finding the vertical path of least energy 
 		through the image using the cumulative vertical energy array.  The 
 		function to calculate the vertical seam finds the lowest cumulative
 		energy pixel and then traverses the rows to find the next adjacent
 		lowest energy pixel.  Then the function deletes that seam and shifts
 		the pixels in the original image.
 		This function accepts a pointer to the greyscale array, a pointer to 
 		the cumulative energy array, the number of rows, and the number of 
 		columns. 

 void horizSeam(int**, int**, int, int) {}
 		This function deals with finding the horizontal path of least energy 
 		through the image using the cumulative horizontal energy array.  The
 		function to calculate the horizontal seam finds the lowest cumulative
 		energy pixel and then traverses the columns to find the next adjacent
 		lowest energy pixel.  Then the function deletes that seam and shifts 
 		the pixels in the original image.
 		This function accepts a pointer to the greyscale array, a pointer to
 		the cumulative energy arrya, the number of rows, and the number of 
 		columns.

 void printOutFile(int**, int, int, string) {}
 		This function writes the new image to a file with the name _processed
 		appended to the end of the original filename.  The processed file is 
 		a .pgm file just like the original but has the number of vertical and 
 		horizontal seams removed specified when the program was run.  
 		This function accepts a pointer to the greyscale array, the number of
 		rows, the number of columns, and the original file name.

 int main(int argc, char** argv) {}
 		This is the main function of the program and deals with the allocation
 		and releasing of memory used in the program.  The main function also
 		has two loops that represent the number of vertical and horizontal 
 		seams that need to be removed from the original image.  
 		This function accepts the number of arguments provided at runtime and 
 		a pointer to those arguments.  It then uses the pointer to obtain the 
 		filename of the image, the number of horizontal seams to remove, and 
 		the number of vertical seams to remove.  

 ###############################################################################
 # INTERESTING THINGS ABOUT THIS PROGRAM
 ###############################################################################

 I have never programmed anything to deal with images or image manipulation.
 		This is my first experience with anything image related but it is a
 		very interesting topic and I believe I will reference this code in the
 		future when dealing with images.