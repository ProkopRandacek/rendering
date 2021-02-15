typedef struct {
	unsigned int ID;
} shader;

shader shd(char* vertPath, char* fragPath);
void shdUse(shader* s);
char* readFile(char* filename);
