/******************************************************************************************
* Advanced Data Structures | Project-3 | Blomm Filters using universal hash functions
* Submitted by: Gautam Verma(gverma), Anshul Chandra (achand13)
********************************************************************************************/

#include<string>
#include<vector>
#include<iostream>
#include<ctime>
#include<climits>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

const int prime = 15993437;					/* Prime Number*/
const int totalHashFunctions = 6;			/* Total hash functions = ln(2) * (n/m)*/
const int initialSizeOfRandomVecor = 10;	/* Initial Size of random vectors to be created*/
const int maxStrings = 2000000;				/* Max keys in the bloom filter*/
const int rowsInBF = 8;						/* Max rows in bloom filter*/

vector<vector<int> > randomVectors;

struct bf_t {
	char table[rowsInBF][maxStrings / 8];
};

/*HELPER PRINT FUNCTIONS*/
//void showVector(vector<int> v, string vectorName = "") {
//	cout << vectorName << ":";
//	for (std::vector<int>::iterator i = v.begin(); i < v.end(); ++i) {
//		cout << *i << " ";
//	}
//	cout << endl;
//}
//
//void showBF(bf_t *bf) {
//	for (int i = 0; i < rowsInBF; ++i) {
//		for (int j = 0; j < maxStrings / 8; ++j) {
//			cout << (int)bf->table[i][j] << " ";
//		}
//		cout << endl;
//	}
//}

/*
Increases the size of a random vector to 'toSize'.
*/
void increaseRandomAVectorSize(vector<int> &randomVector, int toSize) {
	for (int i = randomVector.size(); i <= toSize; ++i) {
		randomVector.push_back((rand() % prime) + 1);
	}
}

/*
Makes 'K' random vectors of default size 'initialSizeOfRandomVector'.
*/
void makeKRandomVectors() {
	for (int i = 0; i < totalHashFunctions; ++i) {
		vector<int> new_vector;
		randomVectors.push_back(new_vector);
		increaseRandomAVectorSize(randomVectors[i], initialSizeOfRandomVecor);
	}
}

/*
Calculates dot product of two vectors.
*/
vector<int> getDotProduct(vector<int> keyVector, vector<int> &randomVector) {
	if (randomVector.size() < keyVector.size()) {
		increaseRandomAVectorSize(randomVector, keyVector.size());
	}
	vector<int> result;
	for (int i = 0; i < keyVector.size(); ++i) {
		result.push_back(keyVector[i] * randomVector[i]);
	}
	return result;
}

/*
Calculates summation of dot product of two vectors.
*/
int getSummationOfVectorElements(vector<int> v) {
	unsigned long sum = 0;
	for (int i = 0; i < v.size(); ++i)
		sum = (sum + v[i]) % prime;
	return (int)(sum % prime);
}

/*
Generates the vector representation of a key.
*/
vector<int> getKeyVector(char *str) {
	vector<int> keyVector;
	while (*str) {
		keyVector.push_back(300 + (int)*str);
		str++;
	}
	return keyVector;
}

/*
Calculates hash value of a key.
*/
int getHashValue(vector<int> keyVector, vector<int> &randomVector) {
	return getSummationOfVectorElements(getDotProduct(keyVector, randomVector));
}

/*
Gives row index corresponding to hash value.
*/
int getRowOfHashValue(int hashValue) {
	return hashValue / maxStrings;
}

/*
Gives column index corresponding to hash value.
*/
int getColumnOfHashValue(int hashValue) {
	return (hashValue % maxStrings) / rowsInBF;
}

/*
Gives bit position in the column index corresponding to hash value.
*/
int getBitPositionInColumn(int hashValue) {
	return ((hashValue % maxStrings) % rowsInBF);
}

/*
Initializes a bloom filter.
*/
bf_t * create_bf() {
	bf_t *bf = (bf_t *)calloc(1, sizeof(bf_t));
	return bf;
}

/*
Inserts a key into the bloom filter.
*/
void insert_bf(bf_t *b, char *s) {
	if (randomVectors.size() == 0) {
		makeKRandomVectors();
	}
	int hashValue, row, column;
	vector<int> keyVector = getKeyVector(s);
	for (int i = 0; i < totalHashFunctions; ++i) {
		hashValue = getHashValue(keyVector, randomVectors[i]);
		row = getRowOfHashValue(hashValue);
		column = getColumnOfHashValue(hashValue);
		int ls = 1 << getBitPositionInColumn(hashValue);
		char nv = b->table[row][column] | ls;
		b->table[row][column] = nv;
	}
}

/*
Tells whether a string is a member of a set.
*/
int is_element(bf_t *b, char *q) {
	if (randomVectors.size() == 0) {
		makeKRandomVectors();
	}
	int hashValue, row, column;
	vector<int> keyVector = getKeyVector(q);
	for (int i = 0; i < totalHashFunctions; ++i) {
		hashValue = getHashValue(keyVector, randomVectors[i]);
		row = getRowOfHashValue(hashValue);
		column = getColumnOfHashValue(hashValue);
		if ((b->table[row][column] & (1 << getBitPositionInColumn(hashValue))) == 0) {
			return 0;
		}
	}
	return 1;
}
#include <stdlib.h>
#include <time.h>

void sample_string_A(char *s, long i)
{
	s[0] = (char)(1 + (i % 254));
	s[1] = (char)(1 + ((i / 254) % 254));
	s[2] = (char)(1 + (((i / 254) / 254) % 254));
	s[3] = 'a';
	s[4] = 'b';
	s[5] = (char)(1 + ((i*(i - 3)) % 217));
	s[6] = (char)(1 + ((17 * i + 129) % 233));
	s[7] = '\0';
}
void sample_string_B(char *s, long i)
{
	s[0] = (char)(1 + (i % 254));
	s[1] = (char)(1 + ((i / 254) % 254));
	s[2] = (char)(1 + (((i / 254) / 254) % 254));
	s[3] = 'a';
	s[4] = (char)(1 + ((i*(i - 3)) % 217));
	s[5] = (char)(1 + ((17 * i + 129) % 233));
	s[6] = '\0';
}
void sample_string_C(char *s, long i)
{
	s[0] = (char)(1 + (i % 254));
	s[1] = (char)(1 + ((i / 254) % 254));
	s[2] = 'a';
	s[3] = (char)(1 + ((i*(i - 3)) % 217));
	s[4] = (char)(1 + ((17 * i + 129) % 233));
	s[5] = '\0';
}
void sample_string_D(char *s, long i)
{
	s[0] = (char)(1 + (i % 254));
	s[1] = (char)(1 + ((i / 254) % 254));
	s[2] = (char)(1 + (((i / 254) / 254) % 254));
	s[3] = 'b';
	s[4] = 'b';
	s[5] = (char)(1 + ((i*(i - 3)) % 217));
	s[6] = (char)(1 + ((17 * i + 129) % 233));
	s[7] = '\0';
}
void sample_string_E(char *s, long i)
{
	s[0] = (char)(1 + (i % 254));
	s[1] = (char)(1 + ((i / 254) % 254));
	s[2] = (char)(1 + (((i / 254) / 254) % 254));
	s[3] = 'a';
	s[4] = (char)(2 + ((i*(i - 3)) % 217));
	s[5] = (char)(1 + ((17 * i + 129) % 233));
	s[6] = '\0';
}



int main()
{
	long i, j;
	bf_t * bloom;

	unsigned seed = time(NULL);

	bloom = create_bf();

	srandom(seed);
	printf("Created Filter\n");
	for (i = 0; i< 1450000; i++)
	{
		char s[8];
		sample_string_A(s, random());
		insert_bf(bloom, s);
	}
	for (i = 0; i< 500000; i++)
	{
		char s[7];
		sample_string_B(s, random());
		insert_bf(bloom, s);
	}
	for (i = 0; i< 50000; i++)
	{
		char s[6];
		sample_string_C(s, random());
		insert_bf(bloom, s);
	}
	printf("inserted 2,000,000 strings of length 8,7,6.\n");

	srandom(seed);

	for (i = 0; i< 1450000; i++)
	{
		char s[8];
		sample_string_A(s, random());
		/*if( is_element( bloom, s ) != 1 )
		{  printf("found negative error (1)\n"); exit(0); }*/
	}
	for (i = 0; i< 500000; i++)
	{
		char s[7];
		sample_string_B(s, random());
		/*if( is_element( bloom, s ) != 1 )
		{  printf("found negative error (2)\n"); exit(0); }*/
	}
	for (i = 0; i< 50000; i++)
	{
		char s[6];
		sample_string_C(s, random());
		/*if( is_element( bloom, s ) != 1 )
		{  printf("found negative error (3)\n"); exit(0); }*/
	}
	j = 0;
	for (i = 0; i< 500000; i++)
	{
		char s[8];
		sample_string_D(s, random());
		if (is_element(bloom, s) != 0)
			j += 1;
	}
	for (i = 0; i< 500000; i++)
	{
		char s[7];
		sample_string_E(s, random());
		if (is_element(bloom, s) != 0)
			j += 1;
	}
	printf("Found %d positive errors out of 1,000,000 tests.\n", j);
	printf("Positive error rate %f%%.\n", (float)j / 10000.0);

}

