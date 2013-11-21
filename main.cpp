/* ##########################################################################
 #
 #  ClastrStream:      Algorithms
 #  Project:           3
 #  File:              main.cpp
 #  Author:            Nicholas Mirolli
 #  Email:             ndm28@zips.uakron.edu
 #
 ############################################################################*/ 

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void calcEnergy(int**, int**, int, int);
void cumulativeVert(int**, int**, int, int);
void cumulativeHoriz(int**, int**, int, int);
void vertSeam(int**, int**, int, int);
void horizSeam(int**, int**, int, int);
void printOutFile(int**, int, int, string);

int main(int argc, char** argv) {
    if (argc != 4) {
        cout << "Invalid number of arguments provided." << endl;
    }
    int vSeams = atoi(argv[3]);
    int hSeams = atoi(argv[2]);
    string filename;
    filename = argv[1];
    cout << "filename: " << filename << endl;
    string outFilename = filename.substr(0, filename.size()-4);
    
    int rows = 0, columns = 0, greyScaleMax = 255;
    ifstream infile(filename.c_str());
    stringstream strStream;
    string input = "";
    
    // Get the first line of the .pgm file (version)
    getline(infile,input);
    // Get the second line of the .pgm file (comment)
    getline(infile,input);
    cout << "Comment : " << input << endl;
    // Continue with a stringstream
    strStream << infile.rdbuf();
    // Get the third line of the .pgm file (columns and rows)
    strStream >> columns >> rows;
    cout << columns << " columns and " << rows << " rows" << endl;
    // Get the fourth line of the .pgm file (maximum greyscale value)
    strStream >> greyScaleMax;
    cout << "greyScaleMax = " << greyScaleMax << endl;
    
    // GreyScale Array used to store the greyscale values of the original image
    int** gsArray = new int*[rows];
    for (int i=0; i<rows; ++i) {
        gsArray[i] = new int [columns];
    }
    
    //Populate gsArray with pgm values
    for(int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            strStream >> gsArray[r][c];
        }
    }
    
    // close input file for efficiency
    infile.close();
    
    //Carry our the vertical seam operation while vSeams is greater than zero
    for(int i=0; i < vSeams; i++) {
        cout << "vertical seams to remove: " << vSeams-i << endl;
        //create new array to store energy in
        int** eArray = new int*[rows];
        for (int i=0; i<rows; i++) {
            eArray[i] = new int[columns];
        }

        //Used to store the cumulative minimum energy for vertical seams
        int** ceArray = new int*[rows];
        for (int j=0; j<rows; j++) {
            ceArray[j] = new int[columns];
        }

        //calculate the energy array
        calcEnergy(gsArray, eArray, rows, columns);
        //calculate the cumulative vertical energies
        cumulativeVert(eArray, ceArray, rows, columns);
        //flag positions of values to be deleted
        vertSeam(gsArray, ceArray, rows, columns);
        //decrement the column number
        columns--;

        //delete memory associated with ceArray and eArray
        for (int k=0; k<rows; k++) {
           delete[] ceArray[k];
           delete[] eArray[k];
        }
        delete[]ceArray;
        delete[]eArray;
    } 
    
    //Carry out the horizontal seam operations while hSeams is greater than zero
    for(int i=0; i < hSeams; i++) {
        cout << "horizontal seams to remove: " << hSeams-i << endl;
        //create new array to store energy in
        int** eArray = new int*[rows];
        for (int i=0; i<rows; ++i) {
            eArray[i] = new int[columns];
        }
        
        //Used to store the cumulative minimum energy for vertical seams
        int** ceArray = new int*[rows];
        for (int j=0; j<rows; ++j) {
            ceArray[j] = new int[columns];
        }
        
        //calculate the energy array
        calcEnergy(gsArray, eArray, rows, columns);
        //calculate the cumulative horizontal energies
        cumulativeHoriz(eArray, ceArray, rows, columns);
        //flag positions of values to be deleted
        horizSeam(gsArray, ceArray, rows, columns);
        //decrement the row number
        rows--;      
        
        //delete memory associated with ceArray and eArray
        for (int k=0; k<rows; k++) {
           delete[] ceArray[k];
           delete[] eArray[k];
        }
        delete[]ceArray;
        delete[]eArray;
    } 
    cout << columns << " columns and " << rows << " rows" << endl;
    
    //write gsArray to a file
    printOutFile(gsArray, rows, columns, outFilename);
    
    //delete memory associated with gsArray
    for (int k=0; k<rows; k++) {
           delete[] gsArray[k];
    }
    delete[]gsArray;
    
    return 0;
}

void calcEnergy(int** gsArray, int** eArray, int rows, int columns) {
    //loops through rows
    for(int i=0; i<rows; i++) {
        for(int j=0; j<columns; j++) {
            //loops through columns
            //if i==0 then dont compare the to the left
            //if i==number of rows then don't compare the bottom
            //if j==0 dont compare left
            //if j==columns dont compare right
            
            //up and left
            if(i==0 && j==0) {
                //only do right and down
                eArray[i][j] = (abs((gsArray[i][j])-(gsArray[i+1][j])) + 
                                   abs((gsArray[i][j])-(gsArray[i][j+1])));
                
            }
            //up and right
            else if((i==0) && (j==(columns-1))){
                //only do left and down
                eArray[i][j] = (abs((gsArray[i][j])-(gsArray[i][j-1])) + 
                                   abs((gsArray[i][j])-(gsArray[i+1][j])));
            }
            //down and left
            else if((i==(rows-1)) && (j==0)) {
                //only do up and right
                eArray[i][j] = (abs((gsArray[i][j])-(gsArray[i-1][j])) + 
                                   abs((gsArray[i][j])-(gsArray[i][j+1])));
            }
            //down and right
            else if((i==(rows-1)) && (j==(columns-1))){
                //only do up and left
                eArray[i][j] = (abs((gsArray[i][j])-(gsArray[i-1][j])) + 
                                   abs((gsArray[i][j])-(gsArray[i][j-1])));
            }
            //for top row
            else if(i==0) {
                //right, left and bottom
                eArray[i][j] = (abs((gsArray[i][j])-(gsArray[i][j+1])) + 
                                   abs((gsArray[i][j])-(gsArray[i][j-1])) + 
                                   abs((gsArray[i][j])-(gsArray[i+1][j])));
            }
            //for bottom row
            else if(i==(rows-1)){
                //top, left and right
                eArray[i][j] = (abs((gsArray[i][j])-(gsArray[i][j+1])) + 
                                   abs((gsArray[i][j])-(gsArray[i][j-1])) + 
                                   abs((gsArray[i][j])-(gsArray[i-1][j])));
            } 
            //for first column
            else if(j==0) {
                //right, top and bottom
                eArray[i][j] = (abs((gsArray[i][j])-(gsArray[i][j+1])) + 
                                   abs((gsArray[i][j])-(gsArray[i-1][j])) + 
                                   abs((gsArray[i][j])-(gsArray[i+1][j])));
            }
            else if(j==(columns-1)){
                //left, top and bottom
                eArray[i][j] = (abs((gsArray[i][j])-(gsArray[i][j-1])) + 
                                   abs((gsArray[i][j])-(gsArray[i+1][j])) + 
                                   abs((gsArray[i][j])-(gsArray[i-1][j])));
            } else {
                //e(i,j) = |v(i,j)-v(i-1,j)|+ |v(i,j)-v(i+1,j)|+ |v(i,j)-v(i,j-1)|+ |v(i,j)-v(i,j+1)|
                eArray[i][j] = (abs((gsArray[i][j])-(gsArray[i-1][j])) + 
                                   abs((gsArray[i][j])-(gsArray[i+1][j])) +
                                   abs((gsArray[i][j])-(gsArray[i][j-1])) +
                                   abs((gsArray[i][j])-(gsArray[i][j+1])));
            }
        }
    }
}

void cumulativeVert(int** eArray, int** ceArray, int rows, int columns) {
    //loop through the rows
    for(int i=0; i<rows; i++) {
        //loop through the columns
        for(int j=0; j<columns; j++) {
            //do this only for the first row
            if(i==0){
                //copy over values
                ceArray[i][j] = eArray[i][j];
            } else {
                //first column
                if(j==0) {
                    //ignore out of bounds
                    ceArray[i][j] = eArray[i][j] + min(ceArray[i-1][j], ceArray[i-1][j+1]);            
                } else if(j==(columns-1)) {
                    //ignore out of bounds
                    ceArray[i][j] = eArray[i][j] + min(ceArray[i-1][j-1], ceArray[i-1][j]);            
                } else {
                    //find the min energy to add to the running total
                    ceArray[i][j] = eArray[i][j] + min(min(ceArray[i-1][j-1], ceArray[i-1][j]), ceArray[i-1][j+1]);                
                }
            }
        }
    } 
}

void cumulativeHoriz(int** eArray, int** ceArray, int rows, int columns) {
    //loop through the columns
    for(int j=0; j<columns; j++) {
        //loop through the rows
        for(int i=0; i<rows; i++) {
            //do this only for the first column
            if(j==0){
                //copy over values
                ceArray[i][j] = eArray[i][j];
            } else {
                //top row
                if(i==0) {
                    //ignore out of bounds
                    ceArray[i][j] = eArray[i][j] + min(ceArray[i][j-1], ceArray[i+1][j-1]);            
                } else if(i==(rows-1)) {
                    //ignore out of bounds
                    ceArray[i][j] = eArray[i][j] + min(ceArray[i][j-1], ceArray[i-1][j-1]);            
                } else {
                    //find the min energy to add to the running total
                    ceArray[i][j] = eArray[i][j] + min(min(ceArray[i-1][j-1], ceArray[i][j-1]), ceArray[i+1][j-1]);                
                }   
            }
        }
    } 
}

void vertSeam(int** gsArray, int** ceArray, int rows, int columns) {
    int mIndex = 0;
    //set minimum to the first element in the last row
    int min = ceArray[rows-1][mIndex];
    
    //loop through last row and find index of the minimum
    for(int j=0; j<columns; j++) {
        if(ceArray[rows-1][j] <= min) {
            mIndex = j;
            min = ceArray[rows-1][j];
        }
    }
    
    //delete element and shift
    for(int it = mIndex; it < (columns-1);it++){
        //copy the left element of the last row
        gsArray[rows-1][it] = gsArray[rows-1][it+1];
    }

    //loop through remaining rows
    for(int i=(rows-2); i>=0;i--){
        //base case for edge rows
        if(mIndex==0){
            //check the minium of directly above or to the right
            if(ceArray[i][mIndex] < ceArray[i][mIndex+1]){
                //return above
                mIndex = mIndex;
            } else if(ceArray[i][mIndex+1] < ceArray[i][mIndex]){
                //return right
                mIndex = mIndex+1;
            }
        }
        // right edge
        else if(mIndex==(columns-1)){
            //only check the minium of directly above or to the right
            if(ceArray[i][mIndex] < ceArray[i][mIndex-1]){
                //return above
                mIndex = mIndex;
            } else if(ceArray[i][mIndex-1] < ceArray[i][mIndex]){
                //return left
                mIndex = mIndex-1;
            }
        }
        //not first or last column case
        // find min of above left and right
        else {
            // check middle as min
            if((ceArray[i][mIndex] < ceArray[i][mIndex-1]) && (ceArray[i][mIndex] < ceArray[i][mIndex+1])){
                //return middle
                mIndex = mIndex;
            } // check the right as min
            else if((ceArray[i][mIndex+1] < ceArray[i][mIndex]) && (ceArray[i][mIndex+1] < ceArray[i][mIndex-1])){
                //return right
                mIndex = mIndex+1;
            } // check left as min
            else if((ceArray[i][mIndex-1] < ceArray[i][mIndex]) && (ceArray[i][mIndex-1] < ceArray[i][mIndex+1])){
                //return left
                mIndex = mIndex-1;
            } // if both left and right are equal and less than middle
            else if((ceArray[i][mIndex-1] < ceArray[i][mIndex]) && (ceArray[i][mIndex+1] < ceArray[i][mIndex]) && (ceArray[i][mIndex+1] == ceArray[i][mIndex-1])){
                // by default return the left
                mIndex = mIndex-1;
            }
        }

        // delete mIndex value and shift all the values over
        for(int it = mIndex; it < (columns-1);it++){
            //copy over the left element
            gsArray[i][it] = gsArray[i][it+1];
        }
    }
}

void horizSeam(int** gsArray, int** ceArray, int rows, int columns) {
    int mIndex = columns-1;
    int min = 99999999;
    
    //loop through last col and find index of the minimum
    for(int i=0; i<rows; i++) {
        if(ceArray[i][columns-1] < min) {
            mIndex = i;
            min = ceArray[i][columns-1];  
        }
    }
            
    for(int it = mIndex; it < (rows-1);it++){
        //copy the left element of the last row
        gsArray[it][columns-1] = gsArray[it+1][columns-1];
    }
    
    //loop through the remaining columns
    for(int j=(columns-2); j>=0;j--){
        //base case for first or last row
        //first row
        if(mIndex==0){
            //only check the minium of directly left and to the bottom
            //check left as min
            if(ceArray[mIndex][j] < ceArray[mIndex+1][j]){
                //return left
                mIndex = mIndex;
            } else if(ceArray[mIndex+1][j] < ceArray[mIndex][j]){
                //return bottom
                mIndex = mIndex+1;
            }
        }
        // right
        else if(mIndex==(rows-1)){
            //check left as min
            if(ceArray[mIndex][j] < ceArray[mIndex-1][j]){
                //return left
                mIndex = mIndex;
            } else if(ceArray[mIndex-1][j] < ceArray[mIndex][j]){
                //return top
                mIndex = mIndex-1;
            }
        }
        //not first or last row
        // find min of top left and bottom
        else {
            // check middle as min
            if((ceArray[mIndex][j] < ceArray[mIndex-1][j]) && (ceArray[mIndex][j] < ceArray[mIndex+1][j])){
                //return left
                mIndex = mIndex;
            } // check the top as min
            else if((ceArray[mIndex-1][j] < ceArray[mIndex][j]) && (ceArray[mIndex-1][j] < ceArray[mIndex+1][j])){
                //return top
                mIndex = mIndex-1;
            } // check bottom as min
            else if((ceArray[mIndex+1][j] < ceArray[mIndex][j]) && (ceArray[mIndex+1][j] < ceArray[mIndex-1][j])){
                //return bottom
                mIndex = mIndex+1;
            }
            // if both top and bottom are equal and lestrStream than left
            else if((ceArray[mIndex+1][j] < ceArray[mIndex][j]) && (ceArray[mIndex-1][j] < ceArray[mIndex][j]) && (ceArray[mIndex+1][j] == ceArray[mIndex-1][j])){
                // by default return the top
                mIndex = mIndex-1;
            }
        }
        // delete mIndex value and shift
        for(int it = mIndex; it < (rows-1);it++){
            //copy over the left element
            gsArray[it][j] = gsArray[it+1][j];
        }
    }
}

//Writes the seam carved image to a .pgm file for viewing
void printOutFile(int** array, int rows, int columns, string outFileName) {
    ofstream outFile;
    outFileName.append("_processed.pgm");
    outFile.open(outFileName.c_str());
    if (!outFile.is_open()) {
        cout << "Can't open output file"  << outFileName << endl;
        return;
    }
 
    // write the header of the pgm file 
    outFile << "P2\n" << "#Output file for " << outFileName << "\n" << columns << " " << rows << "\n255\n";
    
    //writes the contents of the array to a .pgm file for viewing
    for(int i=0; i<rows; i++) {
        //loops through rows and columns
        for(int j=0; j<columns-1; j++) {
            outFile << array[i][j] << " ";
        }
        //last column case appends new line
        outFile << array[i][columns-1] << endl;
    }
    outFile.close();
 }

