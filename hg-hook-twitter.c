

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>


void strip_newline (char *name) {
	size_t ln = strlen(name) - 1;
	if (name[ln] == '\n')
	name[ln] = '\0';
}

int main (int argc, const char **argv, const char **env) {
	char buf[4096];
	char author[128], commit_msg[1024], node[512], paths_default[4096], repo_path[4096];
	char repo_name[256], *tok;
	FILE *pPipe;

	puts(argv[1]);
	chdir(argv[1]);

	if( (pPipe = _popen( "hg log -r tip --template \"{author}\"", "rt" )) == NULL ) exit(1);
	fgets(author, 128, pPipe);
	//printf(author);
	_pclose(pPipe);
	//puts("");

	if( (pPipe = _popen( "hg log -r tip --template \"{desc|firstline}\"", "rt" )) == NULL ) exit(1);
	fgets(commit_msg, 1024, pPipe);
	//printf(commit_msg);
	_pclose(pPipe);
	//puts("");
	
	if( (pPipe = _popen( "hg log -r tip --template \"{node}\"", "rt" )) == NULL ) exit(1);
	fgets(node, 512, pPipe);
	//printf(node);
	_pclose(pPipe);
	//puts("");
	
	
	if( (pPipe = _popen( "hg paths default", "rt" )) == NULL ) exit(1);
	fgets(paths_default, 1024, pPipe);
	strip_newline(paths_default);
	//printf(paths_default);
	_pclose(pPipe);
	//puts("");
	
	
	if( (pPipe = _popen( "hg root", "rt" )) == NULL ) exit(1);
	fgets(repo_path, 1024, pPipe);
	strip_newline(repo_path);
	//printf(repo_path);
	_pclose(pPipe);
	//puts("");
	
	tok = repo_path;

	while ((tok = strtok(tok, "\\")) != NULL) {
		//printf("<<%s>>\n", tok);
		strcpy(repo_name, tok);
		tok = NULL;
	}

	//puts(repo_name);
	
	sprintf(buf, 
		"java -jar \"D:\\Ultimate-Twitter-Bot.jar\" \"" 
		"%s" \
		" pushed a commit to " \
		"%s" \
		" - " \
		"%s" \
		" - " \
		"%s" \
		"/commits/" \
		"%s\"",
		"SiraitX",
		repo_name, commit_msg, paths_default, node
	);
	
	//puts(buf);
	//getchar();
	system(buf);
	
	
	return 0;
}


