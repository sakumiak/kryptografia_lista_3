//Program uruchamiamy z dwoma parametrami
// Pierwszy to nazwa pliku muzycznego
// Drugi, jak długo chcemy go posłuchać
#include<stdio.h>
#include<stdlib.h>
#include<vlc/vlc.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
int main(int argc, char* argv[]){
	FILE *f;
	FILE *p;
	FILE *r;
	f = fopen("/home/michal/pass2.txt", "rw+");
	p = fopen("krypt.txt", "rw+");
	r = fopen("conf.txt", "rw+");	
	char done[100];	
	char check[100];
	char pass[100];
	char decode[100] = "./test 0";
	char code[100] = "./test 1";
	int t;
	if(argc < 3){
	perror("Zła liczba argumentów");
	return 0;
	}
	while(!feof(f)){
		fscanf(f, "%s", check);
	}
	fscanf(p, "%s", done);
	if(strcmp("1", done) != 0){
	printf("Konfirgurowanie\n");
	fprintf(r, "/home/michal/key.txt\n");
	fprintf(r, "/home/michal/key2.txt\n");
	fprintf(r, "/home/michal/iv.txt\n");
	fprintf(r, "/home/michal/pass.txt\n");
	fprintf(r, "/home/michal/pass2.txt\n");
	fclose(r);
	printf("Zabezpiecz sciezki\n");
	system("./test 1 conf.txt /home/michal/key2.txt /home/michal/iv.txt");
	fprintf(p, "1");
	printf("Uruchom ponownie program, aby dokończyc proces...\n");
	fclose(p);
	return 0;
	}
	strcat(decode, " ");
	strcat(decode, argv[1]);
	strcat(decode, " /home/michal/key.txt");
	strcat(decode, " /home/michal/iv.txt");
	strcat(code, " ");
	strcat(code, argv[1]);
	strcat(code, " /home/michal/key.txt");
	strcat(code, " /home/michal/iv.txt");
	printf("To execute program type password: ");
	//Podawanie hasła
	struct termios term, term_orig;
	tcgetattr(STDIN_FILENO, &term);
	term_orig = term;
	term.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	scanf("%s", pass);
	tcsetattr(STDIN_FILENO, TCSANOW, &term_orig);
	//Rozkodowywanie i odtworzenie pliku
	if(strcmp(pass, check) == 0){
	printf("OK\n");	
	printf("Type password to decode mp3\n");
	system(decode);
	libvlc_instance_t *inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;

	inst = libvlc_new(0, NULL);
	m = libvlc_media_new_path(inst, argv[1]);
	printf("%d", t);
	mp = libvlc_media_player_new_from_media(m);
	libvlc_media_player_play(mp);
	sleep(atoi(argv[2]));
	
	libvlc_media_player_stop(mp);
	libvlc_media_release(m);
	libvlc_media_player_release(mp);
	libvlc_release(inst);
	//Ponownie szyfrujemy plik.
	printf("Type password to encode mp3\n");
	system(code);
	}
	else{
		printf("bledne haslo\n"); return 0;
	}
	fclose(f);
	return 0;
}
