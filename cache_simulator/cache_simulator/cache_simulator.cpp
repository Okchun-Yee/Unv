#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 256*4

typedef struct {
	int setIndexBits;
	int associativity;
	int blockSize;
	char* writePolicy;
	char* allocationPolicy;
	char* replacementPolicy;
	char* traceFile;
} CacheConfig;

typedef struct {
	int valid;	//유효 flag
	int tag;	//Tag
	int* data;	//data block
	int lru_cnt; //lru 교체용
} CacheLine;

//last-output val
unsigned int load_hit, load_miss;
unsigned int save_hit, save_miss;
unsigned int total_c, total_load, total_save;

void initCache(CacheLine* cache, int numSets, int associativity, int blockSize) {
	for (int i = 0; i < numSets * associativity; i++) {
		cache[i].valid = 0;
		cache[i].tag = -1;
		cache[i].lru_cnt = 0; // LRU 필드 초기화
		cache[i].data = (int*)malloc(blockSize * sizeof(int));
		for (int j = 0; j < blockSize; j++) {
			cache[i].data[j] = 0;
		}
	}
}

//cache 메모리 해제
void freeCache(CacheLine* cache, int numSets, int associativity) {
	for (int i = 0; i < numSets * associativity; i++) {
		free(cache[i].data);
	}
}

// LRU 카운터 업데이트 함수
void update_lru(CacheLine* cache, int setStart, int associativity, int accessedIndex) {
	for (int i = 0; i < associativity; i++) {
		if (cache[setStart + i].valid) {
			cache[setStart + i].lru_cnt++; // 모든 유효한 캐시 라인의 LRU 증가
		}
	}
	cache[accessedIndex].lru_cnt = 0; // 액세스된 캐시 라인의 LRU 초기화
}

// LRU 방식으로 교체할 캐시 라인 찾기 함수
int findLRU(CacheLine* cache, int setStart, int associativity) {
	int lruIndex = setStart;
	for (int i = 1; i < associativity; i++) {
		if (cache[setStart + i].lru_cnt > cache[lruIndex].lru_cnt) {
			lruIndex = setStart + i; // LRU 값이 가장 큰 인덱스 찾기
		}
	}
	return lruIndex;
}

void simulateCache(CacheConfig config) {
	//cache size 계산
	int numSets = 1 << config.setIndexBits;
	int associativity = config.associativity; //sets 당 블록 설정
	int cacheSize = numSets * associativity;
	CacheLine* cache = (CacheLine*)malloc(cacheSize * sizeof(CacheLine));
	// cache 초기화
	initCache(cache, numSets, associativity, config.blockSize);

	// Read file
	FILE* file = fopen(config.traceFile, "r");
	if (file == NULL) {
		fprintf(stderr, "Error: Cannot open trace file %s\n", config.traceFile);
		exit(EXIT_FAILURE);
	}
	else printf("File, open.\n");

	char line[MAX_LINE_LENGTH];
	char letter = NULL; char hex[11] = { '\0' }; int number = 0;

	//======================Read Cache==============================
	while (fgets(line, sizeof(line), file) != NULL) {
		if (sscanf(line, "%c %s %d", &letter, hex, &number) != 3) {
			fprintf(stderr, "잘못된 형식의 라인: %s\n", line);
			continue;
		}
		//메모리주소에서 0x 지우기
		if (strlen(hex) >= 2 && hex[0] == '0' && hex[1] == 'x') {
			memmove(hex, hex + 2, strlen(hex) - 1);
		}
		unsigned long address = strtoul(hex, NULL, 16);

		int setIndex = (address >> config.blockSize) & (numSets - 1);
		int tag = address >> (config.blockSize + config.setIndexBits);
		int setStart = setIndex * associativity;
		int hit = 0;

		//cache에서 tag 찾기
		for (int i = 0; i < associativity; i++) {
			if (cache[setStart + i].valid && cache[setStart + i].tag == tag) {
				hit = 1;
				update_lru(cache, setStart, associativity, setStart + i);
				break;
			}
		}
		// Check LOAD or SAVE : cache hit or miss 
		//LOAD 찾기
		if (letter == 'l') {
			total_c++; total_load++;
			if (hit == 1) {
				load_hit++;
				printf("LOAD : Cache hit :  0x%8lx\n", address);
			}
			else {
				load_miss++;
				printf("LOAD : Cache miss:  0x%8lx\n", address);
				// Update cache
				int replaceIndex = findLRU(cache, setStart, config.associativity);
				printf("Replacing block in set %d, way %d\n", setIndex, replaceIndex - setStart);
				cache[replaceIndex].valid = 1;
				cache[replaceIndex].tag = tag;
				update_lru(cache, setStart, associativity, replaceIndex); //LRU로 cache 교체

				// Simulate fetching
				for (int i = 0; i < associativity; i++) {
					cache[replaceIndex].data[i] = address + i;	//교체한 값 할당
				}

				//research
				int re_hit = 0;
				for (int i = 0; i < associativity; i++) {
					total_c++;
					if (cache[setStart + i].valid && cache[setStart + i].tag == tag) {
						re_hit = 1; 
						load_hit++; load_miss--;
						update_lru(cache, setStart, associativity, setStart + i);
						break;
					}
				}
			}
		}
		//save 찾기
		else if (letter == 's') {
			total_c++; total_save++;
			if (hit == 1) {
				save_hit++;
				printf("SAVE : Cache hit :  0x%8lx\n", address);
			}
			else {
				save_miss++;
				printf("SAVE : Cache miss:  0x%8lx\n", address);
				// Update cache
				int replaceIndex = findLRU(cache, setStart, associativity);
				printf("Replacing block in set %d, way %d\n", setIndex, replaceIndex - setStart);
				cache[replaceIndex].valid = 1;
				cache[replaceIndex].tag = tag;
				update_lru(cache, setStart, associativity, replaceIndex);
				// Simulate fetching
				for (int i = 0; i < config.blockSize; i++) {
					cache[replaceIndex].data[i] = address + i;
				}

				int re_hit = 0;
				for (int i = 0; i < associativity; i++) {
					total_c++;
					if (cache[setStart + i].valid && cache[setStart + i].tag == tag) {
						re_hit = 1;
						save_hit++; save_miss--;
						update_lru(cache, setStart, associativity, setStart + i);
						break;
					}
				}
			}
		}
	}
	printf("총 로드 횟수       : %8d\n", total_load);
	printf("총 저장 횟수       : %8d\n", total_save);
	printf("LOAD - HIT 횟수    : %8d\n", load_hit);
	printf("LOAD - MISS 횟수   : %8d\n", load_miss);
	printf("SAVE - HIT 횟수    : %8d\n", save_hit);
	printf("SAVE - MISS 횟수   : %8d\n", save_miss);
	printf("TOTAL - CYCLE 횟수 : %8d\n", total_c);
	fclose(file);
	// Free cache memory

	freeCache(cache, cacheSize, associativity);
	free(cache);
}

CacheConfig parseArgs(int argc, char* argv[]) {
	CacheConfig config;
	if (argc != 8) {
		printf("Usage: ./cache_simulator <s> <E> <b> <write_policy> <allocation_policy> <replacement_policy> <tracefile>\n");
		exit(EXIT_FAILURE);
	}

	// Parse arguments
	config.setIndexBits = atoi(argv[1]);
	config.associativity = atoi(argv[2]);
	config.blockSize = atoi(argv[3]);
	config.writePolicy = argv[4];
	config.allocationPolicy = argv[5];
	config.replacementPolicy = argv[6];
	config.traceFile = argv[7];

	printf("cache : %d | %d | %d | %s | %s | %s | %s\n",
		config.setIndexBits, config.associativity, config.blockSize, config.writePolicy, config.allocationPolicy, config.replacementPolicy, config.traceFile);

	return config;
}

int main(int argc, char* argv[]) {
	CacheConfig config = parseArgs(argc, argv);
	simulateCache(config);
	return 0;
}
