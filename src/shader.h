// vim: filetype=c
typedef struct {
	unsigned int ID;
} shader;

shader shd(void);
void shdUse(shader* s);
void shdSetInt(shader s, const char* name, int value);
void shdSetFloat(shader s, const char* name, float value);
void shdSetIVec2(shader s, const char* name, int x, int y);
void shdSetVec3Array (shader s, const char* name, int count, float* values);
void shdSetVec4Array (shader s, const char* name, int count, float* values);
void shdSetFloatArray(shader s, const char* name, int count, float* values);
