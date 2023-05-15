//#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_WORD_LEN 30 //max word length is defined here
#define MAX_LINE_LEN 120 //max characters in a given line is defined here


int verbose = 0, help = 0, debug = 0; /*num = 0;*/ // command line options are set to 0
char *filein = NULL; // filein variable is set to NULL



int get_dict_len( char *file) { //takes in a string that contains a file name
    FILE *fp; //file pointer is declared
    fp = fopen(file,"r"); // opens file in read mode and sets this to the file pointer

    int line_cnt = 0; //counter to track each line of the file
    char line[MAX_LINE_LEN]; //declares the line variable with a max line length specified in the global variable


    while(fgets(line,MAX_LINE_LEN,fp) != NULL){ //while loop to get the next line in the file pointer and checks
                                                // if the next line is NUlL, if not the code below is executed
        line_cnt++; //counter is incremented by 1 each time to get the number of lines in the file
    }
    fclose(fp); //closes the file
    return line_cnt; // returns the counter which should now have the total lines in the file
}

char *to_string(char *str);

int try_key(char **dict, char **text, int key);



int crack_rot(char **dict, char **text) { // crack_rot function, takes in the arrays that hold the dictionary and the
                                          // words in the sentence
    char** d; // declares the two dimensional array for the dictionary
    char** t; // declares the two dimensional array to hold the words in the text
    d = dict, t = text; // points the character pointer pointers to the parameters passed by the function

    int found = -1; //found is declared and set to -1
    int i; // variable i is declared before being used in the loop below
    for(i = 1; i <= 26; i++){ // sets i to 1 and performs the loop while i is less than or equals to 26

        found = try_key(d,t,1); // found variable is set to the value returned by try_key

        if(found != -1){ // if a valid key is found in try_key it will return a value other than -1
            return i; // when correct key is found then the crack_rot function just returns this key else the loop continues
        }
    }
    return found; //returns -1 if key is not found
}
void rotN(char *str, int n) { // NB: str will be modified in-place
    char *p; //declared character pointer to be used in loop below
    for (p=str; *p != '\0'; p++) { //pointer is set to the string passed in the function
        //while the pointer is not NULL, the loop continues, incrementing the char pointer each time
        int currChar = (int)*p; // variable is set to the value being that p is pointing to currently
        if (currChar >= 'a' && currChar <= 'z') { //if the current character is a small letter char then the code below
                                                    // is executed
            currChar = currChar+n; //current char is incremented by n, which is passed into the function
            if (currChar > 'z') { // if the new current char exceeds 'z' then the code below is run
                currChar = currChar-26; // 26 is subtracted from currChar to get a value within the bounds of the alphabet
            }
            *p = (char)currChar; //value at is set to the new rotated value in currChar
        }
        else if (currChar >= 'A' && currChar <= 'Z') { //does the same function above but compares against capital letters
            currChar = currChar+n;
            if (currChar > 'Z') {
                currChar = currChar-26;
            }
            *p = (char)currChar;
        }
    }
}

int try_key(char **dict, char **text, int key) { //takes in two 2-dimentional arrays and a key value to be rotated by
    char ** dic_arr; //declares dictionary array
    char** text_arr; //declares text array
    dic_arr = dict; //sets dictionary array to the 2-d dict array passed into the function
    text_arr = text; //sets dictionary array to the 2-d text array passed into the function
    int not_found = -1; // variable not_found is declared and set to -1
    int checker = 0; // variable checker is declared and set to -


    /*declares pointers to be used in for loop before*/
    char **p;
    char **q;

    // loop below goes through the text array appling the rotN function by the key passed into the function
    for(p = text_arr; **p != NULL; p++ ){ //for each word in the text array while the pointer is not null it is incremented
        //apply rotN on word
        rotN(*p, key);

        if(*(p+1) == NULL){ // error handler for when the next pointer will be NULL to prevent seg-fault
            break; //exits loop when condition above is met
        }
    }

    /*another for loop to now check if each new word is in the dictionary
     * for each word pointed to in p, the entire dictionary in second loop is compared against p*/
    for(p = text_arr; **p != NULL; p++ ){
        for(q = dic_arr; **q!=NULL; q++){
            checker = strcmp(*p, *q); // assigns value returned in strcmp to checker variable, 0 is returned if
                                      // the pointers are the same
            if(checker == 0){ // if the pointers are the same then the code below is executed
                break; //exits the current loop if the word exists in the dictionary
            }
            if(*(q+1) == NULL){ //when we reach the end of the dictionary the loop is exited
                break;
            }
            //compare current text with next word in dictionary

        }
        if (*(p+1) == NULL){
            if (checker == 0) {
                return checker; //when we reach the end of the dictionary the loop is exited
            }
            else{
            return not_found; //if at the end of the loop the value is checker is not 0 then -1 is returned
            }
        }

    }
}




char *to_string(char *str) { //this function take a character array ending with a new line char and replaces that with a '\0'
    char *p; //declares char pointer
    char *temp = (char* ) malloc (20 * sizeof(char)); //using malloc a temp variable is declared so that it can be returned by the function
    int counter = 0; //counter is set to 0 to track the current character
    for(p = str; *p!='\n'; p++){ //loops until pointer is at a new line character
        if(*(p+1) != '\n'){  //checks the next pointer is not a newline
            temp[counter] = *p; // assigns the current pointer to the temp variable at index specified in counter
            counter++;
        }
        else{ //if the next pointer is a new line char then the code below is executed
            temp[counter] = *p; // appends the current pointer to the temp variable
            counter++; //increments the counter
            temp[counter] = '\0'; //assigns the next char in the temp variable as '\0'
        }

    }
    return temp;// returns the new char array which should be a string value
}



int get_len(char * str){ //function returns the length of the char array passed as a parameter
    char * ptr, * start; //declares pointers for the loop below
    start = str; //sets start to the start address of the string pass
    int len = 0; //len is set to 0

    for(ptr = str; *ptr != '\0'; ptr++ ){ //loops until pointer points to null
        if(*ptr == ' '){ //if pointer shows a space char the code below is executed
            len++; // len is incremented by 1
        }
        if(*(ptr +1) == '\0'){ //ensures pointer never reaches null
            break; //exits the loop
        }
    }
    if (ptr != start) len++; // increases the len by 1 again
//    dict_len = len;
    return len; //returns value at len
}

void show_arr_words (char **arr_words){ //function will show the values in the array passed as a paramenter

    char ** p; //declares array pointer
    //int counter = 0; //sets counter to 0
    printf("Cracked text: ");
    for (p = arr_words; *(*p) != NULL; p++){ //loop runs until pointer is NULL, prints the pointer and incr the counter
        printf("%s ", *p );

        //counter++; //increments the counter, used in testing to verify array is indexed correctly
        if(*(p+1) == NULL){
            break;
        }
    }


}

void show_arr_wordsnl (char **arr_words){ //same as show_arr_words function above but includes a new line after each output
                                            //used for testing
    char ** p;
    int counter = 0;
    for (p = arr_words; *(*p) != NULL; p++){
        printf("  %s \n", *p );  //new line after each value

        counter++;
        if(*(p+1) == NULL){
            break;
        }
    }


}

char** read_dict(char* file){ //takes in a char array (string) to point to a file name
    FILE *fp; //declares file pointer
    fp = fopen(file,"r"); //opens file in read mode and assigns it to the file pointer
    int dict_len = get_dict_len(file); //gets the length of the file
    char ** array = /*(char **) */malloc( (dict_len +1) * sizeof (char *)); //using memory allocation
        //to allocate memory for the array to be returned

    int arr_count = 0; //sets arr counter to 0
    char buffer[MAX_LINE_LEN]; //declares char array with max length specified in MAX_LINE_LEN global variable
    if (fopen(file,"r") ==NULL){ //checks that file is actually readable
        printf("read failed\n"); //prints
    }
    else{
        while (arr_count < dict_len){ //while the array count is less than the length of the file
            fgets(buffer, MAX_LINE_LEN, fp); //saves line in the buffer variable
            char *temp /*= (char* ) malloc (128 * sizeof(char))*/;
            temp = to_string(buffer); //removes '\n' from the buffer char array

            array[arr_count] = temp; //appends temp value , now with '\0', to array at index in arr_count

            arr_count++; //increments arr_count to point to the next index
        }

        if (arr_count == dict_len){ //if the arr_count is the same as the dictionary length the code below is executed
            array[arr_count] = NULL; //sets final value in array to null
        }
    }

    fclose(fp); //closes file
    return array; //returns array
}


char **str_to_arr(char * str){  //takes in a string, sentence, and returns an array of the words

    char * ptr; //declares pointer
    int len; //declares length

    char **array = (char**) calloc(1, sizeof(char*)); //allocates memory to array variable

    len = get_len (str); //gets the length of the string

    int s_counter = 0; //index to point to current char in the sentence//string counter
    int c_counter = 0; //index to the current character the current word // word character counter

    int i;
    for ( i=0; i <=len;i++){//loop will run the number of times corresponding to number of words
        char *temp = (char* ) malloc(MAX_WORD_LEN * sizeof(char));
        if (i == len){ // if i is equal to the length then the code below is executed
            array[i] = NULL; // last element is set to NULL
        }
        else{
            for (ptr = (str+s_counter); *ptr != ' ' && *ptr !='\000'; ptr++){
                if (*(ptr + 1) != ' ' && *(ptr + 1) != '\000'){ // if the next pointer is not a space of NULL
                    temp[c_counter] = *ptr; //current pointer is assigned in temp[counter]
                    c_counter++; //c_counter is incremented by 1
                    s_counter++; //s_counter is incremented by 1
                }
                else{ //when the pointer to the next char is a space
                    temp[c_counter] = *ptr; //current pointer is assigned in temp[counter]
                    s_counter++; //string counter is incremented


                    temp[c_counter + 1] = '\0'; //temp will now add null to the end char

                    array = (char **) realloc(array, (i + 1) * sizeof(char *)); //realloc to add more space in array

                    array[i] = temp; //appends temp variable to array
                    c_counter = 0; //resets c_counter
                    break;
                }
            }
        }

        c_counter = 0; //character counter set to 0 as we move along to the next set of words
        s_counter++; //increments string counter
    }
    return array; //returns array
}


int main (int argc , char**argv) {
    char *str;
    int key, cracked;
    char *file = "words.txt";
    char **dict_array = read_dict(file);

    // see: man 3 getopt for docu and an example of command line parsing
    { // see the CW spec for the intended meaning of these options
        int opt;
        while ((opt = getopt(argc, argv, "hvdf:")) != -1) {
            switch (opt) {
                case 'v':
                    verbose = 1;
                    break;
                case 'h':
                    help = 1;
                    break;
                case 'd':
                    debug = 1;
                    break;
                case 'f':
                    filein = (char *) malloc(sizeof(char) * (strlen(optarg) + 1));
                    strcpy(filein, optarg);
                    break;
                default: /* '?' */
                    fprintf(stderr, "Usage: %s [-h] [-v] [-d] [-f <filename>] [<key> <plaintext>] \n", argv[0]);
                    exit(EXIT_FAILURE);
            }
        }
    }

    if (verbose) {
        fprintf(stdout, "Settings for running the program\n");
        fprintf(stdout, "Help is %s\n", (help ? "ON" : "OFF"));
        fprintf(stdout, "Verbose is %s\n", (verbose ? "ON" : "OFF"));
        fprintf(stdout, "Debug is %s\n", (debug ? "ON" : "OFF"));
        if (filein) fprintf(stdout, "Input file = %s\n", filein);
    }
    if (help) {
        fprintf(stdout,
                "%s: crack a rotation cipher, by finding the rotation key, given a plain and cipher text and using a dictionary\n",
                argv[0]);
        fprintf(stderr, "Usage: %s [-h] [-v] [-d] [-f <filename>] [<key> <plaintext>] \n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    if (optind < argc) { // Case: encrypting text with a key; for test case generation!
        if (verbose)
            fprintf(stderr, "Encrypting provided input string with provided key\nUsage: %s [options] <key> <string>\n",
                    argv[0]);
        str = (char *) malloc(strlen(argv[optind]));
        strcpy(str, argv[optind]);
        key = atoi(str);
        strcpy(str, argv[optind + 1]);
        fprintf(stderr, "Rotation value:  '%d'\n", key);
        fprintf(stderr, "Input:  '%s'\n", argv[2]);
        rotN(str, key);
        fprintf(stderr, "Output: '%s'\n", str);
        exit(EXIT_SUCCESS);
    } else {
        if (filein) { // CASE: with -f <filein> options, cipher-texts should be read from the file <filein> line by line
            char *file = filein;
            char **test_array = read_dict(file);
            int len = get_dict_len(file);
            int i = 0;
//            show_arr_wordsnl(test_array);
            while (i < len) {
                char *new_str = calloc(strlen(test_array[i]) + 1, sizeof(char));
                strcpy(new_str, test_array[i]);


                char **new_arr = str_to_arr(new_str);
                int key = crack_rot(dict_array, new_arr);
                if(debug) fprintf(stderr, "line = %s\n", test_array[i]);
                if (key != -1){
                    printf("Key: %d", key);
                    printf("\nTesting \n");
                    printf("Cipher text: '%s'  \n", new_str);


                    show_arr_words(new_arr);
                    printf("\nwith cracked key: %d\n\n", key);
                }
                else{
                    fprintf(stdout, "NO key found\n");
                }


                //            printf("%d\n", strlen(test_array[i]));
                i++;

            }

        }
        else{
            char str0[] = "the quick brown fox jump over the lazy dog";
            
            char exp0[] = "gur dhvpx oebja sbk whzc bire gur ynml qbt";
            char **arr_words;
            char **dict;
            int key = 13, dict_len, arr_len;

            // -----------------------------------------------------------------------------
            // the code below demonstrates how to use the functions above to crack the key
            // AFTER implementing each of the functions, UNCOMMENT the exit() line above to test hard-coded input
            dict = read_dict("words.txt"); // char**
            // show_arr_words(dict);
            // fprintf(stderr, "Input:    '%s'\n", str0);
            arr_words = str_to_arr(exp0);
            if (verbose)   show_arr_words(arr_words);
            cracked = crack_rot(dict, arr_words);
            if (cracked!=-1) {
                char decrypt[MAX_LINE_LEN];
                fprintf(stdout, "Key: %d (expected %d)\n", 26-cracked, key);
                strcpy(decrypt, exp0);
                rotN(decrypt, 26-cracked);
                fprintf(stdout, "Testing\nCipher text:  '%s'\nCracked text: '%s'\nPlain text:   '%s'\nwith cracked key: %d\n",
                        exp0, decrypt, str0, 26-cracked);
                if (strcmp(decrypt,str0)==0)
                    fprintf(stdout, "OK decrypted text.\n");
                else
                    fprintf(stdout, "WRONG decrypted text.\n");
            } else {
                fprintf(stdout, "NO key found\n");
            }
            free(dict);

            /*  Example of encryption:
            fprintf(stderr, "Rotation value:  '%d'\n", key);
            fprintf(stderr, "Input:    '%s'\n", str0);
            rotN_in_c(str0, key);
            fprintf(stderr, "Output:   '%s'\n", str0);
            fprintf(stderr, "Expected: '%s'\n", exp0);
            if (strcmp(str0,exp0) == 0) {
            printf(".. OK\n");
            } else {
            printf("** WRONG\n");
            }
            */
        }
    }
    exit(EXIT_SUCCESS);
}



//
//int main(int argc, char **argv )
//{
//    char *file = "words.txt";
//
//    char **dict_array = read_dict(file);
//    if (argc > 1){
//        char *str;
//        str = (char *) calloc(strlen(argv[1]) + 1, sizeof (char));
//        strcpy(str, argv[1]);
//        printf("String is %s\n", str);
//
//
//
//        char **str_array = str_to_arr(str);
//        int key = crack_rot(dict_array, str_array);
//        printf("Key is %d\n", key);
//        show_arr_words(str_array);
//
//    }
//    else {
//        char *file = "filein.txt";
//        char **test_array = read_dict(file);
////        show_arr_words(test_array);
//        char *string = "gur dhvpx oebja sbk whzc bire gur ynml qbt";
//
////        rotN(test_array[0], 13);
////        rotN(string,13);
////        printf("%s", test_array[0]);
////        printf("%s", string);
//        int len = get_dict_len(file);
//        int i = 0;
////        char * test = "hello\n";
////
////        printf("%d\n", strlen(test));
//
//        show_arr_wordsnl(test_array);
//
//
//        while (i < len) {
//            char *new_str = calloc(strlen(test_array[i]) + 1, sizeof(char));
//            strcpy(new_str, test_array[i]);
//
//
//            char **new_arr = str_to_arr(new_str);
//            int key = crack_rot(dict_array, new_arr);
//
//            printf("Key: %d", key);
//            printf("\nTesting \n");
//            printf("Cipher text: '%s'  \n", new_str);
//
//
//            show_arr_words(new_arr);
//            printf("\nwith cracked key: %d\n ", key);
//
////            printf("%d\n", strlen(test_array[i]));
//            i++;
//        }
//    }
//    return 0;
//
//}