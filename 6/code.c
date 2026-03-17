#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NUM_TEXTS 10
#define NUM_PATTERNS 10
#define TEXT_STEP 1000
#define PATTERN_STEP 10

#define RK_BASE 256LL
#define RK_MOD 1000003LL

typedef struct {
	char path[256];
	char *data;
	int size;
} FileBuffer;

static int ensure_dir(const char *dir_path) {
	struct stat st;
	if (stat(dir_path, &st) == 0 && S_ISDIR(st.st_mode)) {
		return 1;
	}
	if (mkdir(dir_path, 0777) == 0) {
		return 1;
	}
	return 0;
}

static char *read_file_binary(const char *path, int *out_size) {
	FILE *fp = fopen(path, "rb");
	if (!fp) return NULL;

	if (fseek(fp, 0, SEEK_END) != 0) {
		fclose(fp);
		return NULL;
	}

	long fsize = ftell(fp);
	if (fsize < 0) {
		fclose(fp);
		return NULL;
	}
	rewind(fp);

	char *buf = (char *)malloc((size_t)fsize + 1);
	if (!buf) {
		fclose(fp);
		return NULL;
	}

	size_t read_n = fread(buf, 1, (size_t)fsize, fp);
	fclose(fp);
	buf[read_n] = '\0';
	*out_size = (int)read_n;
	return buf;
}

static int write_bytes(const char *path, const char *data, int n) {
	FILE *fp = fopen(path, "wb");
	if (!fp) return 0;
	size_t w = fwrite(data, 1, (size_t)n, fp);
	fclose(fp);
	return (w == (size_t)n);
}

static int ensure_input_files_exist(void) {
	FILE *tf = fopen("text_files.txt", "r");
	FILE *pf = fopen("pattern_files.txt", "r");

	if (tf && pf) {
		fclose(tf);
		fclose(pf);
		return 1;
	}
	if (tf) fclose(tf);
	if (pf) fclose(pf);

	if (!ensure_dir("inputs")) return 0;
	if (!ensure_dir("inputs/texts")) return 0;
	if (!ensure_dir("inputs/patterns")) return 0;

	char *corpus = NULL;
	int corpus_len = 0;
	corpus = read_file_binary("kaggle_source.txt", &corpus_len);
	if (!corpus || corpus_len < 10000) {
		if (corpus) free(corpus);
		printf("Missing or too-small Kaggle source file: kaggle_source.txt (need >= 10000 chars).\n");
		printf("Place a text file extracted from Kaggle dataset in folder 6 with this exact name.\n");
		return 0;
	}

	FILE *text_list = fopen("text_files.txt", "w");
	FILE *pattern_list = fopen("pattern_files.txt", "w");
	if (!text_list || !pattern_list) {
		if (text_list) fclose(text_list);
		if (pattern_list) fclose(pattern_list);
		free(corpus);
		return 0;
	}

	srand((unsigned int)time(NULL));

	for (int i = 1; i <= NUM_TEXTS; i++) {
		int size = i * TEXT_STEP;
		char path[256];
		snprintf(path, sizeof(path), "inputs/texts/text_%dk.txt", i);

		char *buf = (char *)malloc((size_t)size);
		if (!buf) {
			fclose(text_list);
			fclose(pattern_list);
			free(corpus);
			return 0;
		}

		for (int j = 0; j < size; j++) {
			buf[j] = corpus[j % corpus_len];
		}

		if (!write_bytes(path, buf, size)) {
			free(buf);
			fclose(text_list);
			fclose(pattern_list);
			free(corpus);
			return 0;
		}
		free(buf);
		fprintf(text_list, "%s\n", path);
	}

	for (int i = 1; i <= NUM_PATTERNS; i++) {
		int psize = i * PATTERN_STEP;
		char path[256];
		snprintf(path, sizeof(path), "inputs/patterns/pattern_%d.txt", psize);
		char *pat = (char *)malloc((size_t)psize);
		if (!pat) {
			fclose(text_list);
			fclose(pattern_list);
			free(corpus);
			return 0;
		}

		if (i % 2 == 1) {
			int start = rand() % (corpus_len - psize + 1);
			memcpy(pat, corpus + start, (size_t)psize);
		} else {
			// Spurious pattern: start from real corpus slice, then mutate one byte.
			int start = rand() % (corpus_len - psize + 1);
			memcpy(pat, corpus + start, (size_t)psize);
			for (int j = 0; j < psize; j++) {
				if (j == psize / 2) {
					pat[j] = (char)(pat[j] == 'Z' ? 'A' : (pat[j] + 1));
				}
			}
		}

		if (!write_bytes(path, pat, psize)) {
			free(pat);
			fclose(text_list);
			fclose(pattern_list);
			free(corpus);
			return 0;
		}
		free(pat);
		fprintf(pattern_list, "%s\n", path);
	}

	fclose(text_list);
	fclose(pattern_list);
	free(corpus);
	return 1;
}

static int load_file_list(const char *list_path, FileBuffer arr[], int expected) {
	FILE *fp = fopen(list_path, "r");
	if (!fp) return 0;

	char line[256];
	int count = 0;
	while (count < expected && fgets(line, sizeof(line), fp)) {
		size_t len = strlen(line);
		while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
			line[--len] = '\0';
		}
		if (len == 0) continue;

		strncpy(arr[count].path, line, sizeof(arr[count].path) - 1);
		arr[count].path[sizeof(arr[count].path) - 1] = '\0';
		arr[count].data = read_file_binary(arr[count].path, &arr[count].size);
		if (!arr[count].data) {
			fclose(fp);
			return 0;
		}
		count++;
	}

	fclose(fp);
	return (count == expected);
}

static long long rabin_karp_count(const char *text, int n, const char *pattern, int m) {
	if (m == 0 || n == 0 || m > n) return 0;

	long long pattern_hash = 0;
	long long window_hash = 0;
	long long h = 1;
	long long matches = 0;

	for (int i = 0; i < m - 1; i++) {
		h = (h * RK_BASE) % RK_MOD;
	}

	for (int i = 0; i < m; i++) {
		pattern_hash = (RK_BASE * pattern_hash + (unsigned char)pattern[i]) % RK_MOD;
		window_hash = (RK_BASE * window_hash + (unsigned char)text[i]) % RK_MOD;
	}

	for (int i = 0; i <= n - m; i++) {
		if (pattern_hash == window_hash) {
			int equal = 1;
			for (int j = 0; j < m; j++) {
				if (text[i + j] != pattern[j]) {
					equal = 0;
					break;
				}
			}
			if (equal) matches++;
		}

		if (i < n - m) {
			long long left = ((unsigned char)text[i] * h) % RK_MOD;
			window_hash = (RK_BASE * (window_hash - left + RK_MOD) + (unsigned char)text[i + m]) % RK_MOD;
		}
	}

	return matches;
}

int main(void) {
	if (!ensure_input_files_exist()) {
		printf("Failed to prepare input files with Kaggle-only policy.\n");
		return 1;
	}

	FileBuffer texts[NUM_TEXTS] = {0};
	FileBuffer patterns[NUM_PATTERNS] = {0};

	if (!load_file_list("text_files.txt", texts, NUM_TEXTS)) {
		printf("Could not load text files from text_files.txt\n");
		return 1;
	}
	if (!load_file_list("pattern_files.txt", patterns, NUM_PATTERNS)) {
		printf("Could not load pattern files from pattern_files.txt\n");
		for (int i = 0; i < NUM_TEXTS; i++) free(texts[i].data);
		return 1;
	}

	FILE *combo_fp = fopen("timings_combinations.csv", "w");
	FILE *plot_fp = fopen("timings_plot.csv", "w");
	if (!combo_fp || !plot_fp) {
		printf("Could not open output CSV files.\n");
		if (combo_fp) fclose(combo_fp);
		if (plot_fp) fclose(plot_fp);
		for (int i = 0; i < NUM_TEXTS; i++) free(texts[i].data);
		for (int i = 0; i < NUM_PATTERNS; i++) free(patterns[i].data);
		return 1;
	}

	fprintf(combo_fp, "TextFile,TextSize,PatternFile,PatternSize,TimeSeconds,Matches\n");
	fprintf(plot_fp, "TextSize");
	for (int p = 1; p <= NUM_PATTERNS; p++) {
		fprintf(plot_fp, ",P%d", p * PATTERN_STEP);
	}
	fprintf(plot_fp, "\n");

	printf("Rabin-Karp Timing Results (100 combinations)\n");
	printf("---------------------------------------------------------------\n");

	for (int t = 0; t < NUM_TEXTS; t++) {
		fprintf(plot_fp, "%d", texts[t].size);

		for (int p = 0; p < NUM_PATTERNS; p++) {
			clock_t start = clock();
			long long matches = rabin_karp_count(
				texts[t].data,
				texts[t].size,
				patterns[p].data,
				patterns[p].size
			);
			clock_t end = clock();

			double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

			fprintf(combo_fp, "%s,%d,%s,%d,%.9f,%lld\n",
					texts[t].path, texts[t].size,
					patterns[p].path, patterns[p].size,
					elapsed, matches);

			fprintf(plot_fp, ",%.9f", elapsed);

			printf("T=%5d bytes | P=%3d bytes | time=%0.9f s | matches=%lld\n",
				   texts[t].size, patterns[p].size, elapsed, matches);
		}
		fprintf(plot_fp, "\n");
	}

	fclose(combo_fp);
	fclose(plot_fp);

	for (int i = 0; i < NUM_TEXTS; i++) free(texts[i].data);
	for (int i = 0; i < NUM_PATTERNS; i++) free(patterns[i].data);

	printf("---------------------------------------------------------------\n");
	printf("Created files:\n");
	printf("1) text_files.txt and pattern_files.txt (input manifests)\n");
	printf("2) timings_combinations.csv (all 100 combinations)\n");
	printf("3) timings_plot.csv (XY plot-ready matrix)\n");
	printf("Use timings_plot.csv for plotting with pattern size on X-axis.\n");

	return 0;
}
