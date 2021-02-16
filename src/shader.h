typedef struct {
	unsigned int ID;
} shader;

shader shd(char* vertPath, char* fragPath);
void shdUse(shader* s);
char* readFile(char* filename);
void shdSetInt(shader s, const char* name, int value);
void shdSetFloat(shader s, const char* name, float value);
void shdSetIVec2(shader s, const char* name, int x, int y);
