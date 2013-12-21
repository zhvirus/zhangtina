#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

//////////////////////////////////////////////////////////////////////


#include <GL\glaux.h>
#include "..\\inc\\AccessObj.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAccessObj::CAccessObj()
{
	m_pModel = NULL;
}

CAccessObj::~CAccessObj()
{
	Destory();
}

//////////////////////////////////////////////////////////////////////
// FindGroup: Find a material in the model
//////////////////////////////////////////////////////////////////////
unsigned int CAccessObj::FindMaterial(char* name)
{
	unsigned int i;
	bool bFound = false;
	
	// XXX doing a linear search on a string key'd list is pretty lame, but it works and is fast enough for now.
	for (i = 0; i < m_pModel->nMaterials; i++)
	{
		if (!strcmp(m_pModel->pMaterials[i].name, name))
		{
			bFound = true;
			break;
		}
	}
	
	// didn't find the name, so print a warning and return the default material (0).
	if (!bFound)
	{
		printf("FindMaterial():  can't find material \"%s\".\n", name);
		i = 0;
	}
	
	return i;
}


//////////////////////////////////////////////////////////////////////
// DirName: return the directory given a path
//
// path - filesystem path
//
// NOTE: the return value should be free'd.
//////////////////////////////////////////////////////////////////////
char * CAccessObj::DirName(char* path)
{
	static char dir[256];
	char *s;
	
	sprintf(dir, "%s", path);

	s = strrchr(dir, '\\');
	if (s)	s[1] = '\0';
	else	dir[0] = '\0';

	return dir;
}


//////////////////////////////////////////////////////////////////////
// ReadMTL: read a wavefront material library file
//
// model - properly initialized COBJmodel structure
// name  - name of the material library
//////////////////////////////////////////////////////////////////////
void CAccessObj::ReadMTL(char* name)
{
	FILE* file;
	char* dir;
	char* filename;
	char  buf[256];
	char  buf2[256];
	unsigned int nMaterials, i;
	
	dir = DirName(m_pModel->pathname);
	filename = new char [(strlen(dir) + strlen(name) + 1)];
	strcpy(filename, dir);
	strcat(filename, name);
	
	file = fopen(filename, "r");
	if (!file)
	{
		fprintf(stderr, "ReadMTL() failed: can't open material file \"%s\".\n", filename);
		exit(1);
	}
	delete [] filename;

	// count the number of materials in the file
	nMaterials = 1;
	while(fscanf(file, "%s", buf) != EOF)
	{
		switch(buf[0])
		{
		case '#':				/* comment */
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		case 'n':				/* newmtl */
			fgets(buf, sizeof(buf), file);
			nMaterials++;
			sscanf(buf, "%s %s", buf, buf);
			break;
		default:
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		}
	}
	
	rewind(file);
	
	m_pModel->pMaterials = new COBJmaterial [nMaterials];
	m_pModel->nMaterials = nMaterials;
	
	// set the default material
	for (i = 0; i < nMaterials; i++) {
		m_pModel->pMaterials[i].name[0] = '\0';
		m_pModel->pMaterials[i].shininess[0] = 32.0f;
		m_pModel->pMaterials[i].diffuse[0] = 0.8f;
		m_pModel->pMaterials[i].diffuse[1] = 0.8f;
		m_pModel->pMaterials[i].diffuse[2] = 0.8f;
		m_pModel->pMaterials[i].diffuse[3] = 1.f;
		m_pModel->pMaterials[i].ambient[0] = 0.2f;
		m_pModel->pMaterials[i].ambient[1] = 0.2f;
		m_pModel->pMaterials[i].ambient[2] = 0.2f;
		m_pModel->pMaterials[i].ambient[3] = 1.0f;
		m_pModel->pMaterials[i].specular[0] = 0.0f;
		m_pModel->pMaterials[i].specular[1] = 0.0f;
		m_pModel->pMaterials[i].specular[2] = 0.0f;
		m_pModel->pMaterials[i].specular[3] = 1.f;
		m_pModel->pMaterials[i].emissive[0] = 0.0f;
		m_pModel->pMaterials[i].emissive[1] = 0.0f;
		m_pModel->pMaterials[i].emissive[2] = 0.0f;
		m_pModel->pMaterials[i].emissive[3] = 1.0f;
	}
	sprintf(m_pModel->pMaterials[0].name, "default");
	
	// now, read in the data
	nMaterials = 0;
	while(fscanf(file, "%s", buf) != EOF)
	{
		switch(buf[0])
		{
		case '#':				/* comment */
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		case 'n':				/* newmtl */
			fgets(buf, sizeof(buf), file);

			i = 0;
			while (buf[i] != ' ') i ++;
			while (buf[i] == ' ') i ++;
			strcpy(buf2, &buf[i]);
			i = static_cast<unsigned int>(strlen(buf2) - 1);
			while (buf2[i] == 0x0a || buf2[i] == 0x0d || buf2[i] == ' ')
			{
				buf2[i] = '\0';
				i --;
			}

			//sscanf(buf, "%s %s", buf, buf);
			nMaterials++;
			sprintf(m_pModel->pMaterials[nMaterials].name, "%s", buf2);
			break;
		case 'N':
			fscanf(file, "%f", &m_pModel->pMaterials[nMaterials].shininess);
			/* wavefront shininess is from [0, 1000], so scale for OpenGL */
			m_pModel->pMaterials[nMaterials].shininess[0] /= 1000.0f;
			m_pModel->pMaterials[nMaterials].shininess[0] *= 128.0f;
			break;
		case 'K':
			switch(buf[1])
			{
			case 'd':
				fscanf(file, "%f %f %f",
					&m_pModel->pMaterials[nMaterials].diffuse[0],
					&m_pModel->pMaterials[nMaterials].diffuse[1],
					&m_pModel->pMaterials[nMaterials].diffuse[2]);
				break;
			case 's':
				fscanf(file, "%f %f %f",
					&m_pModel->pMaterials[nMaterials].specular[0],
					&m_pModel->pMaterials[nMaterials].specular[1],
					&m_pModel->pMaterials[nMaterials].specular[2]);
				break;
			case 'a':
				fscanf(file, "%f %f %f",
					&m_pModel->pMaterials[nMaterials].ambient[0],
					&m_pModel->pMaterials[nMaterials].ambient[1],
					&m_pModel->pMaterials[nMaterials].ambient[2]);
				break;
			default:
				/* eat up rest of line */
				fgets(buf, sizeof(buf), file);
				break;
			}
			break;
		case 'm':

			fgets(buf, sizeof(buf), file);
			i = 0;
			while (buf[i] != ' ') i ++;
			while (buf[i] == ' ') i ++;
			strcpy(buf2, &buf[i]);
			i = static_cast<unsigned int>(strlen(buf2) - 1);
			while (buf2[i] == 0x0a || buf2[i] == 0x0d || buf2[i] == ' ')
			{
				buf2[i] = '\0';
				i --;
			}

			//fscanf(file, "%s", filename);
			if (buf2[1] != ':')
			{
				sprintf(m_pModel->pMaterials[nMaterials].sTexture,
					"%s%s", dir, buf2);
			}
			else strcpy(m_pModel->pMaterials[nMaterials].sTexture, buf2);

			break;
		default:
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		}
	}

	fclose (file);
}

//////////////////////////////////////////////////////////////////////
// WriteMTL: write a wavefront material library file
//
// model      - properly initialized COBJmodel structure
// modelpath  - pathname of the model being written
// mtllibname - name of the material library to be written
//////////////////////////////////////////////////////////////////////
void CAccessObj::WriteMTL(char* modelpath, char* mtllibname)
{
	FILE* file;
	char* dir;
	char* filename;
	COBJmaterial* material;
	unsigned int i;
	
	dir = DirName(modelpath);
	filename = new char [(strlen(dir)+strlen(mtllibname))];
	strcpy(filename, dir);
	strcat(filename, mtllibname);
	free(dir);
	
	/* open the file */
	file = fopen(filename, "w");
	if (!file)
	{
		fprintf(stderr, "WriteMTL() failed: can't open file \"%s\".\n", filename);
		exit(1);
	}
	delete [] filename;
	
	/* spit out a header */
	fprintf(file, "#  \n");
	fprintf(file, "#  Wavefront MTL generated by OBJ library\n");
	fprintf(file, "#  \n");
	fprintf(file, "#  OBJ library\n");
	fprintf(file, "#  Nate Robins\n");
	fprintf(file, "#  ndr@pobox.com\n");
	fprintf(file, "#  http://www.pobox.com/~ndr\n");
	fprintf(file, "#  \n\n");
	
	for (i = 0; i < m_pModel->nMaterials; i++)
	{
		material = &m_pModel->pMaterials[i];
		fprintf(file, "newmtl %s\n", material->name);
		fprintf(file, "Ka %f %f %f\n", 
			material->ambient[0], material->ambient[1], material->ambient[2]);
		fprintf(file, "Kd %f %f %f\n", 
			material->diffuse[0], material->diffuse[1], material->diffuse[2]);
		fprintf(file, "Ks %f %f %f\n", 
			material->specular[0],material->specular[1],material->specular[2]);
		fprintf(file, "Ns %f\n", material->shininess[0] / 128.0 * 1000.0);
		fprintf(file, "\n");
	}
}

//////////////////////////////////////////////////////////////////////
// FirstPass: first pass at a Wavefront OBJ file that gets all the
// statistics of the model (such as #vertices, #normals, etc)
//
// model - properly initialized COBJmodel structure
// file  - (fopen'd) file descriptor 
//////////////////////////////////////////////////////////////////////
void CAccessObj::FirstPass(FILE* file) 
{
	unsigned int    nVertices;		/* number of vertices in m_pModel */
	unsigned int    nNormals;		/* number of normals in m_pModel */
	unsigned int    nTexCoords;		/* number of texcoords in m_pModel */
	unsigned int    nTriangles;		/* number of triangles in m_pModel */
	COBJgroup* group;			/* current group */
	unsigned  v, n, t;
	char      buf[128];
	
	/* make a default group */
	group = AddGroup("default");
	
	nVertices = nNormals = nTexCoords = nTriangles = 0;
	while(fscanf(file, "%s", buf) != EOF)
	{
		switch(buf[0])
		{
		case '#':				/* comment */
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		case 'v':				/* v, vn, vt */
			switch(buf[1])
			{
			case '\0':			/* vertex */
				/* eat up rest of line */
				fgets(buf, sizeof(buf), file);
				nVertices++;
				break;
			case 'n':				/* normal */
				/* eat up rest of line */
				fgets(buf, sizeof(buf), file);
				nNormals++;
				break;
			case 't':				/* texcoord */
				/* eat up rest of line */
				fgets(buf, sizeof(buf), file);
				nTexCoords++;
				break;
			default:
				printf("FirstPass(): Unknown token \"%s\".\n", buf);
				exit(1);
				break;
			}
			break;

		case 'm':
			fgets(buf, sizeof(buf), file);
			sscanf(buf, "%s %s", buf, buf);
			sprintf(m_pModel->mtllibname, "%s", buf);
			ReadMTL(buf);
			break;
		case 'u':
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		case 'g':				/* group */
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			buf[strlen(buf)-1] = '\0';	/* nuke '\n' */
			group = AddGroup(buf);
			break;
		case 'f':				/* face */
			v = n = t = 0;
			fscanf(file, "%s", buf);
			/* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
			if (strstr(buf, "//"))
			{
				/* v//n */
				sscanf(buf, "%d//%d", &v, &n);
				fscanf(file, "%d//%d", &v, &n);
				fscanf(file, "%d//%d", &v, &n);
				nTriangles++;
				group->nTriangles++;
				while(fscanf(file, "%d//%d", &v, &n) > 0)
				{
					nTriangles++;
					group->nTriangles++;
				}
			}
			else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3)
			{
				/* v/t/n */
				fscanf(file, "%d/%d/%d", &v, &t, &n);
				fscanf(file, "%d/%d/%d", &v, &t, &n);
				nTriangles++;
				group->nTriangles++;
				while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0)
				{
					nTriangles++;
					group->nTriangles++;
				}
			}
			else if (sscanf(buf, "%d/%d", &v, &t) == 2)
			{
				/* v/t */
				fscanf(file, "%d/%d", &v, &t);
				fscanf(file, "%d/%d", &v, &t);
				nTriangles++;
				group->nTriangles++;
				while(fscanf(file, "%d/%d", &v, &t) > 0)
				{
					nTriangles++;
					group->nTriangles++;
				}
			}
			else
			{
				/* v */
				fscanf(file, "%d", &v);
				fscanf(file, "%d", &v);
				nTriangles++;
				group->nTriangles++;
				while(fscanf(file, "%d", &v) > 0)
				{
					nTriangles++;
					group->nTriangles++;
				}
			}
			break;
			
		default:
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		}
	}
	
	/* set the stats in the m_pModel structure */
	m_pModel->nVertices  = nVertices;
	m_pModel->nNormals   = nNormals;
	m_pModel->nTexCoords = nTexCoords;
	m_pModel->nTriangles = nTriangles;
	
	/* allocate memory for the triangles in each group */
	group = m_pModel->pGroups;
	while(group)
	{
		if (group->nTriangles > 0)
			group->pTriangles = new unsigned int [group->nTriangles];
		group->nTriangles = 0;
		group = group->next;
	}
}

//////////////////////////////////////////////////////////////////////
// SecondPass: second pass at a Wavefront OBJ file that gets all
// the data.
//
// model - properly initialized COBJmodel structure
// file  - (fopen'd) file descriptor 
//////////////////////////////////////////////////////////////////////
void CAccessObj::SecondPass(FILE* file) 
{
	unsigned int	nVertices;		/* number of vertices in m_pModel */
	unsigned int	nNormals;		/* number of normals in m_pModel */
	unsigned int	nTexCoords;		/* number of texcoords in m_pModel */
	unsigned int	nTriangles;		/* number of triangles in m_pModel */
	CVector3f *	vertices;		/* array of vertices  */
	CVector3f *	normals;		/* array of normals */
	CVector3f *	texcoords;		/* array of texture coordinates */
	COBJgroup *	group;			/* current group pointer */
	unsigned int	material;		/* current material */
	unsigned int	i, v, n, t;
	char		buf[128];

	/* set the pointer shortcuts */
	vertices     = m_pModel->vpVertices;
	normals      = m_pModel->vpNormals;
	texcoords    = m_pModel->vpTexCoords;
	group        = m_pModel->pGroups;
	
	/* on the second pass through the file, read all the data into the
	allocated arrays */
	nVertices = nNormals = nTexCoords = 1;
	nTriangles = 0;
	material = 0;

	int nNormalAdd = 0;
	int nNormalCount = 0;

	while(fscanf(file, "%s", buf) != EOF)
	{
		switch(buf[0])
		{
		case '#':				/* comment */
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		case 'v':				/* v, vn, vt */
			switch(buf[1])
			{
			case '\0':			/* vertex */
				fscanf(file, "%f %f %f", 
					&vertices[nVertices].x, 
					&vertices[nVertices].y, 
					&vertices[nVertices].z);
				nVertices++;
				break;
			case 'n':				/* normal */
				fscanf(file, "%f %f %f", 
					&normals[nNormals].x,
					&normals[nNormals].y, 
					&normals[nNormals].z);
				nNormals++;
				nNormalCount ++;
				break;
			case 't':				/* texcoord */
				fscanf(file, "%f %f", 
					&texcoords[nTexCoords].x,
					&texcoords[nTexCoords].y);
				nTexCoords++;
				break;
			}
			break;
		case 'u':
			fgets(buf, sizeof(buf), file);
			//sscanf(buf, "%s %s", buf, buf);

			i = 0;
			while (buf[i] != ' ') i ++;
			while (buf[i] == ' ') i ++;
			char buf2[256];
			strcpy(buf2, &buf[i]);
			i = static_cast<unsigned int>(strlen(buf2) - 1);
			while (buf2[i] == 0x0a || buf2[i] == 0x0d || buf2[i] == ' ')
			{
				buf2[i] = '\0';
				i --;
			}

			group->material = material = FindMaterial(buf2);

			break;
		case 'g':				/* group */
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			buf[strlen(buf)-1] = '\0';	/* nuke '\n' */
			group = FindGroup(buf);
			group->material = material;
			nNormalAdd += nNormalCount;
			nNormalCount = 0;
			break;
		case 'f':				/* face */
			v = n = t = 0;
			fscanf(file, "%s", buf);
			/* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
			Tri(nTriangles).mindex = material;
			if (strstr(buf, "//"))
			{
				/* v//n */
				sscanf(buf, "%d//%d", &v, &n);
				Tri(nTriangles).vindices[0] = v;
				Tri(nTriangles).nindices[0] = n;// + nNormalAdd;
				fscanf(file, "%d//%d", &v, &n);
				Tri(nTriangles).vindices[1] = v;
				Tri(nTriangles).nindices[1] = n;// + nNormalAdd;
				fscanf(file, "%d//%d", &v, &n);
				Tri(nTriangles).vindices[2] = v;
				Tri(nTriangles).nindices[2] = n;// + nNormalAdd;
				group->pTriangles[group->nTriangles++] = nTriangles;
				nTriangles++;
				while(fscanf(file, "%d//%d", &v, &n) > 0)
				{
					Tri(nTriangles).vindices[0] = Tri(nTriangles-1).vindices[0];
					Tri(nTriangles).nindices[0] = Tri(nTriangles-1).nindices[0];
					Tri(nTriangles).vindices[1] = Tri(nTriangles-1).vindices[2];
					Tri(nTriangles).nindices[1] = Tri(nTriangles-1).nindices[2];
					Tri(nTriangles).vindices[2] = v;
					Tri(nTriangles).nindices[2] = n;// + nNormalAdd;
					Tri(nTriangles).mindex = material;
					group->pTriangles[group->nTriangles++] = nTriangles;
					nTriangles++;
				}
			}
			else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3)
			{
				/* v/t/n */
				Tri(nTriangles).vindices[0] = v;
				Tri(nTriangles).tindices[0] = t;
				Tri(nTriangles).nindices[0] = n;// + nNormalAdd;
				fscanf(file, "%d/%d/%d", &v, &t, &n);
				Tri(nTriangles).vindices[1] = v;
				Tri(nTriangles).tindices[1] = t;
				Tri(nTriangles).nindices[1] = n;// + nNormalAdd;
				fscanf(file, "%d/%d/%d", &v, &t, &n);
				Tri(nTriangles).vindices[2] = v;
				Tri(nTriangles).tindices[2] = t;
				Tri(nTriangles).nindices[2] = n;// + nNormalAdd;
				group->pTriangles[group->nTriangles++] = nTriangles;
				nTriangles++;
				while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0)
				{
					Tri(nTriangles).vindices[0] = Tri(nTriangles-1).vindices[0];
					Tri(nTriangles).tindices[0] = Tri(nTriangles-1).tindices[0];
					Tri(nTriangles).nindices[0] = Tri(nTriangles-1).nindices[0];
					Tri(nTriangles).vindices[1] = Tri(nTriangles-1).vindices[2];
					Tri(nTriangles).tindices[1] = Tri(nTriangles-1).tindices[2];
					Tri(nTriangles).nindices[1] = Tri(nTriangles-1).nindices[2];
					Tri(nTriangles).vindices[2] = v;
					Tri(nTriangles).tindices[2] = t;
					Tri(nTriangles).nindices[2] = n;// + nNormalAdd;
					Tri(nTriangles).mindex = material;
					group->pTriangles[group->nTriangles++] = nTriangles;
					nTriangles++;
				}
			}
			else if (sscanf(buf, "%d/%d", &v, &t) == 2)
			{
				/* v/t */
				Tri(nTriangles).vindices[0] = v;
				Tri(nTriangles).tindices[0] = t;
				fscanf(file, "%d/%d", &v, &t);
				Tri(nTriangles).vindices[1] = v;
				Tri(nTriangles).tindices[1] = t;
				fscanf(file, "%d/%d", &v, &t);
				Tri(nTriangles).vindices[2] = v;
				Tri(nTriangles).tindices[2] = t;
				group->pTriangles[group->nTriangles++] = nTriangles;
				nTriangles++;
				while(fscanf(file, "%d/%d", &v, &t) > 0)
				{
					Tri(nTriangles).vindices[0] = Tri(nTriangles-1).vindices[0];
					Tri(nTriangles).tindices[0] = Tri(nTriangles-1).tindices[0];
					Tri(nTriangles).vindices[1] = Tri(nTriangles-1).vindices[2];
					Tri(nTriangles).tindices[1] = Tri(nTriangles-1).tindices[2];
					Tri(nTriangles).vindices[2] = v;
					Tri(nTriangles).tindices[2] = t;
					Tri(nTriangles).mindex = material;
					group->pTriangles[group->nTriangles++] = nTriangles;
					nTriangles++;
				}
			}
			else
			{
				/* v */
				sscanf(buf, "%d", &v);
				Tri(nTriangles).vindices[0] = v;
				fscanf(file, "%d", &v);
				Tri(nTriangles).vindices[1] = v;
				fscanf(file, "%d", &v);
				Tri(nTriangles).vindices[2] = v;
				group->pTriangles[group->nTriangles++] = nTriangles;
				nTriangles++;
				while(fscanf(file, "%d", &v) > 0)
				{
					Tri(nTriangles).vindices[0] = Tri(nTriangles-1).vindices[0];
					Tri(nTriangles).vindices[1] = Tri(nTriangles-1).vindices[2];
					Tri(nTriangles).vindices[2] = v;
					group->pTriangles[group->nTriangles++] = nTriangles;
					Tri(nTriangles).mindex = material;
					nTriangles++;
				}
			}
			break;
			
		default:
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Dimensions: Calculates the dimensions (width, height, depth) of
// a model.
//
// model      - initialized COBJmodel structure
// dimensions - array of 3 GLfloats (float dimensions[3])
//////////////////////////////////////////////////////////////////////
void CAccessObj::Dimensions(float* dimensions)
{
	unsigned int i;
	CVector3f vMax, vMin;
	
	assert(m_pModel);
	assert(m_pModel->vpVertices);
	assert(dimensions);
	
	/* get the max/mins */
	vMax = vMin = m_pModel->vpVertices[0];
	for (i = 1; i <= m_pModel->nVertices; i++)
	{
		if (vMax.x < m_pModel->vpVertices[i].x)
			vMax.x = m_pModel->vpVertices[i].x;
		if (vMin.x > m_pModel->vpVertices[i].x)
			vMin.x = m_pModel->vpVertices[i].x;
		
		if (vMax.y < m_pModel->vpVertices[i].y)
			vMax.y = m_pModel->vpVertices[i].y;
		if (vMin.y > m_pModel->vpVertices[i].y)
			vMin.y = m_pModel->vpVertices[i].y;
		
		if (vMax.z < m_pModel->vpVertices[i].z)
			vMax.z = m_pModel->vpVertices[i].z;
		if (vMin.z > m_pModel->vpVertices[i].z)
			vMin.z = m_pModel->vpVertices[i].z;
	}
	
	/* calculate m_pModel width, height, and depth */
	dimensions[0] = vMax.x-vMin.x;
	dimensions[1] = vMax.y-vMin.y;
	dimensions[2] = vMax.z-vMin.z;
}

//////////////////////////////////////////////////////////////////////
// Scale: Scales a model by a given amount.
// 
// model - properly initialized COBJmodel structure
// scale - scalefactor (0.5 = half as large, 2.0 = twice as large)
//////////////////////////////////////////////////////////////////////
void CAccessObj::Scale(float scale)
{
	unsigned int i;
	
	for (i = 1; i <= m_pModel->nVertices; i++)
		m_pModel->vpVertices[i] = m_pModel->vpVertices[i] * scale;
}

//////////////////////////////////////////////////////////////////////
// ReverseWinding: Reverse the polygon winding for all polygons in
// this model.  Default winding is counter-clockwise.  Also changes
// the direction of the normals.
// 
// model - properly initialized COBJmodel structure 
//////////////////////////////////////////////////////////////////////
void CAccessObj::ReverseWinding()
{
	unsigned int i, swap;
	
	assert(m_pModel);
	
	for (i = 0; i < m_pModel->nTriangles; i++)
	{
		swap = Tri(i).vindices[0];
		Tri(i).vindices[0] = Tri(i).vindices[2];
		Tri(i).vindices[2] = swap;
		
		if (m_pModel->nNormals)
		{
			swap = Tri(i).nindices[0];
			Tri(i).nindices[0] = Tri(i).nindices[2];
			Tri(i).nindices[2] = swap;
		}
		
		if (m_pModel->nTexCoords)
		{
			swap = Tri(i).tindices[0];
			Tri(i).tindices[0] = Tri(i).tindices[2];
			Tri(i).tindices[2] = swap;
		}
	}
	
	/* reverse facet normals */
	for (i = 1; i <= m_pModel->nFacetnorms; i++)
		m_pModel->vpFacetNorms[i] = m_pModel->vpFacetNorms[i]*(-1);
	
	/* reverse vertex normals */
	for (i = 1; i <= m_pModel->nNormals; i++)
		m_pModel->vpNormals[i] = m_pModel->vpNormals[i]*(-1);
}

//////////////////////////////////////////////////////////////////////
// FacetNormals: Generates facet normals for a model (by taking the
// cross product of the two vectors derived from the sides of each
// triangle).  Assumes a counter-clockwise winding.
//
// model - initialized COBJmodel structure
//////////////////////////////////////////////////////////////////////
void CAccessObj::FacetNormals()
{
	unsigned int  i;
	CVector3f u, v;
	
	assert(m_pModel);
	assert(m_pModel->vpVertices);
	
	/* clobber any old facetnormals */
	if (m_pModel->vpFacetNorms)
		free(m_pModel->vpFacetNorms);
	
	/* allocate memory for the new facet normals */
	m_pModel->nFacetnorms = m_pModel->nTriangles;
	m_pModel->vpFacetNorms = new CVector3f [m_pModel->nFacetnorms + 1];
	
	for (i = 0; i < m_pModel->nTriangles; i++)
	{
		m_pModel->pTriangles[i].findex = i+1;
		
		u = m_pModel->vpVertices[Tri(i).vindices[1]] - m_pModel->vpVertices[Tri(i).vindices[0]];
		v = m_pModel->vpVertices[Tri(i).vindices[2]] - m_pModel->vpVertices[Tri(i).vindices[0]];

		m_pModel->vpFacetNorms[i+1] = u.Cross(v);
		m_pModel->vpFacetNorms[i+1].Normalize();
	}
}

//////////////////////////////////////////////////////////////////////
// VertexNormals: Generates smooth vertex normals for a model.
// First builds a list of all the triangles each vertex is in.  Then
// loops through each vertex in the the list averaging all the facet
// normals of the triangles each vertex is in.  Finally, sets the
// normal index in the triangle for the vertex to the generated smooth
// normal.  If the dot product of a facet normal and the facet normal
// associated with the first triangle in the list of triangles the
// current vertex is in is greater than the cosine of the angle
// parameter to the function, that facet normal is not added into the
// average normal calculation and the corresponding vertex is given
// the facet normal.  This tends to preserve hard edges.  The angle to
// use depends on the model, but 90 degrees is usually a good start.
//
// model - initialized COBJmodel structure
// angle - maximum angle (in degrees) to smooth across
//////////////////////////////////////////////////////////////////////
void CAccessObj::VertexNormals(float angle)
{
	OBJnode*  node;
	OBJnode*  tail;
	OBJnode** members;
	CVector3f *  normals;
	unsigned int    nNormals;
	CVector3f   average;
	float   dot, cos_angle;
	unsigned int    i, avg;
	
	assert(m_pModel);
	assert(m_pModel->vpFacetNorms);
	
	/* calculate the cosine of the angle (in degrees) */
	cos_angle = (float)cos(angle * 3.14159265 / 180.0);
	
	/* nuke any previous normals */
	if (m_pModel->vpNormals)
		free(m_pModel->vpNormals);
	
	/* allocate space for new normals */
	m_pModel->nNormals = m_pModel->nTriangles * 3; /* 3 normals per triangle */
	m_pModel->vpNormals = new CVector3f [m_pModel->nNormals+1];
	
	/* allocate a structure that will hold a linked list of triangle
	indices for each vertex */
	members = new OBJnode * [m_pModel->nVertices + 1];
	for (i = 1; i <= m_pModel->nVertices; i++)
		members[i] = NULL;
	
	/* for every triangle, create a node for each vertex in it */
	for (i = 0; i < m_pModel->nTriangles; i++)
	{
		node = new OBJnode;
		node->index = i;
		node->next  = members[Tri(i).vindices[0]];
		members[Tri(i).vindices[0]] = node;
		
		node = new OBJnode;
		node->index = i;
		node->next  = members[Tri(i).vindices[1]];
		members[Tri(i).vindices[1]] = node;
		
		node = new OBJnode;
		node->index = i;
		node->next  = members[Tri(i).vindices[2]];
		members[Tri(i).vindices[2]] = node;
	}
	
	/* calculate the average normal for each vertex */
	nNormals = 1;
	for (i = 1; i <= m_pModel->nVertices; i++)
	{
		// calculate an average normal for this vertex by averaging the
		// facet normal of every triangle this vertex is in
		node = members[i];
		if (!node)
			fprintf(stderr, "VertexNormals(): vertex w/o a triangle\n");
		average = CVector3f(0, 0, 0);
		avg = 0;
		while (node)
		{
			/* only average if the dot product of the angle between the two
			facet normals is greater than the cosine of the threshold
			angle -- or, said another way, the angle between the two
			facet normals is less than (or equal to) the threshold angle */
			dot = m_pModel->vpFacetNorms[Tri(node->index).findex].Dot(m_pModel->vpFacetNorms[Tri(members[i]->index).findex]);
			if (dot > cos_angle)
			{
				node->averaged = GL_TRUE;
				average = average + m_pModel->vpFacetNorms[Tri(node->index).findex];
				avg = 1;			/* we averaged at least one normal! */
			}
			else
			{
				node->averaged = GL_FALSE;
			}
			node = node->next;
		}
		
		if (avg)
		{
			/* normalize the averaged normal */
			average.Normalize();
			
			/* add the normal to the vertex normals list */
			m_pModel->vpNormals[nNormals] = average;
			avg = nNormals;
			nNormals++;
		}
		
		/* set the normal of this vertex in each triangle it is in */
		node = members[i];
		while (node)
		{
			if (node->averaged)
			{
				/* if this node was averaged, use the average normal */
				if (Tri(node->index).vindices[0] == i)
					Tri(node->index).nindices[0] = avg;
				else if (Tri(node->index).vindices[1] == i)
					Tri(node->index).nindices[1] = avg;
				else if (Tri(node->index).vindices[2] == i)
					Tri(node->index).nindices[2] = avg;
			}
			else
			{
				/* if this node wasn't averaged, use the facet normal */
				m_pModel->vpNormals[nNormals] = m_pModel->vpFacetNorms[Tri(node->index).findex];
				if (Tri(node->index).vindices[0] == i)
					Tri(node->index).nindices[0] = nNormals;
				else if (Tri(node->index).vindices[1] == i)
					Tri(node->index).nindices[1] = nNormals;
				else if (Tri(node->index).vindices[2] == i)
					Tri(node->index).nindices[2] = nNormals;
				nNormals++;
			}
			node = node->next;
		}
	}
	
	m_pModel->nNormals = nNormals - 1;
	
	/* free the member information */
	for (i = 1; i <= m_pModel->nVertices; i++)
	{
		node = members[i];
		while (node)
		{
			tail = node;
			node = node->next;
			free(tail);
		}
	}
	free(members);
	
	/* pack the normals array (we previously allocated the maximum
	number of normals that could possibly be created (nTriangles *
	3), so get rid of some of them (usually alot unless none of the
	facet normals were averaged)) */
	normals = m_pModel->vpNormals;
	m_pModel->vpNormals = new CVector3f [m_pModel->nNormals+1];
	for (i = 1; i <= m_pModel->nNormals; i++) {
		m_pModel->vpNormals[i] = normals[i];
	}
	free(normals);
}


//////////////////////////////////////////////////////////////////////
// LinearTexture: Generates texture coordinates according to a
// linear projection of the texture map.  It generates these by
// linearly mapping the vertices onto a square.
//
// model - pointer to initialized COBJmodel structure
//////////////////////////////////////////////////////////////////////
void CAccessObj::LinearTexture()
{
	COBJgroup *group;
	float dimensions[3];
	float x, y, scalefactor;
	unsigned int i;
	
	assert(m_pModel);
	
	if (m_pModel->vpTexCoords)
		free(m_pModel->vpTexCoords);
	m_pModel->nTexCoords = m_pModel->nVertices;
	m_pModel->vpTexCoords = new CVector3f [m_pModel->nTexCoords+1];
	
	Dimensions(dimensions);
	scalefactor = 2.f / objAbs(objMax(objMax(dimensions[0], dimensions[1]), dimensions[2]));
	
	/* do the calculations */
	for(i = 1; i <= m_pModel->nVertices; i++)
	{
		x = m_pModel->vpVertices[i].x * scalefactor;
		y = m_pModel->vpVertices[i].z * scalefactor;
		m_pModel->vpTexCoords[i] = CVector3f((x + 1.f) / 2.f, (y + 1.f) / 2.f, 0.f);
	}
	
	/* go through and put texture coordinate indices in all the triangles */
	group = m_pModel->pGroups;
	while(group)
	{
		for(i = 0; i < group->nTriangles; i++)
		{
			Tri(group->pTriangles[i]).tindices[0] = Tri(group->pTriangles[i]).vindices[0];
			Tri(group->pTriangles[i]).tindices[1] = Tri(group->pTriangles[i]).vindices[1];
			Tri(group->pTriangles[i]).tindices[2] = Tri(group->pTriangles[i]).vindices[2];
		}    
		group = group->next;
	}	
}

//////////////////////////////////////////////////////////////////////
// SpheremapTexture: Generates texture coordinates according to a
// spherical projection of the texture map.  Sometimes referred to as
// spheremap, or reflection map texture coordinates.  It generates
// these by using the normal to calculate where that vertex would map
// onto a sphere.  Since it is impossible to map something flat
// perfectly onto something spherical, there is distortion at the
// poles.  This particular implementation causes the poles along the X
// axis to be distorted.
//
// model - pointer to initialized COBJmodel structure
//////////////////////////////////////////////////////////////////////
void CAccessObj::SpheremapTexture()
{
	COBJgroup* group;
	float theta, phi, rho, x, y, z, r;
	unsigned int i;
	
	assert(m_pModel);
	assert(m_pModel->vpNormals);
	
	if (m_pModel->vpTexCoords)
		free(m_pModel->vpTexCoords);
	m_pModel->nTexCoords = m_pModel->nNormals;
	m_pModel->vpTexCoords = new CVector3f [m_pModel->nTexCoords+1];
	
	for (i = 1; i <= m_pModel->nNormals; i++)
	{
		z = m_pModel->vpNormals[i].x;	/* re-arrange for pole distortion */
		y = m_pModel->vpNormals[i].y;
		x = m_pModel->vpNormals[i].z;
		r = (float)sqrt((x * x) + (y * y));
		rho = (float)sqrt((r * r) + (z * z));
		
		if(r == 0.0f)
		{
			theta = 0.0f;
			phi = 0.0f;
		}
		else
		{
			if(z == 0.0f)
				phi = 3.14159265f / 2.0f;
			else	phi = (float)acos(z / rho);
			
			if(y == 0.0)
				theta = 3.141592365f / 2.0f;
			else	theta = (float)asin(y / r) + (3.14159265f / 2.0f);
		}
		
		m_pModel->vpTexCoords[i] = CVector3f(theta / 3.14159265f, phi / 3.14159265f, 0.0f);
	}
	
	/* go through and put texcoord indices in all the triangles */
	group = m_pModel->pGroups;
	while(group)
	{
		for (i = 0; i < group->nTriangles; i++)
		{
			Tri(group->pTriangles[i]).tindices[0] = Tri(group->pTriangles[i]).nindices[0];
			Tri(group->pTriangles[i]).tindices[1] = Tri(group->pTriangles[i]).nindices[1];
			Tri(group->pTriangles[i]).tindices[2] = Tri(group->pTriangles[i]).nindices[2];
		}
		group = group->next;
	}
}

//////////////////////////////////////////////////////////////////////
// objDelete: Deletes a COBJmodel structure.
//
// model - initialized COBJmodel structure
//////////////////////////////////////////////////////////////////////
void CAccessObj::Destory()
{
	delete m_pModel;
	m_pModel = NULL;
}

//////////////////////////////////////////////////////////////////////
// objReadOBJ: Reads a model description from a Wavefront .OBJ file.
// Returns a pointer to the created object which should be free'd with
// objDelete().
//
// filename - name of the file containing the Wavefront .OBJ format data.  
//////////////////////////////////////////////////////////////////////
void CAccessObj::LoadOBJ(char* filename)
{
	FILE*     file;

	// open the file
	file = fopen(filename, "r");
	if (!file)
	{
		fprintf(stderr, "objReadOBJ() failed: can't open data file \"%s\".\n",
			filename);
		exit(1);
	}
	
	if (m_pModel != NULL) Destory();

	// allocate a new model
	m_pModel = new COBJmodel;

	sprintf(m_pModel->pathname, "%s", filename);
	m_pModel->mtllibname[0] = '\0';

	m_pModel->nVertices	= 0;
	m_pModel->vpVertices	= NULL;
	m_pModel->nNormals	= 0;
	m_pModel->vpNormals	= NULL;
	m_pModel->nTexCoords	= 0;
	m_pModel->vpTexCoords	= NULL;
	m_pModel->nFacetnorms	= 0;
	m_pModel->vpFacetNorms	= NULL;
	m_pModel->nTriangles	= 0;
	m_pModel->pTriangles	= NULL;
	m_pModel->nMaterials	= 0;
	m_pModel->pMaterials	= NULL;
	m_pModel->nGroups	= 0;
	m_pModel->pGroups	= NULL;
	m_pModel->position	= CVector3f (0, 0, 0);
	
	// make a first pass through the file to get a count of the number
	// of vertices, normals, texcoords & triangles
	FirstPass(file);
	
	/* allocate memory */
	m_pModel->vpVertices = new CVector3f [m_pModel->nVertices + 1];
	m_pModel->pTriangles = new COBJtriangle [m_pModel->nTriangles];
	if (m_pModel->nNormals)
	{
		m_pModel->vpNormals = new CVector3f [m_pModel->nNormals + 1];
	}
	if (m_pModel->nTexCoords)
	{
		m_pModel->vpTexCoords = new CVector3f [m_pModel->nTexCoords + 1];
	}
	
	/* rewind to beginning of file and read in the data this pass */
	rewind(file);
	SecondPass(file);
	
	/* close the file */
	fclose(file);

	// Calc bounding box
	CalcBoundingBox();

	for (UINT i = 0; i < m_pModel->nMaterials; i ++)
		m_pModel->pMaterials[i].LoadTexture();
}

//////////////////////////////////////////////////////////////////////
// WriteOBJ: Writes a model description in Wavefront .OBJ format to
// a file.
//
// model    - initialized COBJmodel structure
// filename - name of the file to write the Wavefront .OBJ format data to
// mode     - a bitwise or of values describing what is written to the file
//            OBJ_NONE     -  render with only vertices
//            OBJ_FLAT     -  render with facet normals
//            OBJ_SMOOTH   -  render with vertex normals
//            OBJ_TEXTURE  -  render with texture coords
//            OBJ_COLOR    -  render with colors (color material)
//            OBJ_MATERIAL -  render with materials
//            OBJ_COLOR and OBJ_MATERIAL should not both be specified.  
//            OBJ_FLAT and OBJ_SMOOTH should not both be specified.  
//////////////////////////////////////////////////////////////////////
void CAccessObj::WriteOBJ(char* filename, unsigned int mode)
{
	unsigned int    i;
	FILE*     file;
	COBJgroup* group;
	
	assert(m_pModel);
	
	/* do a bit of warning */
	if (mode & OBJ_FLAT && !m_pModel->vpFacetNorms)
	{
		printf("WriteOBJ() warning: flat normal output requested "
			"with no facet normals defined.\n");
		mode &= ~OBJ_FLAT;
	}
	if (mode & OBJ_SMOOTH && !m_pModel->vpNormals)
	{
		printf("WriteOBJ() warning: smooth normal output requested "
			"with no normals defined.\n");
		mode &= ~OBJ_SMOOTH;
	}
	if (mode & OBJ_TEXTURE && !m_pModel->vpTexCoords)
	{
		printf("WriteOBJ() warning: texture coordinate output requested "
			"with no texture coordinates defined.\n");
		mode &= ~OBJ_TEXTURE;
	}
	if (mode & OBJ_FLAT && mode & OBJ_SMOOTH)
	{
		printf("WriteOBJ() warning: flat normal output requested "
			"and smooth normal output requested (using smooth).\n");
		mode &= ~OBJ_FLAT;
	}
	if (mode & OBJ_COLOR && !m_pModel->pMaterials)
	{
		printf("WriteOBJ() warning: color output requested "
			"with no colors (materials) defined.\n");
		mode &= ~OBJ_COLOR;
	}
	if (mode & OBJ_MATERIAL && !m_pModel->pMaterials)
	{
		printf("WriteOBJ() warning: material output requested "
			"with no materials defined.\n");
		mode &= ~OBJ_MATERIAL;
	}
	if (mode & OBJ_COLOR && mode & OBJ_MATERIAL)
	{
		printf("WriteOBJ() warning: color and material output requested "
			"outputting only materials.\n");
		mode &= ~OBJ_COLOR;
	}
	
	
	/* open the file */
	file = fopen(filename, "w");
	if (!file)
	{
		fprintf(stderr, "WriteOBJ() failed: can't open file \"%s\" to write.\n",
			filename);
		exit(1);
	}
	
	/* spit out a header */
	fprintf(file, "#  \n");
	fprintf(file, "#  Wavefront OBJ\n");
	fprintf(file, "#  \n");
	
	if (mode & OBJ_MATERIAL && m_pModel->mtllibname)
	{
		fprintf(file, "\nmtllib %s\n\n", m_pModel->mtllibname);
		WriteMTL(filename, m_pModel->mtllibname);
	}
	
	/* spit out the vertices */
	fprintf(file, "\n");
	fprintf(file, "# %d vertices\n", m_pModel->nVertices);
	for (i = 1; i <= m_pModel->nVertices; i++)
	{
		fprintf(file, "v %f %f %f\n", 
			m_pModel->vpVertices[i].x,
			m_pModel->vpVertices[i].y,
			m_pModel->vpVertices[i].z);
	}
	
	/* spit out the smooth/flat normals */
	if (mode & OBJ_SMOOTH)
	{
		fprintf(file, "\n");
		fprintf(file, "# %d normals\n", m_pModel->nNormals);
		for (i = 1; i <= m_pModel->nNormals; i++)
		{
			fprintf(file, "vn %f %f %f\n", 
				m_pModel->vpNormals[i].x,
				m_pModel->vpNormals[i].y,
				m_pModel->vpNormals[i].z);
		}
	}
	else if (mode & OBJ_FLAT)
	{
		fprintf(file, "\n");
		fprintf(file, "# %d normals\n", m_pModel->nFacetnorms);
		for (i = 1; i <= m_pModel->nNormals; i++)
		{
			fprintf(file, "vn %f %f %f\n", 
				m_pModel->vpFacetNorms[i].x,
				m_pModel->vpFacetNorms[i].y,
				m_pModel->vpFacetNorms[i].z);
		}
	}
	
	/* spit out the texture coordinates */
	if (mode & OBJ_TEXTURE)
	{
		fprintf(file, "\n");
		fprintf(file, "# %d texcoords\n", m_pModel->nTexCoords);
		for (i = 1; i <= m_pModel->nTexCoords; i++)
		{
			fprintf(file, "vt %f %f\n", 
				m_pModel->vpTexCoords[i].x,
				m_pModel->vpTexCoords[i].y);
		}
	}
	
	fprintf(file, "\n");
	fprintf(file, "# %d groups\n", m_pModel->nGroups);
	fprintf(file, "# %d faces (triangles)\n", m_pModel->nTriangles);
	fprintf(file, "\n");
	
	group = m_pModel->pGroups;
	while(group)
	{
		fprintf(file, "g %s\n", group->name);
		if (mode & OBJ_MATERIAL)
			fprintf(file, "usemtl %s\n", m_pModel->pMaterials[group->material].name);
		for (i = 0; i < group->nTriangles; i++)
		{
			if (mode & OBJ_SMOOTH && mode & OBJ_TEXTURE)
			{
				fprintf(file, "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
					Tri(group->pTriangles[i]).vindices[0], 
					Tri(group->pTriangles[i]).nindices[0], 
					Tri(group->pTriangles[i]).tindices[0],
					Tri(group->pTriangles[i]).vindices[1],
					Tri(group->pTriangles[i]).nindices[1],
					Tri(group->pTriangles[i]).tindices[1],
					Tri(group->pTriangles[i]).vindices[2],
					Tri(group->pTriangles[i]).nindices[2],
					Tri(group->pTriangles[i]).tindices[2]);
			}
			else if (mode & OBJ_FLAT && mode & OBJ_TEXTURE)
			{
				fprintf(file, "f %d/%d %d/%d %d/%d\n",
					Tri(group->pTriangles[i]).vindices[0],
					Tri(group->pTriangles[i]).findex,
					Tri(group->pTriangles[i]).vindices[1],
					Tri(group->pTriangles[i]).findex,
					Tri(group->pTriangles[i]).vindices[2],
					Tri(group->pTriangles[i]).findex);
			}
			else if (mode & OBJ_TEXTURE)
			{
				fprintf(file, "f %d/%d %d/%d %d/%d\n",
					Tri(group->pTriangles[i]).vindices[0],
					Tri(group->pTriangles[i]).tindices[0],
					Tri(group->pTriangles[i]).vindices[1],
					Tri(group->pTriangles[i]).tindices[1],
					Tri(group->pTriangles[i]).vindices[2],
					Tri(group->pTriangles[i]).tindices[2]);
			}
			else if (mode & OBJ_SMOOTH)
			{
				fprintf(file, "f %d//%d %d//%d %d//%d\n",
					Tri(group->pTriangles[i]).vindices[0],
					Tri(group->pTriangles[i]).nindices[0],
					Tri(group->pTriangles[i]).vindices[1],
					Tri(group->pTriangles[i]).nindices[1],
					Tri(group->pTriangles[i]).vindices[2], 
					Tri(group->pTriangles[i]).nindices[2]);
			}
			else if (mode & OBJ_FLAT)
			{
				fprintf(file, "f %d//%d %d//%d %d//%d\n",
					Tri(group->pTriangles[i]).vindices[0], 
					Tri(group->pTriangles[i]).findex,
					Tri(group->pTriangles[i]).vindices[1],
					Tri(group->pTriangles[i]).findex,
					Tri(group->pTriangles[i]).vindices[2],
					Tri(group->pTriangles[i]).findex);
			}
			else
			{
				fprintf(file, "f %d %d %d\n",
					Tri(group->pTriangles[i]).vindices[0],
					Tri(group->pTriangles[i]).vindices[1],
					Tri(group->pTriangles[i]).vindices[2]);
			}
		}
		fprintf(file, "\n");
		group = group->next;
	}
	
	fclose(file);
}


void CAccessObj::Boundingbox(CVector3f &vMax, CVector3f &vMin)
{
	vMax = m_vMax;
	vMin = m_vMin;
}

void CAccessObj::CalcBoundingBox()
{
	unsigned int i;
	CVector3f vCent;

	m_vMax = m_vMin = m_pModel->vpVertices[1];
	for (i = 1; i <= m_pModel->nVertices; i++)
	{
		if (m_vMax.x < m_pModel->vpVertices[i].x)
			m_vMax.x = m_pModel->vpVertices[i].x;
		if (m_vMin.x > m_pModel->vpVertices[i].x)
			m_vMin.x = m_pModel->vpVertices[i].x;
		
		if (m_vMax.y < m_pModel->vpVertices[i].y)
			m_vMax.y = m_pModel->vpVertices[i].y;
		if (m_vMin.y > m_pModel->vpVertices[i].y)
			m_vMin.y = m_pModel->vpVertices[i].y;
		
		if (m_vMax.z < m_pModel->vpVertices[i].z)
			m_vMax.z = m_pModel->vpVertices[i].z;
		if (m_vMin.z > m_pModel->vpVertices[i].z)
			m_vMin.z = m_pModel->vpVertices[i].z;
	}

	vCent = (m_vMax + m_vMin)*0.5f;

	for (i = 1; i <= m_pModel->nVertices; i++)
		m_pModel->vpVertices[i] = m_pModel->vpVertices[i] - vCent;

	m_vMax = m_vMax - vCent;
	m_vMin = m_vMin -vCent;
}

