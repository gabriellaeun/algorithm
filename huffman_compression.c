#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_TREE_HT 100    
#define ALPHABET_SIZE 69
#define B_FILENAME "binary_hw1.hfb"
#define T_FILENAME "txt_hw1.hft"

// 허프만 트리 노드
typedef struct MinHeapNode
{
	char data;
	int freq;
	int* array;
	struct MinHeapNode *left;
	struct MinHeapNode *right;
}MinHeapNode;

typedef struct MinHeap
{
	int size;
	int capacity;
	MinHeapNode **array;
}MinHeap;


typedef struct Table {
	char data;
	char huffmancode[MAX_TREE_HT];
}Table;





MinHeapNode* ALLOCATE_NODE(char data, int freq);
void SWAP_NODE(MinHeapNode** a, MinHeapNode** b);
void MIN_HEAPIFY(MinHeap* minHeap, int index);
int IS_HEAP_SIZE_ONE(MinHeap* minHeap);
MinHeapNode* EXTRACT_MIN(MinHeap* minHeap);
void INSERT_MIN_HEAP(MinHeap* minHeap, MinHeapNode* minHeapNode);
int IS_LEAF(MinHeapNode* root);
MinHeap* BUILD_MIN_HEAP(char* data, int* freq, int size);
MinHeapNode* BUILD_HUFFMAN_TREE(char* data, int* freq, int size);
void MAKE_HUFFMAN_CODE(MinHeapNode* root, int* arr, int top);
void ENCODING(char* story, int size);
MinHeapNode* HUFFMAN(char* data, int* freq, int size);
int compare(const void *A, const void  *B);
long file_length(char filename[]);



MinHeapNode* root;
Table* table;
int k = 0;
long length_f, length_t, length_b;
float com_t, com_b;

int main(int argc, char* argv[]) {
	FILE *fp; //파일 입출력 변수
	FILE *fp_table;

	int cnt = 0;    //파일 입력 변수 개수
	int i, asc_big, asc_small, asc_num;
	int asc_CR;
	char *story = (char*)malloc(sizeof(char));    //문자열 배열
	char *letter = (char*)malloc(sizeof(char) * 69);   
	int *frequency = (int*)malloc(sizeof(int) * 69);    //위의 배열에 대한 빈도수를 저장하는 배열
	table = (Table*)malloc(sizeof(Table) * 69);    //알파벳과 빈도수에 대한 테이블을 저장하는 구조체 배열
	if (!(fp = fopen(B_FILENAME, "rt")) {
		printf("**** File open error ****\n");
		exit(1);
	}//파일 읽기
	
	//story라는 배열로 알파벳을 받아옴.
	while (!feof(fp))
	{
		story = (char*)realloc(story, sizeof(char)*(cnt + 1));
		fscanf(fp, "%c", &story[cnt]);
		cnt++;
	}
	//모든 데이터와 빈도수의 배열을 0으로 초기화 해줌.
	for (i = 0; i < 68; i++)
	{
		letter[i] = ' ';
		frequency[i] = 0;
	}
	//받아온 문자열을 알파벳을 저장하는 배열과 빈도수를 저장하는 배열로 나누어서 저장해준다.
	for (i = 0; i < cnt; i++)
	{
		asc_big = story[i] - 65;
		asc_small = story[i] - 71;
		asc_num = story[i] + 4;
		asc_CR = story[i] - 13;

		//띄어쓰기 ' '일때는 따로 관리해줘야한다.
		if (story[i] == ' ') {
			letter[62] = story[i];
			frequency[62]++;
		}
		else if (story[i] == ',') {
			letter[63] = story[i];
			frequency[63]++;
		}
		else if (story[i] == '.') {
			letter[64] = story[i];
			frequency[64]++;
		}
		else if (story[i] == '!') {
			letter[65] = story[i];
			frequency[65]++;
		}
		else if (story[i] == '?') {
			letter[66] = story[i];
			frequency[66]++;
		}
		else if (story[i] == '-') {
			letter[67] = story[i];
			frequency[67]++;
		}
		else if (asc_CR == 0) {
			letter[68] = story[i];
			frequency[68]++;
		}
		else if (isupper(story[i])) {
			letter[asc_big] = story[i];
			frequency[asc_big]++;
		}
		else if (islower(story[i])) {
			letter[asc_small] = story[i];
			frequency[asc_small]++;
		}
		else {
			letter[asc_num] = story[i];
			frequency[asc_num]++;
		}
	}
	root = HUFFMAN(letter, frequency, ALPHABET_SIZE);    //허프만 트리를 만들고 ROOT값을 가져온다.

	fclose(fp);
	qsort(table, ALPHABET_SIZE, sizeof(Table), compare);    //테이블을 알파벳기준 정렬을 한다.

	fp_table = fopen("D:\\txt_hw1.hft", "w");    //테이블을 텍스트 파일로 만들어준다.
	if (fp_table == NULL)    //파일 없다면 에러 처리
	{
		printf("**** File open error ****\n");
		system("pause");
		exit(1);
	}

	for (i = 0; i < ALPHABET_SIZE; i++) {
		fprintf(fp_table, "%c, %s\n", table[i].data, table[i].huffmancode);
	}
	length_f = file_length(argv[1]);
	length_t = file_length(T_FILENAME);
	com_t = length_t / length_f;

	printf("원본 파일 크기 : %ld\n", length_f);
	printf("순수한 압축 파일 크기 : %ld   압축률 : %f\n", length_t, com_t);
	fclose(fp_table);

	ENCODING(story, cnt - 1);    //인코딩
	return 0;
}

//허프만트리를 만들기 위한 함수이다.
MinHeapNode* HUFFMAN(char* data, int* freq, int size)
{
	int i = 0;
	MinHeapNode* root = BUILD_HUFFMAN_TREE(data, freq, size);        //허프만 트리를 만든다.
	int top = 0;
	root->array = (int*)malloc(MAX_TREE_HT);
	MAKE_HUFFMAN_CODE(root, root->array, top);
	return root;
}

//허프만 트리를 만드는 함수
MinHeapNode* BUILD_HUFFMAN_TREE(char* data, int* freq, int size)
{
	MinHeapNode *left, *right, *top;    //트리에 필요한 top과 left,right를 만들어준다.


	MinHeap* minHeap = BUILD_MIN_HEAP(data, freq, size);        //허프만트리를 만들기위한 Min Heap을 만들어준다.


	//minHeap의 size가 1일때까지 반복한다. 허프만 트리 만드는 과정
	while (!IS_HEAP_SIZE_ONE(minHeap))
	{
		//가장 빈도수 낮은 두 값을 빼온다.
		left = EXTRACT_MIN(minHeap);
		right = EXTRACT_MIN(minHeap);
		//printf("left : %d   right : %d\n",left->data,right->data);
		//$는 leaf노드와 아닌 노드들을 구별하기위해서 만들었다.
		//허프만 트리를 만들어준다.
		//빈도수 대로 추출해서 왼쪽 오른쪽으로 계속해서 추가해 나간다.
		//가장 작은 노드들 두개를 더해서 하나의 노드 TOP 로 놓는다.
		top = ALLOCATE_NODE('$', left->freq + right->freq);
		top->left = left;
		top->right = right;
		INSERT_MIN_HEAP(minHeap, top);
	}

	printf("%c\n\n", EXTRACT_MIN(minHeap)->data);
	//return minHeap->array[0];
	return EXTRACT_MIN(minHeap);
}


//새로운 노드를 만들어준다. MinHeap에 저장을 하기 위해서.
MinHeapNode* ALLOCATE_NODE(char data, int freq)
{
	MinHeapNode* temp = (MinHeapNode*)malloc(sizeof(MinHeapNode));
	temp->left = NULL;
	temp->right = NULL;
	temp->data = data;
	temp->freq = freq;
	return temp;
}

//MIN_HEAP으로 만드는 HEAPIFY과정을 보인다.
void MIN_HEAPIFY(MinHeap* minHeap, int index)
{
	int smallest = index;
	int left = 2 * index + 1;
	int right = 2 * index + 2;

	if (left < minHeap->size &&minHeap->array[left]->freq < minHeap->array[smallest]->freq)
	{
		smallest = left;
	}
	if (right < minHeap->size &&minHeap->array[right]->freq < minHeap->array[smallest]->freq)
	{
		smallest = right;
	}
	if (smallest != index)
	{
		SWAP_NODE(&minHeap->array[smallest], &minHeap->array[index]);
		MIN_HEAPIFY(minHeap, smallest);
	}
}

//힙의 사이즈가 1인지 아닌지 판단해주는 함수
int IS_HEAP_SIZE_ONE(MinHeap* minHeap)
{
	return (minHeap->size == 1);
}

//Root에 있는 원소를 추출후 다시 MinHeap을 만드는 과정이다.
MinHeapNode* EXTRACT_MIN(MinHeap* minHeap)
{
	MinHeapNode* temp = minHeap->array[0];
	minHeap->array[0] = minHeap->array[minHeap->size - 1];
	minHeap->size--;
	MIN_HEAPIFY(minHeap, 0);
	return temp;
}

//구성된 MIN_HEAP에 새로운 NODE(top,left,right)를 추가시켜주는 과정이다.
void INSERT_MIN_HEAP(MinHeap* minHeap, MinHeapNode* minHeapNode)
{
	int i;
	minHeap->size++;    //Min Heap의 사이즈를 하나 추가시켜준다.

	i = minHeap->size - 1;    //마지막 인덱스

	//빈도수를 비교해가며 어디에 들어가야할지 인덱스를 찾는 과정이다.
	while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq)
	{
		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}
	minHeap->array[i] = minHeapNode;
}

//MIN_HEAP 만든다.
MinHeap* BUILD_MIN_HEAP(char* data, int* freq, int size)
{
	int i, n;

	MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
	minHeap->size = size;
	minHeap->capacity = size;
	minHeap->array = (MinHeapNode**)malloc(minHeap->capacity * sizeof(MinHeapNode*));

	//만들어준 MinHeap의 각 노드에 알파벳과 빈도수를 넣어준다.
	for (i = 0; i < size; i++)
	{
		minHeap->array[i] = ALLOCATE_NODE(data[i], freq[i]);
	}

	//minHeap을 Heapify등을 해서 넘겨준다.
	n = (minHeap->size - 1) / 2;
	for (i = n; i >= 0; i--)
	{
		MIN_HEAPIFY(minHeap, i);
	}
	return minHeap;
}

//허프만코드를 만드는 과정이다.
//leaf일때까지 찾아간다.
void MAKE_HUFFMAN_CODE(MinHeapNode* root, int* arr, int top)
{
	int i;
	//왼쪽이 있다면 배열에 0을 추가하고 왼쪽 자식으로 이동한다.
	if (root->left)
	{
		arr[top] = 0;
		MAKE_HUFFMAN_CODE(root->left, arr, top + 1);
	}
	//오른쪽이 있다면 배열에 1을 추가하고 오른쪽 자식으로 이동한다.
	if (root->right)
	{
		arr[top] = 1;
		MAKE_HUFFMAN_CODE(root->right, arr, top + 1);
	}
	//leaf일때 그에 해당하는 알파벳과 허프만코드를 저장한다.
	if (IS_LEAF(root))
	{
		//root->data 에는 허프만코드의 알파벳
		table[k].data = root->data;

		//arr에는 허프만코드가 들어있음.
		for (i = 0; i < top; i++) {
			table[k].huffmancode[i] = arr[i] + 48;
			//printf("%c\n",table[k].huffmancode[i]);
		}
		table[k].huffmancode[i] = '\0';
		k++;
	}
}


//인코딩 함수
void ENCODING(char* story, int size) {
	int i = 0;
	FILE *fp = fopen("D:\\binary_hw1.hfb", "wb");
	//테이블에 index값으로 접근하여 문자열들을 허프만코드로 바꾸어준다.
	for (i = 0; i < size; i++) {
		//띄어쓰기 ' '일때는 아스키코드로 바로 index 접근을 할수 없기때문에 따로 만들어준다.
		if (story[i] == ' ') {
			fprintf(fp, "%s", table[0].huffmancode);
		}
		else if (story[i] == ',') {
			fprintf(fp, "%s", table[1].huffmancode);
		}
		else if (story[i] == '.') {
			fprintf(fp, "%s", table[2].huffmancode);
		}
		else if (story[i] == '!') {
			fprintf(fp, "%s", table[3].huffmancode);
		}
		else if (story[i] == '?') {
			fprintf(fp, "%s", table[4].huffmancode);
		}
		else if (story[i] == '-') {
			fprintf(fp, "%s", table[5].huffmancode);
		}
		else if (isupper(story[i])) {
			fprintf(fp, "%s", table[story[i] - 59].huffmancode);
		}
		else if(islower(story[i]{
			fprintf(fp, "%s", table[story[i] - 65].huffmancode);
		}
		else {
			fprintf(fp, "%s", table[story[i] + 57].huffmancode);
		}
	}
	fclose(fp);
	
	length_b = file_length_b(B_FILENAME);
	com_b = length_b / length_f;
	printf("Huffman 인코딩 압축 파일 크기 : %ld   압축률 : %f\n", length_b, com_b);
	printf("*******ENCODING SUCCESS********\n");
}

//leaf 노드인가 아닌가에 대해 판단해주는 함수이다.
int IS_LEAF(MinHeapNode* root)
{
	return !(root->left) && !(root->right);
}

void SWAP_NODE(MinHeapNode** a, MinHeapNode** b)
{
	MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

int compare(const void *A, const void  *B)
{
	Table* ptrA = (Table*)A;
	Table* ptrB = (Table*)B;

	char a = ptrA->data, b = ptrB->data;

	if (a == b) return 0;
	else if (a > b) return 1;
	else return -1;
}

long file_length(char filename[]) {
	FILE *fp;
	long l;

	if ((fp = fopen(filename, "r+")) == NULL)
		return 0L;

	fseek(fp, 0, SEEK_END);
	l = ftell(fp);
	fclose(fp);

	return l;
}

long file_length_b(char filename[]) {
	FILE *fp;
	long l;

	if ((fp = fopen(filename, "rb")) == NULL)
		return 0L;

	fseek(fp, 0, SEEK_END);
	l = ftell(fp);
	fclose(fp);

	return l;
}
