//Program uruchamiany z argumentami
//1. to tryb szyfrowanie/deszyfrowanie
//2. to plik który mamy szyfrować, bądź deszyfrować.
//3. to plik z którego bierzemy nasz klucz.
//4. to plik od którego bierzemy identyfikator klucza
#include<stdio.h>
#include<openssl/aes.h>
#include<openssl/evp.h>
#include<string.h>
#include<termios.h>
#include<unistd.h>
int do_crypt(FILE *in, FILE *out, int do_encrypt, FILE *keys, FILE *svi)
{
	unsigned char inbuf[1024], outbuf[1024+EVP_MAX_BLOCK_LENGTH];
	int intlen, outlen;
	EVP_CIPHER_CTX ctx;
	unsigned char key[16];
	unsigned char iv[16];
	while(!feof(keys)){
		fscanf(keys, "%s", key);
	}
	while(!feof(svi)){
		fscanf(svi, "%s", iv);
	}
	EVP_CIPHER_CTX_init(&ctx);
	EVP_CipherInit_ex(&ctx, EVP_aes_128_cbc(), NULL, NULL, NULL, do_encrypt);
	OPENSSL_assert(EVP_CIPHER_CTX_key_length(&ctx) == 16);
	OPENSSL_assert(EVP_CIPHER_CTX_iv_length(&ctx) == 16);
	EVP_CipherInit_ex(&ctx, NULL, NULL, key, iv, do_encrypt);
	for(;;){
	intlen = fread(inbuf, 1, 1024, in);
	if(intlen <=0) break;
	if(!EVP_CipherUpdate(&ctx, outbuf, &outlen, inbuf, intlen))
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		return 0;
	}
	fwrite(outbuf, 1, outlen, out);
	}
	if(!EVP_CipherFinal_ex(&ctx, outbuf, &outlen)){
		EVP_CIPHER_CTX_cleanup(&ctx);
		return 0;
	}
	fwrite(outbuf, 1, outlen, out);
	EVP_CIPHER_CTX_cleanup(&ctx);
	return 1;
}

int main(int argc, char* argv[])
{
	int mode = atoi(argv[1]), counter = 0;
	char password[100];
	char check[100];	
	char music[100];
	char klucz[100];	
	char id[100];
	FILE *f;
	FILE *r;
	FILE *p;
	FILE *pas;
	FILE *ivs;

	p = fopen("/home/michal/pass.txt", "rw+");
	while(!feof(p)){
		fscanf(p, "%s", check);
		//counter = counter + 1;
	}
	printf("To execute program type password: ");
	struct termios term, term_orig;
	tcgetattr(STDIN_FILENO, &term);
	term_orig = term;
	term.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	scanf("%s", password);
	tcsetattr(STDIN_FILENO, TCSANOW, &term_orig);	

	if(strcmp(password, check) == 0){
	 printf("Ok\n");
	if(argc < 5) {perror("Za mala liczba argumentow"); return 0;}
	strcpy(music, argv[2]);
	strcpy(klucz, argv[3]);
	strcpy(id, argv[4]);
	f = fopen(music,"rw+");
	r = fopen(music,"rw+");
	pas = fopen(klucz, "rw+");
	ivs = fopen(id, "rw+");
	if(mode == 1)
	do_crypt(f, r, mode, pas, ivs);
	else if(mode == 0) do_crypt(r, f, mode, pas, ivs);
	else{
		printf("1 - kodowanie pliku, 0 - dekodowanie pliku\n");
		return 0;
	}
	fclose(f);
	fclose(r);
	fclose(pas);
	fclose(ivs);
	fclose(p);
	}
	else{
		printf("Zle wprowadzone haslo...\n");
		return 0;
	}

	return 0;
}
