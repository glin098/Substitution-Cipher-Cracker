#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>

char alphabet[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char english_freq[26] = "ETAOINSHRDLCUMWFGYPBVKJXQZ";


char* decrypt(char* ciphertext, char generated_key[]){

    int key_indexes[1000];
    char str_cpy[strlen(ciphertext)];
    memcpy(str_cpy, ciphertext, strlen(ciphertext));
    int position = 0;

    for(int i = 0; i < strlen(ciphertext); i++){
        for(int j = 0; j < strlen(generated_key); j++){
            if(str_cpy[i] != generated_key[j]){
                continue;
            }
            else if(str_cpy[i] == generated_key[j]){
                key_indexes[position++] = j;
            }
        }
    }
    for(int i = 0; i < strlen(ciphertext); i++){
        str_cpy[i] = alphabet[key_indexes[i]];
    }
    return str_cpy;
}

void displayLetterFrequency(int count[]){
    printf("\nLetter Frequency: \n");
    for(int idx = 0; idx < 26; idx++){
        printf("%c = %d\n", idx + 'A', count[idx]);
    }
}

void displayEnglishFreq(int arr_freq[]){
    printf("\nEnglish Letter Frequency: \n");
    for(int i = 0; i < 26; i++){
        printf("%c = %d\n", english_freq[i], arr_freq[i]);
    }
}

void findFrequency(char* ciphertext){
    int count[26] = {0}, idx = 0;
    int arr_freq[26];
    int position = 0;
    int c = 0;
    
    // counts each letter in ciphertext
    while(ciphertext[idx] != '\0'){
        count[ciphertext[idx++] - 'A']++;
    }

    displayLetterFrequency(count);

    // adding the frequencies in an array 
    for(int idx = 0; idx < 26; idx++){
        arr_freq[position++] = count[idx];
        c++;
    }

    // sorting array frequencies of ciphertext
    int temp;
    for(int i = 0; i < c; ++i){
        for(int j = i + 1; j < c; ++j){
            if(arr_freq[i] < arr_freq[j]){
                temp = arr_freq[i];
                arr_freq[i] = arr_freq[j];
                arr_freq[j] = temp;
            }
        }
    }
    displayEnglishFreq(arr_freq);
}

char* generateKey(char* ciphertext){

    int count[26] = {0}, idx = 0;
    int arr_freq[26];
    int position = 0;
    int c = 0;

    while(ciphertext[idx] != '\0'){
        count[ciphertext[idx++] - 'A']++;
    }

    for(int idx = 0; idx < 26; idx++){
        arr_freq[position++] = count[idx];
        c++;
    }

    // sorting
    int arr_letterIndexes[26] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
    int temp;
    for(int i = 0; i < 26; i++){
        for(int j = i + 1; j < c; j++){
            if(count[i] < count[j]){
                temp = count[i];
                count[i] = count[j];
                count[j] = temp;
                temp = arr_letterIndexes[i];
                arr_letterIndexes[i] = arr_letterIndexes[j];
                arr_letterIndexes[j] = temp;
            }
        }
    }

    //generate key
    static char generated_key[26];
    int p = 0;
    for(int i = 0; i < 26; i++){
        generated_key[p++] = alphabet[arr_letterIndexes[i]];
    }

    printf("Key: ");
    for(int i = 0; i < 26; i++){
        generated_key[english_freq[i] - 'A'] = alphabet[arr_letterIndexes[i]];
    }

    return generated_key;
}

char *remove_white_spaces(char *str)
{
	int i = 0, j = 0;
	while (str[i] != '\0')
	{
		if (str[i] != ' ' && str[i] != '\n'){
          str[j++] = str[i];
        }
		i++;
	}
	str[j] = '\0';
	return str;
}

char *permute(char *generated_key){

    char swp = 0;;
    int i = 0; 

    // cyclic
    char state1;
    for(int i = 0; i < strlen(generated_key); i++){
        if(i == 0){
            state1 = generated_key[i];
        }
        else if(i == strlen(generated_key) - 1){
            generated_key[i - 1] = generated_key[i];
            generated_key[strlen(generated_key) - 1] = state1;
        }
        else{
            generated_key[i - 1] = generated_key[i];
        }
    }
    
    printf("\nNew Key: %s", generated_key); 

    return generated_key;
}

int getScore(char* ciphertext, char generated_key[]){
    FILE *dict;
    dict = fopen("dictionary.txt", "r"); 

    if (dict == NULL){
        printf("cant open file");
        exit(0);
    }

    char data[1000]; // 1000 lines of data
    char line[100]; // one line from file
    int score = 0, i = 0;
    char swp = 0;

    while(fgets(line, sizeof(line), dict)){
        char* l = remove_white_spaces(line);

        // // find occurance of a word in the dictionary to the decrypted output
        char* locate = strstr(ciphertext, l); 
        
        if(locate){
            score++;
        }
    }
    printf("\nScore: %d\n", score);
    return score;
}


int main(){

    char ciphertext[1000];
    printf("input ciphertext: \n");
    scanf("%s", ciphertext);

    char key_arr[26];
    printf("\nCIPHERTEXT: \n%s", ciphertext);

    findFrequency(ciphertext);

    char* generatedKey = generateKey(ciphertext); // prints out key

    char save_key[strlen(generatedKey)];
    memcpy(save_key, generatedKey, strlen(generatedKey));

    char* output = decrypt(ciphertext, generatedKey);

    // print output using generated key
    printf("\n\nOutput ciphertext using generated Key: \n%s\n", output);

    int c1;
    int c2;

    int score = getScore(output, generatedKey);
    printf("%d\n", score);

    printf("%s\n", generatedKey);

    char temp = 0;
    int max = 0;

    while(1){
        printf("What are the two indexes you want to switch in the key? (0-25): \n");
        printf("Index 1: ");
        scanf("%d", &c1);
        printf("Index 2: ");
        scanf("%d", &c2);

        temp = generatedKey[c1];
        generatedKey[c1] = generatedKey[c2];
        generatedKey[c2] = temp;

        printf("Generated Key: %s\n", generatedKey);
        char* decryptText = decrypt(ciphertext, generatedKey);
        printf("\nDecrypt using new key: %s\n", decryptText);
        score = getScore(decryptText, generatedKey);
        printf("%d\n", score);

        char max_key[strlen(generatedKey)];

        if(score > max){
            max = score;
            memcpy(max_key, generatedKey, strlen(generatedKey));
        }
    }
}

