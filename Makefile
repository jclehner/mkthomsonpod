mkthomsonpod: mkthomsonpod.c
	$(CC) mkthomsonpod.c -o mkthomsonpod -Wall -lcrypto

clean:
	rm -f mkthomsonpod
