#define MSG_LEN 1024

struct Message
{
	int type;
	int length;
	char message[MSG_LEN];
};
