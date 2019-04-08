#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <getopt.h>

static void do_memo(char* channel, char *message);

#define DEFAULT_CHANNEL "random"

static struct option longopts[] = {
	{"channel", required_argument, NULL, 'c'},
	{0, 0, 0, 0}
};

int main(int argc, char *argv[]){
	int opt;
	char name_channel[64] = DEFAULT_CHANNEL;

	while ((opt = getopt_long(argc, argv, "c:", longopts, NULL)) != -1) {
		switch (opt) {
			case 'c':
				strncpy(name_channel, optarg, 49);
				break;
			case '?':
				fprintf(stderr, "Usage: %s [-c CHANNEL] [NAME] [contents]\n", argv[0]);
				exit(1);
		}
	}

	if (optind == argc) {
		fprintf(stderr, "Usage: %s [contents]\n", argv[0]);
		exit(1);
	} else {
		do_memo(name_channel, argv[optind]);
	}
	exit(0);
}

static void do_memo(char* channel, char *message) {
	FILE *ftoken;
	int c;
	char *p;
	char token[80];
	ftoken = fopen(".accesstoken", "r");
	if (!ftoken) {
		perror(".accesstoken");
		exit(1);
	}
	if (fgets(token, 80, ftoken)) {
		p = strchr(token, '\n');
		if (p) {
			*p = '\0';
		}
	}
	fclose(ftoken);
	char send_message[256];
	sprintf(send_message, "curl -XPOST -d 'token=%s&channel=%s&text=%s' https://slack.com/api/chat.postMessage 1>/dev/null 2>&1", token, channel, message);
	system(send_message);
	fprintf(stdout, "channel:%s, message:%s\n", channel, message);
}
