#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

/**
 * @brief node structure for linked list
 */

typedef struct Node {
    int val; /**< original value */
    bool marked; /**<for marking zeros, following the hungarian algorithm for assignment problems*/
    bool crossed; /**<for crossing lines */
    int row; /**<row matrix index */
    int col; /**<column matrix index */
    int numberOfCrossingTimes; /**<this serves to check which nodes are crossed twice, indicating an intersection*/
    int value_affected_by_max_element; /**< value that will be altered in order to find the max sum */
    struct Node *next; /**<pointer to next node */
} Node;

int max_element;  /**<maximum element in the matrix */
int min_element; /**<minimum element in the matrix */
int number_of_cols; /**<number of columns in the matrix */
int number_of_lines;/**<number of lines in the matrix */
bool loop = true; /**<control variable for the algorithm loop */

/**
 * @brief Reads matrix data from file
 *
 * this function reads the matrix data from a file and creates a linked list representation of the matrix
 *
 * @param head Pointer to the head of the linked list
 */

void ReadFile(Node** head){
    char buffer[1024];
    FILE* file = fopen("../proj/cmake-build-debug/mat.txt", "r");
    if(file == NULL){
        printf("Unreadable file.\n");
    }

    int value;
    Node* current = *head;
    while (fscanf(file, "%d,", &value) == 1){
        if (current == NULL) {
            //if the current node is NULL create a new one and assign it as the head
            *head = (Node*)malloc(sizeof(Node));
            if (*head == NULL) {
                printf("Failed\n");
                exit(0);
            }
            (*head)->val = value;
            (*head)->next = NULL;
            current = *head;
        } else {
            current->next = (Node*)malloc(sizeof(Node));
            if (current->next == NULL) {
                printf("Fail\n");
                exit(0);
            }
            current = current->next;
            current->val = value;
            current->next = NULL;
        }
    }
    fscanf(file, "rows:%d\n", &number_of_lines);
    fscanf(file, "cols:%d", &number_of_cols);
    fclose(file);
}

/**
 * @brief Assigns matrix position to each node based on rows and columns
 * @param head Pointer to the head of the linked list
 */

void attributeMatrixPosition(Node* head){
    Node* current = head;
    while(current != NULL){
        for(int counter_for_line = 1; counter_for_line <= number_of_lines; counter_for_line++){
            for(int counter_for_col = 1; counter_for_col <= number_of_cols; counter_for_col++){
                if(current == NULL){
                    return;
                }
                current-> row = counter_for_line;
                current-> col = counter_for_col;
                current = current->next;
            }
        }
    }
}

/**
 * @brief Finds the maximum element in the matrix by iterating through the linked list
 * @param head Pointer to the head of the linked list
 */

void findMaxElement(Node* head) {
    Node* current = head;
    int counter = 0;
    while (current != NULL) {
        counter++;
        if(current->val > max_element){
            max_element = current->val;
        }
        current = current->next;
        if(counter == number_of_cols){
            printf("\n");
            counter = 0;
        }
    }
    printf("The biggest element is %d\n", max_element); //mostly for debug
}

/**
 * @brief Inserts a line at the end of the matrix
 *
 * transverses the list once to find the last element and then lets the user add a whole line
 *
 * @param head Pointer to the head of the linked list
 */

void insertLine(Node* head){
    int keep_on_loop = 0;
    Node* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    while(keep_on_loop < number_of_cols){
        Node *newNode = (Node *)malloc(sizeof(Node));
        int value;
        printf("Choose the value you want to insert: ");
        if(scanf("%d", &value) != 1){
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        newNode->val = value;
        newNode->next = NULL;

        current->next = newNode;
        current = newNode;
        keep_on_loop++;
    }
    number_of_lines++;
    attributeMatrixPosition(head);
}

/**
 * @brief Inserts a line of zeros at the end of the matrix
 *
 * this is specifically for the hungarian algorithm, as it requires the matrix to be a square matrix
 *
 * @param head Pointer to the head of the linked list
 */

void insertLineofZeros(Node* head){
    int keep_on_loop = 0;
    Node* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    while(keep_on_loop < number_of_cols){
        Node *newNode = (Node *)malloc(sizeof(Node));
        int value = 0;
        newNode->val = value;
        newNode->next = NULL;

        current->next = newNode;
        current = newNode;
        keep_on_loop++;
    }
    number_of_lines++;
    attributeMatrixPosition(head);
}

/**
 * @brief Inserts a column at the end of the matrix
 *
 * calls the attribute matrix position function at the end so every node has a position parameter
 *
 * @param head Pointer to the head of the linked list
 */
void insertColumn(Node* head) {
    Node* current = head;
    int counter = 1;
    int linecounter = 1;

    while (current != NULL && linecounter <= number_of_lines) {
        if (counter == number_of_cols) {
            Node *newNode = (Node *)malloc(sizeof(Node));
            if (newNode == NULL) {
                printf("Failed\n");
                return;
            }

            int value;
            printf("Choose the value you want to insert: ");
            scanf("%d", &value);
            newNode->val = value;
            newNode->next = current->next;
            current->next = newNode;

            counter = 0;
            linecounter++;

            current = newNode->next; //skip the new node
        } else {
            current = current->next;
        }
        counter++;
    }
    number_of_cols++;
    attributeMatrixPosition(head);
}

/**
 * @brief Inserts a column of zeros at the end of the matrix
 * @param head Pointer to the head of the linked list
 */

void insertColumnofZeros(Node* head){
    Node* current = head;
    int counter = 1;
    int linecounter = 1;

    while (current != NULL && linecounter <= number_of_lines) {
        if (counter == number_of_cols) {
            Node *newNode = (Node *)malloc(sizeof(Node));
            if (newNode == NULL) {
                printf("Failed\n");
                return;
            }

            newNode->val = 0;
            newNode->next = current->next;
            current->next = newNode;

            counter = 0;
            linecounter++;

            current = newNode->next; //skip the new node
        } else {
            current = current->next;
        }
        counter++;
    }
    number_of_cols++;
    attributeMatrixPosition(head);
}

/**
 * @brief Prints the matrix represented by the linked list
 * @param head Pointer to the head of the linked list
 */

void printList(Node* head) {
    Node* current = head;
    int counter = 0;
    while (current != NULL) {
        printf("%d ", current->val);
        counter++;
        current = current->next;
        if(counter == number_of_cols){
            printf("\n");
            counter = 0;
        }
    }
}

/**
 * @brief Prints the matrix represented by the linked list
 *
 * mostly for debug purposes, allows to print the matrix after the row and column reductions
 *
 * @param head Pointer to the head of the linked list
 */

void printListReduced(Node* head) {
    Node* current = head;
    int counter = 0;
    while (current != NULL) {
        printf("%d ", current->value_affected_by_max_element);
        counter++;
        current = current->next;
        if(counter == number_of_cols){ // this lets me print the matrix in it's actual format
            printf("\n");
            counter = 0;
        }
    }
}

/**
 * @brief Iterates through the matrix to assign new values affected by the maximum element
 *
 * this will iterate through the linked list in order to assigned the new variable in the struct with a new number obtained by subtracting from the max element of the matrix every other element
 *
 * @param head Pointer to the head of the linked list
 */

void iterate(Node* head) {
    Node* current = head;
    while (current != NULL) {
        current->value_affected_by_max_element =  max_element - current->val;
        current = current->next;
    }
}

/**
 * @brief Reduces each row by subtracting the minimum value in that row from each value in that row
 * @param head Pointer to the head of the linked list
 */

void rowReduce(Node* head){
    Node* current = head;
    int minVal = INT_MAX;
    //first loop makes sure it goes through all the lines
    for(int j = 0; j < number_of_lines; j++) {
        //second loop goes through a line and finds the mininum value
        for (int i = 0; i < number_of_cols; i++) {
            if(current->value_affected_by_max_element < minVal){
                minVal = current-> value_affected_by_max_element;
            }
            current = current-> next;
        }
        current = head;
        //third loop will make the substraction
        for(int k = 0; k < number_of_cols; k++){
            current->value_affected_by_max_element -= minVal;
            current = current->next;
        }
        //at the end, head is now set to the beginning of the next line
        //and we reset the min value so we can get a unique one per line
        head = current;
        minVal = INT_MAX;
    }
}

/**
 * @brief Reduces each column by finding the smallest number in each col and subtracting it from the in that same col
 * @param head Pointer to the head of the linked list
 */

void columnReduce(Node* head){
    Node* current = head;
    int whatColAmI = 1;
    for(int i = 0; i < number_of_cols; i++){
        int minVal = INT_MAX;
        while(current != NULL){
            if(current->value_affected_by_max_element < minVal && whatColAmI == current->col){
                minVal = current->value_affected_by_max_element;
            }
            current = current->next;
        }
        current = head;
        while(current != NULL){
            if(current->col == whatColAmI){
                current->value_affected_by_max_element -= minVal;
            }
            current = current->next;
        }
        current = head;
        whatColAmI++;
    }
}

/**
 * @brief Counts the number of zeros in a line
 *
 * function is essencial to figuring out which lines and columns have the most zeros so we can decide which to cross out
 *
 * @param head Pointer to the head of the linked list
 * @param position The position of the line
 * @return The number of zeros in the line
 */

int countZerosByLine(Node* head, int position){
    Node* current = head;
    int zeroline = 0;
    while(current != NULL){
        if(current->value_affected_by_max_element == 0 && current->row == position){
            zeroline++;
        }
        current = current->next;
    }
    return zeroline;
}

/**
 * @brief Counts the number of zeros in a column
 *
 * function is essencial to figuring out which lines and columns have the most zeros so we can decide which to cross out
 *
 * @param head Pointer to the head of the linked list
 * @param position The position of the column
 * @return The number of zeros in the column
 */

int countZerosByCol(Node* head, int position){
    Node* current = head;
    int zerocol = 0;
    while(current != NULL){
        if(current->value_affected_by_max_element == 0 && current->col == position){
            zerocol++;
        }
        current = current->next;
    }
    return zerocol;
}

/**
 * @brief Helper function for tracking zeros in rows and columns
 *
 * basically it will check if the a node containing 0 is on the same row or col of a previously marked or already crossed out zero is
 *
 * @param array Array to track zeros
 * @param c Column index
 * @param r Row index
 * @return True if the element is not in the same line or col of an already marked 0, otherwise false
 */

bool yesORno(int* array[2],int c, int r){
    for(int i = 0; i < number_of_cols; i++){
        if (array[0][i] == c || array[1][i] == r) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Checks if a line is already crossed
 * @param crossedline Array to track crossed lines
 * @param line The line to check
 * @return True if the line is already crossed, otherwise false
 */

bool isItInLine(int crossedline[], int line){
    for(int i = 0; i < number_of_cols; i++){
        if(crossedline[i] == (line + 1)){
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks if a column is already crossed
 * @param crossedcol Array to track crossed columns
 * @param col The column to check
 * @return True if the column is already crossed, otherwise false
 */

bool isItInCol(int crossedcol[], int col){
    for(int i = 0; i < number_of_cols; i++){
        if(crossedcol[i] == (col + 1)){
            return true;
        }
    }
    return false;
}

/**
 * @brief Determines whether to cut a line or not based on already crossed zeros
 *
 * this serves to avoid cutting a line unnecessarily, as in, if perchance the number of zeros in a line were reduced to 0 after cutting another line or column
 *
 * @param head Pointer to the head of the linked list
 * @param line_to_cut The line to check
 * @return False if all zeros in the line are already crossed out, otherwise true
 */

bool whatToCutLine(Node* head,int line_to_cut){
    Node* current = head;
    int zeros_in_line = 0;
    int lines_already_crossed = 0;
    while(current != NULL){
        if(current->row == line_to_cut && current->value_affected_by_max_element == 0){
            zeros_in_line++;
        }
        current=current->next;
    }
    current = head;
    while(current != NULL){
        if(current->row == line_to_cut && current->value_affected_by_max_element == 0 && current->crossed == true){
            lines_already_crossed++;
        }
        current = current->next;
    }
    if(lines_already_crossed == zeros_in_line){
        return false;
    }
    return true;
}

/**
 * @brief Determines whether to cut a column or not based on already crossed zeros
 *
 * this serves to avoid cutting a column unnecessarily, as in, if perchance the number of zeros in a column were reduced to 0 after cutting another line or column
 *
 * @param head Pointer to the head of the linked list
 * @param col_to_cut The column to check
 * @return False if all zeros in the column are already crossed out, otherwise true
 */
bool whatToCutCol(Node* head, int col_to_cut){
    Node* current = head;
    int col_already_crossed = 0;
    int zeros_in_col = 0;
    while(current != NULL){
        if(current->col == col_to_cut && current->value_affected_by_max_element == 0){
            zeros_in_col++;
        }
        current=current->next;
    }
    current = head;
    while(current != NULL){
        if(current->col == col_to_cut && current->value_affected_by_max_element == 0 && current->crossed == true){
            col_already_crossed++;
        }
        current = current->next;
    }
    if(col_already_crossed == zeros_in_col){
        return false;
    }
    return true;
}

/**
 * @brief Crosses out lines and columns in the matrix based on the Hungarian algorithm
 *
 * this will use the two arrays to check which line/col has more zeros, so those can be cut out first funtion stops when the total number of zeros is equal to the amount of zeros crossed
 *
 * @param head Pointer to the head of the linked list
 * @param arraycol Array tracking zeros by columns
 * @param arrayline Array tracking zeros by rows
 */

void strikeYouAreOut(Node* head,int arraycol[], int arrayline[]){
    Node* current = head;
    int total_zero_counter = 0;
    int* crossedColTracker = (int *)malloc(number_of_cols * sizeof(int));
    int* crossedLineTracker = (int *)malloc(number_of_lines * sizeof(int));
    for(int i = 0; i < number_of_cols; i++){
        crossedColTracker[i] = 0;
        crossedLineTracker[i] = 0;
    }

    while(current != NULL){
        if(current-> value_affected_by_max_element == 0){
            total_zero_counter++;
        }
        current = current->next;
    }
    int totalcrossedZeros = 0;
    while(total_zero_counter != totalcrossedZeros){
        current = head;
        bool itsCOL = false;
        bool itsLINE = false;
        int max_num_line = 0;
        int line_to_be_crossed = INT_MIN;
        int col_to_be_crossed = INT_MIN;
        int max_num_col = -1;
        int i;
        for(i = 0; i < number_of_cols; i++){
            if((max_num_line <= arrayline[i] && isItInLine(crossedLineTracker, i) == false) && whatToCutLine(head, (i + 1)) == true){
                max_num_line = arrayline[i];
                line_to_be_crossed = i + 1;
            }
            if((max_num_col <= arraycol[i] && isItInCol(crossedColTracker, i) == false) && whatToCutCol(head, (i + 1)) == true){
                max_num_col = arraycol[i];
                col_to_be_crossed = i + 1;
            }
        }

        int track_for_line = 0;
        int track_for_col = 0;
        if(max_num_line > max_num_col){
            itsLINE = true;
            crossedLineTracker[track_for_line] = line_to_be_crossed;
            track_for_line++;
        }else if(max_num_col > max_num_line){
            itsCOL = true;
            crossedColTracker[track_for_col] = col_to_be_crossed;
            track_for_col++;
        }else if(max_num_line == max_num_col){
            itsLINE = true;
            crossedLineTracker[track_for_line] = line_to_be_crossed;
            track_for_line++;
        }

        if(itsLINE == true){
            while(current != NULL){
                if(current->row == line_to_be_crossed){
                    if(current->crossed == true){
                        current->numberOfCrossingTimes++;
                    }else{
                        current->crossed = true;
                        current->numberOfCrossingTimes++;
                    }
                }else if(current->crossed == true){
                    current->crossed = true;
                }else{
                    current->crossed = false;
                }
                current = current->next;
            }
            current = head;
        }else if(itsCOL == true){
            while(current != NULL){
                if(current->col == col_to_be_crossed){
                    if(current->crossed == true){
                        current->numberOfCrossingTimes++;
                    }else{
                        current->crossed = true;
                        current->numberOfCrossingTimes++;
                    }
                }else if(current->crossed == true) {
                    current->crossed = true;
                }else{
                    current->crossed = false;
                }
                current = current->next;
            }
            current = head;
        }
        totalcrossedZeros = 0;
        while(current != NULL){
            if(current->value_affected_by_max_element == 0 && current->crossed == true){
                totalcrossedZeros++;
            }
            current = current->next;
        }
    }
}

/**
 * @brief Final step of the Hungarian algorithm - calculates the maximum sum
 *
 * this should calculate the sum using the original values and the marked function if a node is marked then we should get the original value and add it to the sum
 *
 * @param head Pointer to the head of the linked list
 */

void done(Node* head){
    Node* current = head;
    int sum = 0;
    while (current != NULL){
        if(current->marked == true){ /**if position is marked, check the original value and add it to the sum*/
            sum += current->val;
        }
        current = current->next;
    }
    printf("The optimal maximum sum is %d.\n", sum);
    printf("\n");
    loop = false; /**ends the algorithm loop and gives the control back to the outer loop in the main function*/
}

/**
 * @brief Step 1 of the Hungarian algorithm
 *
 * first step is based on finding a zero per line/col and marking it so that we have a unique number selected in every row and col
 *
 * @param head Pointer to the head of the linked list
 */

void step1(Node* head){
    Node* current = head;
    const int rows = 2;
    int* array_to_track_zeros[rows];
    for(int i = 0; i < rows; i++){
        array_to_track_zeros[i] = (int*)malloc((number_of_cols * number_of_lines) * sizeof(int));
    }
    int i = 0;
    /**this iteration serves to reset the attributes of the matrix before passing through this step again*/
    while(current != NULL){
        current->numberOfCrossingTimes = 0;
        current->marked = false;
        current->crossed = false;
        current = current->next;
    }
    current = head;

    /**this iteration selects a zero from every col/line so that it won't have more than one selected value in the same row/col*/
    while(current != NULL){
        if(current->value_affected_by_max_element == 0 && yesORno(array_to_track_zeros, current->col, current->row) == true){
            current->marked = true;
            /**array_to_track_zeros, uses the iteration to keep track of the already marked zeros
            and to stop the program from marking a zero if there's already a marked one in
            the same row/col*/
            array_to_track_zeros[0][i] = current->col;
            array_to_track_zeros[1][i] = current->row;
            i++;
        }else{
            current->marked = false;
        }
        current = current->next;
    }
}

/**
 * @brief Step 2 of the Hungarian algorithm
 *
 * step 2 checks if the number of marked elements equals to the number of lines/cols indicating there's an unique number for every line/col, if not, it prompts the counting zeros per line and col functions and the strike out function so we can proceed with the algorithm
 *
 * @param head Pointer to the head of the linked list
 */

void step2(Node* head){
    Node* current = head;
    int marked_zero_counter = 0;
    int* array_for_cols;
    int* array_for_rows;
    array_for_cols = (int*)malloc(number_of_cols * sizeof(int)); //stores the number of zeros in each col
    array_for_rows = (int*)malloc(number_of_lines * sizeof(int)); //stores the number of zeros in each line

    /**sets everything to 0 to prevent errors*/
    for(int i = 0; i < number_of_cols; i++){
        array_for_cols[i] = 0;
        array_for_rows[i] = 0;
    }

    while(current != NULL){
        //if an element is marked then we add 1 to the marked zero counter
        if(current->marked == true){
            marked_zero_counter++;
        }
        current = current->next;
    }
    if(marked_zero_counter == number_of_cols){ //if we have an unique element marked, per line and col, then we found the optimal solution
        //call function done
        done(head);
    }else{
        //if not, we have to now cover the cols/lines with zeros with the least amount of lines possible
        //for that we need to figure out the lines/cols with the most amount of zeros so we can cross those out first
        int position = 0;
        for(int i = 0; i < number_of_cols; i++){
            array_for_rows[i] = countZerosByLine(head, position + 1);
            array_for_cols[i] = countZerosByCol(head, position + 1);
            position++;
        }
        strikeYouAreOut(head, array_for_cols, array_for_rows); //this will take care of the crossing out
    }
}

/**
 * @brief Step 3 of the Hungarian algorithm
 *
 * step 3 is based on finding the minimum element that's not crossed out we then iterate through the linked list again, subtracting the min val from the uncrossed elements and adding the min val to the values that have been crossed twice
 *
 * @param head Pointer to the head of the linked list
 */

void step3(Node* head){
    Node* current = head;
    int minimum_num = INT_MAX;
    while(current != NULL){
        if(current->value_affected_by_max_element < minimum_num && current->crossed == false){
            minimum_num = current->value_affected_by_max_element;
        }
        current = current->next;
    }
    current = head;
    while (current != NULL){
        if(current->numberOfCrossingTimes == 2){
            current->value_affected_by_max_element += minimum_num;
        }else if(current->crossed == false){
            current->value_affected_by_max_element -= minimum_num;
        }
        current = current->next;
    }
}

/**
 * @brief Calculates the maximum sum using a variation of the Hungarian algorithm
 *
 * first checks if the matrix is squared or not and calls the respective functions to fix that
 * after that, it row and column reduced, as well as printing the reductions to the console
 * and at last, it triggers the algorithm loop, which will run, until the function done is triggered
 *
 * @param head Pointer to the head of the linked list
 */

void findSumMax(Node* head){
    attributeMatrixPosition(head);
    iterate(head);
    printListReduced(head);
    printf("\n");
    int res = 0;
    if(number_of_cols != number_of_lines){
        if(number_of_cols > number_of_lines){
            res = number_of_cols - number_of_lines;
            for(int i = 0; i < res; i++){
                insertLineofZeros(head);
            }
        }else{
            res = number_of_lines - number_of_cols;
            for(int i = 0; i < res; i++){
                insertColumnofZeros(head);
            }
        }
    }
    //these prints are mostly for debugging purposes
    printListReduced(head);
    printf("\n");
    rowReduce(head);
    printf("\n");
    printListReduced(head);
    printf("\n");
    columnReduce(head);
    printf("\n");
    printListReduced(head);
    printf("\n");

    while(loop == true){
        step1(head);
        step2(head);
        step3(head);
    }
}

/**
 * @brief Allows the user to alter a specific element in the matrix
 * @param head Pointer to the head of the linked list
 */

void alterElement(Node* head){
    Node* current = head;
    bool loopy = true;
    while(loopy == true){
        printf("Choose row of number you wish to change: ");
        int choice;
        int choice1;
        if(scanf("%d", &choice) != 1){
            printf("Invalid option.\n");
            while (getchar() != '\n');
            continue;
        }
        printf("Choose column of number you wish to change: ");
        if(scanf("%d", &choice1) != 1){
            printf("Invalid.\n");
            while (getchar() != '\n');
            continue;
        }else{
            if(choice > number_of_lines || choice1 > number_of_cols || choice <= 0 || choice1 <= 0){
                printf("Invalid option.\n");
                continue;
            }
            while(current != NULL){
                if(current->row == choice && current->col == choice1){
                    int choice2;
                    printf("Choose number you wish to change it to:");
                    if(scanf("%d", &choice2) != 1){
                        printf("Invalid choice.\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    current->val = choice2;
                    loopy = false;
                    break;
                }
                current = current -> next;
            }
        }
    }
}

/**
 * @brief Allows the user to remove a line or column from the matrix
 * @param head_ref Pointer to the pointer to the head of the linked list
 */

void removeLineOrCol(Node** head_ref){
    Node* current = *head_ref;
    bool startloop = true;
    while(startloop == true){
        printf("Do you wish to remove a column or line? Type 0 for column and 1 for line.\n");
        int choice;
        if(scanf("%d", &choice) != 1){
            printf("Invalid.\n");
            while (getchar() != '\n');
            continue;
        }else{
            int choice2;
            Node* previous = NULL;
            switch(choice){
                case 0:
                    printf("Which column do you wish to remove? ");
                    if(scanf("%d", &choice2) != 1){
                        printf("Invalid.\n");
                        while (getchar() != '\n');
                        continue;
                    }else{
                        while (current != NULL) {
                            if (current->col == choice2) {
                                if (previous == NULL) {
                                    Node* temp = current;
                                    *head_ref = current->next; //move head to next node
                                    current = current->next; //move current to the next node
                                    free(temp); //free removed node
                                } else {
                                    Node* temp = current;
                                    previous->next = current->next; //link previous node to next bypassing current
                                    current = current->next; //move current to the next node
                                    free(temp); //free removed node
                                }
                            } else {
                                previous = current; //move forward previous
                                current = current->next; //move forward current
                            }
                        }
                        number_of_cols--;
                        attributeMatrixPosition(*head_ref);
                        startloop = false;
                        break;
                    }
                case 1:
                    printf("Which line do you wish to remove? ");
                    if(scanf("%d", &choice2) != 1){
                        printf("Invalid.\n");
                        while (getchar() != '\n');
                        continue;
                    }else{
                        while (current != NULL) {
                            if (current->row == choice2) {
                                if (previous == NULL) {
                                    Node* temp = current;
                                    *head_ref = current->next; //move head to next node
                                    current = current->next; //move current to the next node
                                    free(temp); //free removed node
                                } else {
                                    Node* temp = current;
                                    previous->next = current->next; //link previous node to next bypassing current
                                    current = current->next; //move current to the next node
                                    free(temp); //free removed node
                                }
                            } else {
                                previous = current; //move forward previous
                                current = current->next; //move forward current
                            }
                        }
                        number_of_lines--;
                        attributeMatrixPosition(*head_ref);
                        startloop = false;
                        break;
                    }
                default:
                    continue;
            }

        }
    }
}
/**
 * @brief Main function
 */

int main() {
    Node *head = NULL;
    bool loopy = true;
    ReadFile(&head);
    int pick;
    while (loopy == true){
        printList(head);
        attributeMatrixPosition(head);
        findMaxElement(head);
        printf("1-Insert a column\n"
               "2-Insert a line\n"
               "3-Remove line or column\n"
               "4-Alter value\n"
               "5-Calculate max value\n"
               "6-Exit\n"
               "Type number to choose option: ");
        scanf("%d", &pick);
        switch (pick) {
            case 1:
                insertColumn(head);
                break;
            case 2:
                insertLine(head);
                break;
            case 3:
                removeLineOrCol(&head);
                break;
            case 4:
                alterElement(head);
                break;
            case 5:
                findSumMax(head);
                break;
            case 6:
                exit(0);
            default:
                printf("Choose a valid option\n");
                continue;
        }
    }
}
