#include"stdafx.h"
#include"texture.h"


IUINT32** texture_create_default()
{
	IUINT32** texture = new IUINT32*[TEXTURE_SIZE_MAX];
	for (int j = 0; j < TEXTURE_SIZE_MAX; j++)
	{
		texture[j] = new IUINT32[TEXTURE_SIZE_MAX];
	}
	for (int j = 0; j < TEXTURE_SIZE_MAX; j++)
	{
		for (int i = 0; i < TEXTURE_SIZE_MAX; i++)
		{
			int x = i / 32, y = j / 32;
			texture[j][i] = ((x + y) & 1) ? 0xffffff : 0x3fbcef;
		}
	}

	return texture;
}
void texture_destroy(IUINT32** texture)
{
	for (int j = 0; j < TEXTURE_SIZE_MAX; j++)
	{
		delete[] texture[j];
	}
	delete[] texture;
}

/*
Texture* texture_create(const char* filename)
{
	Texture* texture = (Texture*)malloc(sizeof(Texture));
	memset(texture, 0, sizeof(Texture));

	char *result = malloc(strlen(RESOURCES_PATH) + strlen(filename) + 1);
	strcpy(result, RESOURCES_PATH);
	strcat(result, filename);

	char* path = result;
	texture_load(texture, path);
	return texture;
}




void texture_load(Texture* texture, const char* fillename)
{

	FILE *file;
	file = fopen(fillename, "rb");

	fseek(file, sizeof(BITMAPFILEHEADER), SEEK_CUR);

	BITMAPINFOHEADER info_head;
	fread(&info_head, sizeof(BITMAPINFOHEADER), 1, file); 

	texture->width = info_head.biWidth;
	texture->height = info_head.biHeight;

	int biBitCount = info_head.biBitCount;

	int n_linebyte = (texture->width*biBitCount / 8 + 3) / 4 * 4;
	unsigned char* buffer =  (unsigned char*)calloc(1, n_linebyte * texture->height);
	fread(buffer, sizeof(char), n_linebyte * texture->height, file);
	fclose(file);
	
	memset(texture->texbuf, 0, TEXTURE_SIZE_MAX * TEXTURE_SIZE_MAX * 4);
	int count = 0;
	for (int i = texture->height -1 ; i >= 0 ; i--)
	{
		for (int j = 0; j < texture->width; j++)
		{
			int n_row, n_column, n_rgb = 3;
			n_row = i * n_linebyte;
			n_column = j * n_rgb;

			int r, g, b;
			r = *(buffer + n_row + n_column + 2);
			g = *(buffer + n_row + n_column + 1);
			b = *(buffer + n_row + n_column);
			texture->texbuf[i][j] = (r << 16) | (g << 8) | (b);
			//printf("corlor= %d\n", texture->texbuf[count][j]);
			//printf("R=%d G=%d B=%d\n", r, g, b);
		}
		count++;
	}

	//return texture;
}
*/

