#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

typedef struct {
	bool playback;
	bool background;
}FLAGS;

typedef struct {
	char *name[256];
	int fc;
}FILES;

FLAGS flags;
FILES *files;

/* calculates remaing seconds from a minute double value */
int get_remaining_secs (double total_secs) {
	double remaining_secs;
	double whole_mins;

	whole_mins = (double) total_secs / 60;

	remaining_secs = (double) whole_mins - (int) whole_mins;

	return (int) remaining_secs * 60;
}

/* returns remaing minutes as int */
int get_mins (double total_secs) {
	double mins;

	mins = (double) total_secs / 60;

	return (int) mins;
}

double get_time (char *file_name) {
  double tt;
	uint8_t *buf;
	uint32_t wav_length;
	uint32_t sample_length;
	uint32_t sample_size;
	uint32_t sample_count;
	SDL_AudioSpec spec;

	/* loads file and calculates the length in seconds */
	if (SDL_LoadWAV(file_name, &spec, &buf, &wav_length) != NULL) {
		sample_length = 0;

		SDL_FreeWAV(buf);

		sample_size = SDL_AUDIO_BITSIZE(spec.format) / 8;
		sample_count = wav_length / sample_size;

		if (spec.channels)
			sample_length = wav_length / spec.channels;
		else
			sample_length = sample_count;

		tt = (double) sample_length / (double) spec.freq;
	} else return 0.0;

	return tt;
}

void print_time (char *file) {
	double total_time, raw_time;
	int mins, secs;

	raw_time = get_time(file);

	mins = get_mins(raw_time);
	secs = get_remaining_secs(raw_time);
	fprintf(stdout, "Total time %dm %ds (%0.2f total seconds)\n", mins, secs, raw_time);
}

void data_callback (ma_device *device, void *output, const void *input, ma_uint32 frame_count) {
	ma_decoder *decoder;

	decoder = (ma_decoder*)device->pUserData;

	if (decoder == NULL) return;

	ma_decoder_read_pcm_frames(decoder, output, frame_count, NULL);
	(void)input;
}

int audio_playback (const char *file) {
	ma_result result;
	ma_decoder decoder;
	ma_device_config device_config;
	ma_device device;

	result = ma_decoder_init_file(file, NULL, &decoder);

	if (result != MA_SUCCESS) {
		fprintf(stderr, "ERROR: Couldn't load file\n");
		return -1;
	}

	device_config = ma_device_config_init(ma_device_type_playback);
	device_config.playback.format = decoder.outputFormat;
	device_config.playback.channels = decoder.outputChannels;
	device_config.sampleRate = decoder.outputSampleRate;
	device_config.dataCallback = data_callback;
	device_config.pUserData = &decoder;

	/* opens playback device */
	if (ma_device_init(NULL, &device_config, &device) != MA_SUCCESS) {
		fprintf(stderr, "ERROR: failed to open playback device\n");
		ma_decoder_uninit(&decoder);
		return -1;
	}

	/* starts playback device */
	if (ma_device_start(&device) != MA_SUCCESS) {
		fprintf(stderr, "ERROR: failed to start playback device\n");
		ma_device_uninit(&device);
		ma_decoder_uninit(&decoder);
		return -1;
	}

	getchar();

	ma_device_uninit(&device);
	ma_decoder_uninit(&decoder);
	return 0;
}

/* makes sure file exists and can be opened */
bool is_file (char *file) {
	FILE *fp;

	fp = fopen(file, "r");

	if (fp == NULL) {
		fprintf(stderr, "ERROR: No such file or directory\n");
		return false;
	}

	fclose(fp);
	return true;
}

/* sees if file is of a valid type */
bool is_valid (char *file) {
	char *tmp = (char*)malloc(sizeof(*file));
	const char *types[] = { ".wav", ".mp3", ".ogg", };
	size_t i = 0;

	/* find index of the . in the file name */
	for (; file[i] != (char) 0; i++)
		if (file[i] == '.') break;

	/* place file extension in string */
	for (; i < strlen(file); i++)
		strncat(tmp, &file[i], 1);

	/* see if type is supported */ 
	for (size_t i = 0; types[i] != NULL; i++)
		if (!strncmp(types[i], tmp, strlen(types[i]))) {
			memset(tmp, 0, sizeof((char*)tmp));
			free(tmp);
			return true;
		}

	fprintf(stderr, "ERROR: Invalid file type %s\n%s\n", file, tmp);
	memset(tmp, 0, sizeof((char*)tmp));
	free(tmp);
	return false;
}

/* just sets all flags to false */
void set_defs (void) {
	files->fc = 0;
	flags.playback = false;
	flags.background = false;
}

int parse_args (int argc, char **argv) {
	if (argv[1] == NULL)
		return -1;

	/* looks for flags and if not then set a file to play */
	for (int i = 1, j = 0; i < argc && argv[i] != NULL; i++) {
		if (argv[i][0] == '-' && argv[i][1] != (char) 0)
			for (int k = 1; argv[i][k] != (char) 0; k++)
				switch (argv[i][k]) {
					case 'p':
						flags.playback = true;
						break;
					case 'b':
						flags.background = true;
						break;
					default:
						fprintf(stderr, "No such flag %c\n", argv[i][k]);
						return -1;
				}
		else {
			files->name[j] = strdup(argv[i]);
			files->fc++;
			j++;
		}
	}
	return 0;
}

/* a simple setup function for the program */
int global_setup (int argc, char **argv) {
	files = (FILES*)malloc(sizeof(*files));

	if (files == NULL) {
		fprintf(stderr, "ERROR: malloc(files) in global_setup()\n");
		return -1;
	}

	set_defs();

	if (parse_args(argc, argv) != 0)
		return -1;

	for (int i = 0; files->name[i] != NULL; i++)
		if (is_file(files->name[i]) != true || is_valid(files->name[i]) != true)
			return -1;

	for (int i = 0; i < files->fc; i++) {
		print_time(files->name[i]);
		fprintf(stdout, "Press ENTER to quit ...\n");
		audio_playback(files->name[i]);
	}

	free(files);
	return 0;
}

int main (int argc, char *argv[]) {
	return global_setup(argc,argv);
}
