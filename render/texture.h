#pragma once
#define TEXTURE_SIZE_MAX 256
IUINT32** texture_create_default();
void texture_destroy(IUINT32** texture);
//Texture* texture_create(const char* filename);
//void texture_load(Texture* texture, const char* fillename);
